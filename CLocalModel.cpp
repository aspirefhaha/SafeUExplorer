#ifdef UNICODE
#undef UNICODE
#endif

#include "CLocalModel.h"
#include <qimage.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qdatetime.h>
#include <qdebug.h>
#include <qfileiconprovider.h>

#include <Windows.h>


static bool needSDebug = false;

CLocalModel::CLocalModel(QObject *parent)
	: QAbstractItemModel(parent)
{

	
}

CLocalModel::~CLocalModel()
{
	for(int i = 0; i < m_rootDrives.count(); i++)
	{
//		ExfatFSPrivate * rootItem = m_rootDrives.at(i);
//		if(rootItem->m_pexfatRoot){
//			//TODO
//            //exfat_unmount(rootItem->m_pexfatRoot);
//			free(rootItem->m_pexfatRoot);
//		}
	}
//    qDeleteAll(m_allItems);
}

//ExfatFSPrivate * CLocalModel::findOutFSChild(QString abspath,FSITEMTYPE type) const{
//	QList<ExfatFSPrivate *>::const_iterator pitem= m_allItems.begin();
//	while(pitem != m_allItems.end()){
//		if((*pitem)->match(abspath,type)){
//			return *pitem;
//		}
//		pitem++;
//	}
//	return NULL;
//}

//bool CLocalModel::isDirectory(const char * dirpath)
//{

//	return false;
//}

QModelIndex CLocalModel::index(int row, int column,
                  const QModelIndex &parent) const
{
	//qDebug() << "call index with row" << row << " col " << column  << " parent row " << parent.row() << " column " << parent.column() ;

	if (!parent.isValid())
    {
        // 首层节点绑定关系
        if (row < m_rootDrives.size() && row != -1)
            return createIndex(row, column, m_rootDrives.at(row));
		else
			return QModelIndex();
    }
    else
    {
        // 其它层节点绑定关系
        if (parent.internalPointer() != nullptr)
        {
            ExfatFSPrivate * parentItemPtr = static_cast<ExfatFSPrivate *>(parent.internalPointer());
			
			QString childPath ;
            FSITEMTYPE childType = FTUNKNOWN;
//			switch(parentItemPtr->fstype){
//            case FTDRIVE:
//				{
//					//根据路径，row行数,返回绝对路径和类型
//					char lpPath[EXFAT_UTF8_NAME_BUFFER_MAX] = {0};
//					struct exfat * ef = parentItemPtr->m_pexfatRoot;
//					struct exfat_node * pparentnode = exfat_get_node(ef->root);
//					struct exfat_node * node = NULL;
//					int rc =  0;
//					int searchidx = 0;
//					if(pparentnode != NULL){
//						struct exfat_iterator it;
//						rc = exfat_opendir(ef, pparentnode, &it);
//						if (rc != 0)
//							break;
//						while ((node = exfat_readdir(&it)))
//						{
//							if(row == searchidx){
//								if(node->attrib & EXFAT_ATTRIB_DIR){
//									childType = EXFTDIR;
									
//								}
//								else {
//									childType = EXFTFILE;
//								}
//								int len = exfat_utf16_length((const le16_t *)&node->name);
//								childPath = QString("/") +QString::fromUtf16((const  unsigned short *)&node->name,len);

//								exfat_put_node(ef, node);
//								break;
//							}
//							exfat_put_node(ef, node);
//							searchidx++;
//						}
//						exfat_closedir(ef, &it);
//						exfat_put_node(ef,pparentnode);
//						if(childType != EXFTUNKNOWN){
//							ExfatFSPrivate * pFind = NULL;
//							pFind = findOutFSChild(childPath,childType);
//							if(pFind == NULL){
//								pFind = new ExfatFSPrivate(childPath,childType,row,column,parentItemPtr);
//                                (const_cast<CLocalModel*>(this))->m_allItems.append(pFind);
//							}
//							return createIndex(row,column,pFind);
//						}

//					}

					

//				}
//				break;
//            case FTDIR:
//				{
//					//根据路径，row行数,返回绝对路径和类型
//					char lpPath[EXFAT_UTF8_NAME_BUFFER_MAX] = {0};
//					int len = 0;
//					struct exfat_node * pparentnode = NULL;
//					struct exfat * ef = parentItemPtr->m_pexfatRoot;
//					struct exfat_node * node = NULL;
//					int rc = 0;
//					int searchidx = 0;
//					exfat_utf16_to_utf8(lpPath,(const le16_t *)parentItemPtr->absPath.data(),EXFAT_UTF8_NAME_BUFFER_MAX,parentItemPtr->absPath.length());
//					rc = exfat_lookup(ef,&pparentnode,lpPath);
					
//					if(rc == 0){
//						struct exfat_iterator it;
//						rc = exfat_opendir(ef, pparentnode, &it);
//						if (rc != 0)
//							break;
//						while ((node = exfat_readdir(&it)))
//						{
//							if(row == searchidx){
//								int len = exfat_utf16_length((const le16_t *)&node->name);
//								QString filename = QString::fromUtf16((const  unsigned short *)&node->name,len);
//								if(node->attrib & EXFAT_ATTRIB_DIR){
//									childType = EXFTDIR;
//								}
//								else
//									childType = EXFTFILE;

//								childPath = parentItemPtr->absPath + "/" + filename ;//+

//								exfat_put_node(ef, node);
//								break;
//							}
//							exfat_put_node(ef, node);
//							searchidx++;
//						}
//						exfat_closedir(ef, &it);
//						exfat_put_node(ef,pparentnode);
//						if(childType != EXFTUNKNOWN){
//							ExfatFSPrivate * pFind = NULL;
//							pFind = findOutFSChild(childPath,childType);
//							if(pFind == NULL){
//								pFind = new ExfatFSPrivate(childPath,childType,row,column,parentItemPtr);
//                                (const_cast<CLocalModel*>(this))->m_allItems.append(pFind);
//							}
//							return createIndex(row,column,pFind);
//						}

//					}

					

//				}
//				break;
//            case FTFILE:
//				{
//					return QModelIndex();
//				}
//				break;
//			default:
//				return QModelIndex();
//			}
        }
    }
    return QModelIndex();

}

