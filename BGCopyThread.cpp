#include "exfat.h"
#include "BGCopyThread.h"

#define RWONCESIZE 40960

BGCopyThread::BGCopyThread(QObject * parent)
    :QThread(parent),
	m_bQuit(false),
	m_TotalSize(1),
	m_TotalCount(0),
	ef(NULL)
{

}

BGCopyThread::~BGCopyThread()
{

}

void BGCopyThread::addUDiskSourceToRealItems(CopyItem diritem)
{
	CopyItem realitem;
	realitem.sourceType = FTDIR;
	realitem.size = 0;
	realitem.source = diritem.source;
	realitem.target = diritem.targetDir + "/" + diritem.source.mid(diritem.sourceDir.length(), diritem.source.length() - diritem.sourceDir.length());
	realitem.targetType = FTLDIR;
	realItems.append(realitem);
	m_TotalCount++;
	m_TotalSize += realitem.size;
	struct exfat_node * pdirnode;
	struct exfat_iterator it;
	struct exfat_node * node;
	if (exfat_lookup(ef, &pdirnode, diritem.source.toUtf8().data()) == 0) {
		int rc = exfat_opendir(ef, pdirnode, &it);
		if (rc != 0)
			return;
		while ((node = exfat_readdir(&it)))
		{
			int len = exfat_utf16_length((const le16_t *)&node->name);
			QString nodename = QString::fromUtf16((const  unsigned short *)&node->name, len);
			if (node->attrib & EXFAT_ATTRIB_DIR) {
				CopyItem copyitem;
				copyitem.sourceType = FTDIR;
				
				copyitem.source = diritem.source + "/" + nodename;
				copyitem.targetDir = diritem.targetDir;
				copyitem.target = realitem.target + "/" + nodename;
				copyitem.sourceDir = diritem.sourceDir;
				copyitem.size = 0;
				addUDiskSourceToRealItems(copyitem);
			}
			else {
				CopyItem fileitem;
				fileitem.size = node->size;
				fileitem.source = diritem.source + "/" + nodename;
				fileitem.target = realitem.target + "/" + nodename;
				fileitem.sourceType = FTFILE;
				fileitem.targetType = FTLFILE;
				m_TotalCount++;
				m_TotalSize += fileitem.size;
				realItems.append(fileitem);
			}
			exfat_put_node(ef, node);
		}
		exfat_closedir(ef, &it);
	}
	
}

void BGCopyThread::addLocalSourceToRealItems(CopyItem diritem)
{
	CopyItem realitem;
	realitem.sourceType = FTLDIR;
	realitem.source = diritem.source;
	realitem.targetType = FTDIR;
	realitem.size = 0;
	QString tardir = diritem.targetDir;
	if (tardir == "/")
		tardir = "";
	realitem.target = tardir  + diritem.source.mid(diritem.sourceDir.length(), diritem.source.length() - diritem.sourceDir.length());
	realItems.append(realitem);
	m_TotalCount++;
	QDir dir(diritem.source);
	//dir.entryInfoList(QDir::Files)�����ļ���Ϣ
	foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files))
	{
		//�����ļ���С
		m_TotalSize += fileInfo.size();
		m_TotalCount++;
		CopyItem fileitem;
		fileitem.source = fileInfo.absoluteFilePath();
		QString filetardir = diritem.targetDir;
		if (filetardir == "/")
			filetardir = "";
		fileitem.target = filetardir + fileInfo.absoluteFilePath().mid(diritem.sourceDir.length(), fileInfo.absoluteFilePath().length() - diritem.sourceDir.length());
		fileitem.targetType = FTFILE;
		fileitem.sourceType = FTLFILE;
		fileitem.size = fileInfo.size();
		realItems.append(fileitem);
	}
	//dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot)����������Ŀ¼�������й���
	foreach(QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
	{
		CopyItem subdirItem;
		subdirItem.source = diritem.source + "/" + subDir;
		subdirItem.sourceDir = diritem.sourceDir;
		subdirItem.sourceType = FTLDIR;
		subdirItem.targetDir = diritem.targetDir;
		subdirItem.size = 0;
		addLocalSourceToRealItems(subdirItem);
		
	}
}

