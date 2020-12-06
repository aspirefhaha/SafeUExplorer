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
	int m_TotalCount;
	void addLocalSourceToRealItems(CopyItem diritem);
	void addUDiskSourceToRealItems(CopyItem diritem);
	struct exfat * ef;

Q_SIGNALS:
	void total(int total, qint64 totalsize);
	void curFinished(qint64 size, int finisheditem);
	void curItem(QString source, QString target);
	void curFileProg(qint64 filesize, qint64  finishedsize);
	void copyFinished();

protected:
    void run() override;
};

#endif // BGCOPYTHREAD_H