char* GBKToUTF8( const char* chGBK ){	
	DWORD dWideBufSize=MultiByteToWideChar(CP_ACP, 0,(LPCSTR)chGBK,-1, NULL, 0);  	
	wchar_t * pWideBuf=new wchar_t[dWideBufSize];  	
	wmemset(pWideBuf, 0, dWideBufSize);  	
	MultiByteToWideChar(CP_ACP,0,(LPCSTR)chGBK,-1,pWideBuf,dWideBufSize); 	
	DWORD dUTF8BufSize=WideCharToMultiByte(CP_UTF8,0,(LPCWSTR)pWideBuf,-1,NULL,0,NULL,NULL);  	
	char * pUTF8Buf=new char[dUTF8BufSize];  	
	memset(pUTF8Buf, 0, dUTF8BufSize);  	
	DWORD chgSize = WideCharToMultiByte( CP_UTF8,0,(LPCWSTR)pWideBuf,-1,pUTF8Buf,dUTF8BufSize,NULL,NULL); 	
	//char tmpstr[256];
	//sprintf(tmpstr,"widechartomb ret %d\n",chgSize);
	//OutputDebugString(tmpstr);
	delete[]pWideBuf;	
	return pUTF8Buf;
}

//void CLocalModel::editFile(const QString&)
//{
	
//}


