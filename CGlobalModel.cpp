#ifdef UNICODE
#undef UNICODE
#endif
#include "exfat.h"
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
static bool needCDebug = false;

CGlobalModel::CGlobalModel(QObject *parent)
	: QAbstractItemModel(parent)
{
    //QStringList resultDriversName;

	ef = new (struct exfat);
	exfat_mount(ef, "nothing", "rw");
    FSPrivate * pRootItem = new FSPrivate(FTUSAFE,tr("SafeUDiskDataZone"),0,0,"/",NULL,ef);
    m_rootDrives.append(pRootItem);
	m_allItems.append(pRootItem);
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
        m_allItems.append(pRootItem);
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
    qDeleteAll(m_allItems);
	if (ef && ef->dev) {
		exfat_unmount(ef);
	}
	delete ef;
}


QModelIndex CGlobalModel::index(int row, int column,
                  const QModelIndex &parent) const
{
    if(needCDebug)
        qDebug() << "call index with row:" << row << " col:" << column  << " parent row:" << parent.row() << " col:" << parent.column() ;

	if (!parent.isValid())
    {
        if(needSDebug)
            qDebug() << " parent invalid!!!!!!!!!!!";
        // 首层节点绑定关系
        if (row < m_rootDrives.size() && row != -1){
            FSPrivate * rootItem = m_rootDrives.at(row);
            rootItem->m_col = column;
            rootItem->m_row = row;
            return createIndex(row, column, rootItem);
        }
		else
			return QModelIndex();
    }
    else
    {
        // 其它层节点绑定关系
        if(needSDebug)
            qDebug() << "parent valid";
        if (parent.internalPointer() != nullptr)
        {
            FSPrivate * parentItemPtr = static_cast<FSPrivate *>(parent.internalPointer());
            if(needSDebug)
                qDebug() << "parent abs:" << parentItemPtr->absPath;

            FSITEMTYPE childType = FTUNKNOWN;
            switch(parentItemPtr->fstype){
              case FTLDIR:
              case FTLDRIVE:
			  case FTDIR:
			  case FTUSAFE:
                {
                    if(parentItemPtr->m_lsChildren.isEmpty()){
                        return QModelIndex();
                    }
                    return createIndex(row,column,parentItemPtr->m_lsChildren.at(row));
                }
                break;
			default:
				return QModelIndex();
            }
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
    if(needSDebug)
        qDebug() << "call data with row " << index.row() << " column " << index.column() << " role " << role ;
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
        case FTLDIR:
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
        case FTFILE:
			break;
        case FTUSAFE:
		case FTDIR:
        case FTLDIR:
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

    //* 如果是无效的节点则返回无效节点
	if(!child.isValid())
	{
        if(needSDebug)
            qDebug() << "call parent with child  row " << child.row() << " column " << child.column() << "no parent ";
		return QModelIndex();
	}

    FSPrivate* childData = static_cast<FSPrivate*>(child.internalPointer());
    if(needSDebug)
        qDebug() << "call parent with child  row " << child.row() << " column " << child.column() << "name:" << childData->absPath;
	for(int i = 0; i < m_rootDrives.count(); i++)
	{
		if(m_rootDrives[i] == childData) //* 如果是父节点（分组）则返回无效父节点（分组没有父节点）
		{
			//qDebug() << "no parent ";
			return QModelIndex();
		}
	}
    switch(childData->fstype){
    case FTLDRIVE:
	case FTUSAFE:
        return QModelIndex();
    case FTLDIR:
    case FTLFILE:
	case FTDIR:

        return createIndex(childData->m_pParent->m_row, childData->m_pParent->m_col,childData->m_pParent);
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

QModelIndex CGlobalModel::findLocalItem(QString &abspath)
{
	//TODO
	int allcount = m_allItems.count();
	FSPrivate * item = NULL;
	bool foundone = false;
	for (int i = 0; i < allcount; i++) {
		item = m_allItems.at(i);
		if (item->absPath == abspath) {
			break;
		}
	}
	return QModelIndex();
}
int CGlobalModel::rowCount(const QModelIndex &parent ) const
{
    if(!parent.isValid()){
        if(needSDebug)
            qDebug() << "call rowCount of root is:" <<  m_rootDrives.size();
		return m_rootDrives.size();
	}
	else{
        FSPrivate * parentData = static_cast<FSPrivate*>(parent.internalPointer());
        if(needCDebug)
            qDebug() << "call rowCount of " << parentData->absPath ;
		switch(parentData->fstype ){
        case FTLDIR:
        case FTLDRIVE:
            {
                if(!parentData->firsted){
                    return parentData->m_lsChildren.size();
                }
                QFileInfoList result;
                QDir dir(parentData->absPath);
                dir.setFilter(QDir::Hidden | QDir::NoSymLinks | QDir::Dirs | QDir::NoDotAndDotDot);
                dir.setSorting(QDir::Time | QDir::Reversed);
                if(needSDebug)
                    qDebug() << "ask item " + parentData->absPath +  " children size : " << dir.entryInfoList().size();
                QFileInfoList finfolist =  dir.entryInfoList();
                int row = 0;
                foreach(QFileInfo fileinfo,finfolist){
                    FSPrivate * newItem = nullptr;

                    if(needCDebug)
                        qDebug() << "create new item name " << fileinfo.fileName() << " with abs:" << fileinfo.absoluteFilePath();
                    newItem = new FSPrivate(FTLDIR,fileinfo.fileName(),row,0,fileinfo.absoluteFilePath(),parentData);
                    row++;
                    (const_cast<CGlobalModel*>(this))->m_allItems.append(newItem);
                    parentData->m_lsChildren.append(newItem);

                }
				parentData->firsted = false;
                return parentData->m_lsChildren.size();

			}
        case FTLFILE:
            return 0;
        case FTSPEC:
            //TODO
            return 0;
        case FTUSAFE:
			{
				int listsize = 0;
				int rc;
				struct exfat_node * prootdir = exfat_get_node(ef->root);
				struct exfat_node* node;

				if (!parentData->firsted) {
					return parentData->m_lsChildren.size();
				}
				do {
					struct exfat_iterator it;
					parentData->firsted = false;
					rc = exfat_opendir(ef, prootdir, &it);
					if (rc != 0)
						break;
					while ((node = exfat_readdir(&it)))
					{
						if (node->attrib & EXFAT_ATTRIB_DIR) {
							FSPrivate * newItem = nullptr;
							int len = exfat_utf16_length((const le16_t *)&node->name);
							QString dirname = QString::fromUtf16((const  unsigned short *)&node->name, len);
							QString absname = "/" + dirname;
							if (needCDebug)
								qDebug() << "create new item name " << dirname  << " with abs:" << absname;
							newItem = new FSPrivate(FTDIR, dirname, listsize++, 0, absname, parentData);
							(const_cast<CGlobalModel*>(this))->m_allItems.append(newItem);
							parentData->m_lsChildren.append(newItem);
							listsize++;
						}
						exfat_put_node(ef, node);
					}
					exfat_closedir(ef, &it);
				} while (0);
				exfat_put_node(ef, prootdir);
				return listsize;
			}
        case FTDIR:
			{
				//根据输入的目录绝对地址，获取下级文件和文件夹的数量
				int listsize = 0;
				int rc;
				struct exfat_node * pdir;
				char utf8str[EXFAT_UTF8_NAME_BUFFER_MAX] = { 0 };
				if (!parentData->firsted) {
					return parentData->m_lsChildren.size();
				}
				parentData->firsted = false;
				exfat_utf16_to_utf8(utf8str, (const le16_t *)parentData->absPath.data(), EXFAT_UTF8_NAME_BUFFER_MAX, parentData->absPath.length());
				rc = exfat_lookup(ef, &pdir, utf8str);
				if (rc != 0)
					return 0;
				struct exfat_node* node;
				FSPrivate * newItem = nullptr;
				do {
					struct exfat_iterator it;
					rc = exfat_opendir(ef, pdir, &it);
					if (rc != 0)
						break;
					while ((node = exfat_readdir(&it)))
					{
						if (node->attrib & EXFAT_ATTRIB_DIR) {
							int len = exfat_utf16_length((const le16_t *)&node->name);
							QString dirname = QString::fromUtf16((const  unsigned short *)&node->name, len);
							QString absname = parentData->absPath + "/" + dirname;
							if (needCDebug)
								qDebug() << "create new item name " << dirname << " with abs:" << absname;
							newItem = new FSPrivate(FTDIR, dirname, listsize++, 0, absname, parentData);
							(const_cast<CGlobalModel*>(this))->m_allItems.append(newItem);
							parentData->m_lsChildren.append(newItem);
							listsize++;
						}
						exfat_put_node(ef, node);
					}
					exfat_closedir(ef, &it);
				} while (0);
				exfat_put_node(ef, pdir);

				return listsize; 
				
            }
        case FTFILE:
			return 0;
        case FTUNKNOWN:
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

