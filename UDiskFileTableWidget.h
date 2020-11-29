#ifndef UDISKFILETABLEWIDGET_H
#define UDISKFILETABLEWIDGET_H

#include <QObject>

#include <QTableWidget>

class UDiskFileTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit  UDiskFileTableWidget(QWidget * parent=nullptr);
    ~UDiskFileTableWidget();

signals:
    void startDrag();
	void acceptItemList(QList<int>);

protected:
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif // UDISKFILETABLEWIDGET_H