QVariant CLocalModel::headerData(int section, Qt::Orientation orientation, int role ) const
{

	QString returnValue;
	switch (role) {
    case Qt::DecorationRole:
        if (section == 0) {
            // ### TODO oh man this is ugly and doesn't even work all the way!
            // it is still 2 pixels off
            QImage pixmap(16, 1, QImage::Format_Mono);
            pixmap.fill(0);
            pixmap.setAlphaChannel(pixmap.createAlphaMask());
            return pixmap;
        }
        break;
    case Qt::TextAlignmentRole:
        return Qt::AlignLeft;
    }

    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QAbstractItemModel::headerData(section, orientation, role);
    switch (section) {
    case 0: returnValue = tr("Name");
            break;
    case 1: returnValue = tr("Size");
            break;
    case 2: returnValue =
#ifdef Q_OS_MAC
               tr("Kind", "Match OS X Finder");
#else
               tr("Type", "All other platforms");
#endif
           break;
    // Windows   - Type
    // OS X      - Kind
    // Konqueror - File Type
    // Nautilus  - Type
    case 3: returnValue = tr("Date Modified");
            break;
    default: return QVariant();
    }
    return returnValue;
}
QVariant CLocalModel::data(const QModelIndex & index,
                                    int role ) const
{
	//qDebug() << "call data with row " << index.row() << " column " << index.column() << " role " << role ;
    if(!index.isValid())
    {
        return QVariant();
    }
    if( (role == Qt::TextAlignmentRole) || (role == Qt::ForegroundRole) )
    {
		if(index.column() != 0)
			return int(Qt::AlignHCenter | Qt::AlignVCenter);
		else
			return int(Qt::AlignLeft| Qt::AlignVCenter);
    }
	
	if(role == Qt::DecorationRole && index.column() == 0){
		ExfatFSPrivate * selPtr = static_cast<ExfatFSPrivate *>(index.internalPointer()); 
        FSITEMTYPE fstype = selPtr->fstype;
		QFileIconProvider icon_provider;
		switch(fstype){
        case FTFILE:
			{
				QIcon icon = icon_provider.icon(QFileIconProvider::File);
				return QVariant(icon);
			}
			break;
        case FTLDRIVE:
			{
				QIcon icon = icon_provider.icon(QFileIconProvider::Drive);
				return QVariant(icon);
			}
			break;
        case FTDIR:
			{
				QIcon icon = icon_provider.icon(QFileIconProvider::Folder);
				return QVariant(icon);
			}
			break;
		}
	}
    if(role == Qt::DisplayRole)
    {
		//int rowcount = index.row();
		ExfatFSPrivate * selPtr = static_cast<ExfatFSPrivate *>(index.internalPointer()); 
        FSITEMTYPE fstype = selPtr->fstype;
		QString fspath = selPtr->absPath;
		//qDebug() << "want display " << fspath << " type" << fstype << " row " << index.row() << " column " << index.column() << " parent" << index.parent();
		switch(fstype){
        case FTDIR:
        case FTFILE:
			{
				//获取fspath文件的文件名、大小、类型、最后修改时间

				QString filename = selPtr->absPath;
//				struct exfat_node * node;
				QString filesize = "0";
				QString filetype = tr("File");
				QString filelastmodifytime = "1980-01-01 00::00::00";
//				char utf8str[EXFAT_UTF8_NAME_BUFFER_MAX]={0};
//				exfat_utf16_to_utf8(utf8str,(const le16_t *)fspath.data(),EXFAT_UTF8_NAME_BUFFER_MAX,fspath.length());
//				int rc = exfat_lookup(selPtr->m_pexfatRoot,&node,(const char *)&utf8str);
//				do{
//					if(rc ==0){
//						char utf8str[EXFAT_UTF8_NAME_BUFFER_MAX]={0};
//						int len = 0;
//						if(node->attrib & EXFAT_ATTRIB_DIR){
//							filetype=tr("Dir");
//							filesize = covertHumanString(node->size);
//						}
//						else{
//							//struct stat tmpstat;
//							//exfat_stat(selPtr->m_pexfatRoot,node,&tmpstat);
//							filesize = covertHumanString(node->size);
							
//						}
//						filelastmodifytime = QDateTime::fromTime_t(node->mtime).toString("yyyy-MM-dd hh:mm:ss");
//						len = exfat_utf16_length((const le16_t *)&node->name);
//						filename = QString::fromUtf16((const  unsigned short *)&node->name,len);
//						exfat_put_node(selPtr->m_pexfatRoot,node);
//					}
//				}while(0);
				switch(index.column()){
				case 0:
					return QVariant(filename);
					break;
				case 1:
					return QVariant(filesize);
					break;
				case 2:
					return QVariant(filetype);
					break;
				case 3:
					return QVariant(filelastmodifytime);
					break;
				default:
					qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!should not here!!!!!!!!!!!!!!!!!!!!!!!!!!";
					break;
				}
			}
			break;
        case FTLDRIVE:
			{
				//QFileInfo info(fspath);
//				QString filename = selPtr->m_pexfatRoot->label;
//				QString filesize = "0";
//				if(isRootItem(selPtr)){
//					qlonglong tmpsize = (qlonglong)(selPtr->m_pexfatRoot->sb->cluster_count.__u32);
//					filesize = covertHumanString( tmpsize << (selPtr->m_pexfatRoot->sb->spc_bits + selPtr->m_pexfatRoot->sb->sector_bits));
//				}
//				QString leftsize = covertHumanString(((qlonglong)exfat_count_free_clusters(selPtr->m_pexfatRoot)) << (selPtr->m_pexfatRoot->sb->spc_bits+selPtr->m_pexfatRoot->sb->sector_bits));
//				//total = le32_to_cpu(selPtr->m_pexfatRoot->sb->cluster_count);
//				//selPtr->m_pexfatRoot->sb->allocated_percent = ((total - free) * 100 + total / 2) / total;
				QString filetype = tr("Drive");
				QString filelastmodifytime = "";
//				switch(index.column()){
//				case 0:
//					return QVariant(filename+"(" + filesize+")");
//					break;
//				case 1:
//					return QVariant(leftsize);
//					break;
//				case 2:
//					return QVariant(filetype);
//					break;
//				case 3:
//					return QVariant(filelastmodifytime);
//					break;
//				default:
//					qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!here!!!!!!!!!!!!!!!!!!!!!!!!!!";
//					break;
//				}
			}
			break;
		}
			
        return QVariant("unknown");
    }
    //return QAbstractItemModel::data();
	return QVariant();
}

