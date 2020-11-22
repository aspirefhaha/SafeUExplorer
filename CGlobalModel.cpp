#ifdef UNICODE
#undef UNICODE
#endif

#include "CGlobalModel.h"
#include <QImage>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>
#include <QDebug>
#include <QList>
#include <QStringList>
#include <QFileIconProvider>
#include "CommonTools.h"
#include "CFSPrivate.h"

static bool needSDebug = false;

CGlobalModel::CGlobalModel(QObject *parent)
	: QAbstractItemModel(parent)
{
    //QStringList resultDriversName;
    FSPrivate * pRootItem = new FSPrivate(FTUSAFE,tr("SafeUDiskDataZone"),0,0,"/",NULL);
    m_rootDrives.append(pRootItem);
    foreach (QFileInfo my_info, QDir::drives())
    {
        //resultDriversName.push_back(my_info.absolutePath());
        QString vl = GetVolumeLabel(my_info.absolutePath());
        //qDebug() << "VolumeName:" << vl;
        QString label = my_info.absolutePath();
        if(label.endsWith("/")){
            label = label.left(label.size() -1);
        }
        if(!vl.isEmpty()){
            label = vl+"(" + label + ")";
        }
        pRootItem = new FSPrivate(FTLDRIVE,label,0,0,my_info.absolutePath(),NULL);
        m_rootDrives.append(pRootItem);
    }


    //qDebug() << resultDriversName;
	
}

CGlobalModel::~CGlobalModel()
{
	for(int i = 0; i < m_rootDrives.count(); i++)
	{
        FSPrivate * rootItem = m_rootDrives.at(i);
//        if(rootItem->m_pexfatRoot){
//            //TODO
//            //exfat_unmount(rootItem->m_pexfatRoot);
//            free(rootItem->m_pexfatRoot);
//        }
	}
    foreach(auto item,m_rootDrives)
    {
        m_rootDrives.removeOne(item);
        //qDebug()<<"qlist.size()="<<m_rootDrives.size();
    }
//    qDeleteAll(m_allItems);
}


QModelIndex CGlobalModel::index(int row, int column,
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
            FSPrivate * parentItemPtr = static_cast<FSPrivate *>(parent.internalPointer());
			
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
//							FSPrivate * pFind = NULL;
//							pFind = findOutFSChild(childPath,childType);
//							if(pFind == NULL){
//								pFind = new FSPrivate(childPath,childType,row,column,parentItemPtr);
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
//							FSPrivate * pFind = NULL;
//							pFind = findOutFSChild(childPath,childType);
//							if(pFind == NULL){
//								pFind = new FSPrivate(childPath,childType,row,column,parentItemPtr);
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

//void CLocalModel::editFile(const QString&)
//{
	
//}


QVariant CGlobalModel::headerData(int section, Qt::Orientation orientation, int role ) const
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
    default: return QVariant();
    }
    return returnValue;
}
QVariant CGlobalModel::data(const QModelIndex & index,
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
        FSPrivate * selPtr = static_cast<FSPrivate *>(index.internalPointer());
        FSITEMTYPE fstype = selPtr->fstype;
		QFileIconProvider icon_provider;
		switch(fstype){
        case FTUSAFE:
            {
                QIcon icon = icon_provider.icon(QFileIconProvider::Network);
                return QVariant(icon);
            }
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
        FSPrivate * selPtr = static_cast<FSPrivate *>(index.internalPointer());
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

				switch(index.column()){
				case 0:
					return QVariant(filename);
					break;

				default:
                    qDebug() << "!!!!!!!!!!!should not here!!!!!!!!!!!!!!";
					break;
				}
			}
			break;
        case FTUSAFE:
        case FTLDRIVE:
			{
                QString fslabel = selPtr->m_label;
                return QVariant(fslabel);
			}
			break;
		}
			
        return QVariant("unknown");
    }
    //return QAbstractItemModel::data();
	return QVariant();
}


QModelIndex CGlobalModel::parent(const QModelIndex &child) const
{
	//qDebug() << "call parent with cild  row " << child.row() << " column " << child.column() ;
    //* 如果是无效的节点则返回无效节点
	if(!child.isValid())
	{
		//qDebug() << "no parent ";
		return QModelIndex();
	}

    FSPrivate* childData = static_cast<FSPrivate*>(child.internalPointer());
	for(int i = 0; i < m_rootDrives.count(); i++)
	{
		if(m_rootDrives[i] == childData) //* 如果是父节点（分组）则返回无效父节点（分组没有父节点）
		{
			//qDebug() << "no parent ";
			return QModelIndex();
		}
	}
//	for(int i = 0; i<m_allItems.count();i++){
//		FSPrivate* item = m_allItems[i];
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
//		FSPrivate * rootItem = m_rootDrives.at(i);
//		if(rootItem->m_pexfatRoot){
//			//TODO
//            //exfat_unmount(rootItem->m_pexfatRoot);
//			free(rootItem->m_pexfatRoot);
//		}
//	}
//	m_rootDrives.clear();

//}

//bool CLocalModel::isRootItem(FSPrivate * priv) const
//{
//	bool isrootitem = false;
//	QList<FSPrivate*>::const_iterator litem = m_rootDrives.begin();
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
//	QList<FSPrivate*>::iterator litem = m_rootDrives.begin();
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
//			FSPrivate * newItem = new FSPrivate(devname,fstype,m_rootDrives.size(),0,NULL);
//			newItem->m_pexfatRoot = ef;
//			this->m_rootDrives.append(newItem);
//			this->m_allItems.append(newItem);
//		}
//	}
//}

int CGlobalModel::rowCount(const QModelIndex &parent ) const
{
    if(!parent.isValid()){
		return m_rootDrives.size();
	}
	else{
        FSPrivate * parentData = static_cast<FSPrivate*>(parent.internalPointer());
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
int CGlobalModel::columnCount(const QModelIndex &parent ) const
{
    return 1;
}

