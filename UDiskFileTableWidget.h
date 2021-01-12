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
	void DelUdiskItem(QModelIndex);

protected slots:
	void show_menu(QPoint);
	void clickgoose();

protected:
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	QModelIndex m_selIndex;
	QMenu *menu;
	QMenu *menuProperty;
	QAction *pnew ;
	QAction * propertyAction;
};

#endif // UDISKFILETABLEWIDGET_H
