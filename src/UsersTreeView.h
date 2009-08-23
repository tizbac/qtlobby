#ifndef USERSTREEVIEW_H
#define USERSTREEVIEW_H

#include <QTreeView>

class UsersTreeView : public QTreeView {
    Q_OBJECT
public:
    UsersTreeView( QWidget * parent = 0 );
    ~UsersTreeView();
signals:
    void sendInputAndFocus( QString input );
protected slots:
    virtual void customContextMenuRequestedSlot( const QPoint & point );
    void doubleClickedSlot( const QModelIndex & index );
};

#endif // USERSTREEVIEW_H
