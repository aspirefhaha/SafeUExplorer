#ifndef FILETABLEWIDGET_H
#define FILETABLEWIDGET_H

#include <QObject>
#include <QTableWidget>

class FileTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    explicit FileTableWidget(QWidget * parent=nullptr);
    ~FileTableWidget();

signals:
    void startDrag();
	void acceptItemList(QList<int>);

protected:
	void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif // FILETABLEWIDGET_H