void BGCopyThread::run()
{
	m_TotalSize = 1;
	m_TotalCount = 0;
	realItems.clear();
	qDebug() << "calculating...";
	if (!m_bQuit) {
		foreach(auto copyitem, copyItems) {
			if (copyitem.sourceType == FTLDRIVE) {
				QFileInfo sourceinfo(copyitem.source);
				if (sourceinfo.isDir()) {
					addLocalSourceToRealItems(copyitem);
				}
				else {
					CopyItem realitem;
					realitem.source = copyitem.source;
					realitem.sourceType = FTLFILE;
					realitem.size = sourceinfo.size();
					QString targetpath;
					QString targetdir = copyitem.targetDir;
					if (targetdir == "/")
						targetdir = "";
					realitem.target = targetdir  + copyitem.source.mid(copyitem.sourceDir.length(), copyitem.source.length() - copyitem.sourceDir.length());
					realItems.append(realitem);
					m_TotalCount++;
					m_TotalSize += realitem.size;
				}
			}
			else if (copyitem.sourceType == FTUSAFE) {
				if (ef == NULL || ef->dev == NULL) {
					//TODo
					m_bQuit = true;
				}
				else {
					struct exfat_node * node;
					if (exfat_lookup(ef, &node, copyitem.source.toUtf8().data()) == 0) {
						if (node->attrib & EXFAT_ATTRIB_DIR) {
							addUDiskSourceToRealItems(copyitem);
						}
						else {
							CopyItem realitem;
							realitem.source = copyitem.source;
							realitem.target = copyitem.targetDir +"/" + copyitem.source.mid(copyitem.sourceDir.length(), copyitem.source.length() - copyitem.sourceDir.length());
							realitem.sourceType = FTFILE;
							realitem.targetType = FTLFILE;
							realitem.size = node->size;
							m_TotalCount++;
							m_TotalSize += node->size;
							realItems.append(realitem);
						}

						exfat_put_node(ef,node);
					}
				}
				

			}
			
		}
		emit total(m_TotalCount, m_TotalSize);
	}
	int curfile = 0;
	qint64 cursize = 0;
	for (int i = 0; i < realItems.count() && !m_bQuit;i++) {
		CopyItem realitem = realItems.at(i);
		//qDebug() << realitem.target;
		
		switch (realitem.sourceType) {
		case FTLDIR:
			exfat_mkdir(ef, realitem.target.toUtf8().data());
			break;
		case FTDIR:
			{
				QDir dir;
				if (!dir.exists(realitem.target)) {
					dir.mkdir(realitem.target);
				}
			}
			break;
		case FTFILE:// from udisk to local
			{
				struct exfat_node * pnode = NULL;
				off_t off = 0;
				char * buf = (char *)malloc(RWONCESIZE);
				ssize_t readsize = 0;
				QFile targetfile(realitem.target);
				if (!targetfile.open(QIODevice::Truncate | QIODevice::WriteOnly))
					continue;
				if (exfat_lookup(ef, &pnode, realitem.source.toUtf8().data()) == 0) {
					
					do {
						readsize = exfat_generic_pread(ef, pnode, buf, RWONCESIZE, off);
						if (readsize > 0) {
							targetfile.write(buf, readsize);
						}
						off += readsize;
					} while (readsize > 0);
				}
				targetfile.close();
				exfat_put_node(ef,pnode);
				free(buf);
			}
			break;
		case FTLFILE://from local file to udisk
			{
				int mknoderet = exfat_mknod(ef, realitem.target.toUtf8().data());
				if (mknoderet == 0) {
					struct exfat_node * node;

					if (exfat_lookup(ef, &node, realitem.target.toUtf8().data()) == 0) {
						QFile sourcef(realitem.source);
						bool isOk = sourcef.open(QIODevice::ReadOnly);
						if (isOk) {
							off_t off = 0;
							char * buf = (char *)malloc(RWONCESIZE);
							qint64 onceread = 0;
							do {
								onceread = sourcef.read(buf, RWONCESIZE);
								if (onceread <= 0) {
									break;
								}
								ssize_t writensize = exfat_generic_pwrite(ef, node, buf, onceread, off);
								if (writensize == onceread) {
									off += writensize;
								}
								else {
									break;
								}
							} while (onceread>0);

							free(buf);
							sourcef.close();
						}
						exfat_flush_node(ef, node);
						exfat_put_node(ef, node);
					}
				}
			}
			break;
		}
		cursize += realitem.size;
		curfile++;
		emit curFinished(cursize, curfile);
		emit curItem(realitem.source, realitem.target);
		//sleep(3);
	}

	qDebug() << "now thread quit";
	//sleep(1);
	emit copyFinished();
}