//int CLocalModel::formatfs(const char * spec)
//{

//	struct exfat_dev * dev = exfat_open(spec, EXFAT_MODE_RW);
//	if (dev == NULL)
//		return 1;
//	if (setupfs(dev, 9, 6, "NEWFS", 0x1314,0) != 0)
//	{
//		exfat_close(dev);
//		return 1;
//	}
//	if (exfat_close(dev) != 0)
//		return 1;
//	addRootDevice(m_FSFileName,EXFTDRIVE);
//	return 0;
//}

//int CLocalModel::resetfs()
//{
//	this->beginResetModel();
//	for(int i = 0; i < m_rootDrives.count(); i++)
//	{
//		ExfatFSPrivate * rootItem = m_rootDrives.at(i);
//		if(rootItem->m_pexfatRoot){
//			//TODO
//			exfat_unmount(rootItem->m_pexfatRoot);
//			free(rootItem->m_pexfatRoot);
//		}
//	}
//	m_rootDrives.clear();
	
//	qDeleteAll(m_allItems);
//	m_allItems.clear();

//	formatfs(m_FSFileName.toStdString().c_str());
//	this->addRootDevice(m_FSFileName.toStdString().c_str(),EXFTDRIVE);
//	this->endResetModel();
//	return 0;
//}

QModelIndex CLocalModel::parent(const QModelIndex &child) const
{
	//qDebug() << "call parent with cild  row " << child.row() << " column " << child.column() ;
    //* 如果是无效的节点则返回无效节点
	if(!child.isValid())
	{
		//qDebug() << "no parent ";
		return QModelIndex();
	}

	ExfatFSPrivate* childData = static_cast<ExfatFSPrivate*>(child.internalPointer());
	for(int i = 0; i < m_rootDrives.count(); i++)
	{
		if(m_rootDrives[i] == childData) //* 如果是父节点（分组）则返回无效父节点（分组没有父节点）
		{
			//qDebug() << "no parent ";
			return QModelIndex();
		}
	}
//	for(int i = 0; i<m_allItems.count();i++){
//		ExfatFSPrivate* item = m_allItems[i];
//		if(item == childData){
//			if(item->m_pParent != NULL){
//				//qDebug() << "find parent " << item->m_pParent;
//				return createIndex(item->m_pParent->m_row,item->m_pParent->m_col,item->m_pParent);
//			}
//		}
//		//return QModelIndex();
//	}
	//* 未匹配到有效的父节点数据，返回无效节点
	//qDebug() << "no parent ";
	return QModelIndex();
}

