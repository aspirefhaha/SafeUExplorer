#ifndef BGCOPYTHREAD_H
#define BGCOPYTHREAD_H


#include <QObject>
#include <QThread>
#include "CopyItem.h"

class BGCopyThread : public QThread
{
    Q_OBJECT
public:
    explicit BGCopyThread(QObject * parent=0);
    ~BGCopyThread();
    QList<CopyItem> copyItems;
	QList<CopyItem> realItems;
	bool m_bQuit;

	void try_quit() {
		m_bQuit = true;
	}
	qint64 m_TotalSize;
	qint64 m_FinishedSize;
	int m_TotalCount;
	void addLocalSourceToRealItems(CopyItem diritem);
	void addUDiskSourceToRealItems(CopyItem diritem);
	void RemovePath(QString path);
	quint64 getDiskFreeSpace(QString driver);
	struct exfat * ef;
	QList<QString> m_DelItemList;

Q_SIGNALS:
	void total(int total, qint64 totalsize);
	void curFinished(qint64 size, int finisheditem);
	void curItem(QString source, QString target);
	void curFileProg(qint64 filesize, qint64  finishedsize);
	void speed(qreal speed);
	void workFinished();
	void LeftSizeError(qint64 needSize, qint64 leftSize);

protected:
    void run() override;
	void removeUDiskFile(struct exfat_node * pnode);
	void removeUDiskDir(struct exfat_node * pnode);

private:
	void addUDiskDelSubFiles(QString curpath,struct exfat_node * pnode, QList<QString> & list);
	void addUDiskDelFile(QString filename, QList<QString> & list);
};

#endif // BGCOPYTHREAD_H