//void CLocalModel::refreshRootDevice()
//{
//	for(int i = 0; i < m_rootDrives.count(); i++)
//	{
//		ExfatFSPrivate * rootItem = m_rootDrives.at(i);
//		if(rootItem->m_pexfatRoot){
//			//TODO
//            //exfat_unmount(rootItem->m_pexfatRoot);
//			free(rootItem->m_pexfatRoot);
//		}
//	}
//	m_rootDrives.clear();

//}

//bool CLocalModel::isRootItem(ExfatFSPrivate * priv) const
//{
//	bool isrootitem = false;
//	QList<ExfatFSPrivate*>::const_iterator litem = m_rootDrives.begin();
//	while(litem != m_rootDrives.end()){
//		if((*litem)->match(priv->absPath, priv->fstype)){
//			isrootitem = true;
//			break;
//		}
//		litem++;
//	}
//	return isrootitem;
//}


//void CLocalModel::addRootDevice(QString devname,FSITEMTYPE fstype)
//{
//	bool alreadyHas = false;
//	QList<ExfatFSPrivate*>::iterator litem = m_rootDrives.begin();
//	while(litem != m_rootDrives.end()){
//		if((*litem)->match(devname, fstype)){
//			alreadyHas = true;
//			break;
//		}
//		litem++;
//	}
//	if(!alreadyHas){
//		struct exfat * ef = (struct exfat * )malloc(sizeof(struct exfat));
//		int mount_ret =  exfat_mount(ef, devname.toStdString().c_str(), "rw");
//		if(mount_ret ==0){
//			ExfatFSPrivate * newItem = new ExfatFSPrivate(devname,fstype,m_rootDrives.size(),0,NULL);
//			newItem->m_pexfatRoot = ef;
//			this->m_rootDrives.append(newItem);
//			this->m_allItems.append(newItem);
//		}
//	}
//}

int CLocalModel::rowCount(const QModelIndex &parent ) const
{
    if(!parent.isValid()){
		return m_rootDrives.size();
	}
	else{
		ExfatFSPrivate * parentData = static_cast<ExfatFSPrivate*>(parent.internalPointer());
		switch(parentData->fstype ){
        case FTLDRIVE:
			{
				int listsize = 0;
//				int rc;
//				struct exfat * ef = parentData->m_pexfatRoot;
//				struct exfat_node * prootdir  = exfat_get_node(ef->root);
//				struct exfat_node* node;
//				if(needSDebug)
//					OutputDebugString("in drive rowCount\n");
//				do{
//					struct exfat_iterator it;
//					rc = exfat_opendir(ef, prootdir, &it);
//					if (rc != 0)
//						break;
//					while ((node = exfat_readdir(&it)))
//					{
//						listsize++;
//						exfat_put_node(ef, node);
//					}
//					exfat_closedir(ef, &it);
//				}while(0);
//				exfat_put_node(ef,prootdir);
				return listsize;
			}
        case FTDIR:
			{
				//根据输入的目录绝对地址，获取下级文件和文件夹的数量
				int listsize = 0;
				int rc;
//				struct exfat * ef = parentData->m_pexfatRoot;
//				struct exfat_node * pdir ;
//				char utf8str[EXFAT_UTF8_NAME_BUFFER_MAX]={0};
//				if(needSDebug)
//					OutputDebugString("in dir rowCount\n");
//				exfat_utf16_to_utf8(utf8str,(const le16_t *)parentData->absPath.data(),EXFAT_UTF8_NAME_BUFFER_MAX,parentData->absPath.length());
//				rc = exfat_lookup(ef,&pdir,utf8str);
//				if(rc!=0)
//					return 0;
//				struct exfat_node* node;
//				do{
//					struct exfat_iterator it;
//					rc = exfat_opendir(ef, pdir, &it);
//					if (rc != 0)
//						break;
//					while ((node = exfat_readdir(&it)))
//					{
//						listsize++;
//						exfat_put_node(ef, node);
//					}
//					exfat_closedir(ef, &it);
//				}while(0);
//				exfat_put_node(ef,pdir);
				
                return listsize;
			}
			break;
        case FTFILE:
			return 0;
		default:
			return 0;
		} 
			
	}
	return 0;
}
int CLocalModel::columnCount(const QModelIndex &parent ) const
{
    return 4;
}

