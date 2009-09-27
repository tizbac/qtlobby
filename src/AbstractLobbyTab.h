// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef ABSTRACTLOBBYTAB_H
#define ABSTRACTLOBBYTAB_H
#include <QWidget>
#include <QObject>
#include <QIcon>
#include <QMap>
#include <QStandardItem>

#include "Command.h"
#include "Battles.h"

/**
 * This is an abstract class, all lobby tab objects should derive from it
 */
class AbstractLobbyTab : public QObject {
    Q_OBJECT
public:
    AbstractLobbyTab(  QObject * parent );
    virtual ~AbstractLobbyTab();
    virtual void setupUi( QWidget * tab ) = 0;
    virtual void setActive( bool isActive ) = 0;
    void setUserNameCountryCodeMap( QMap<QString, QString>* userNameCountryCodeMap );
    virtual QString getTabTitle();
    virtual void setItem(QStandardItem* item);
    QStandardItem* getItem();

    QString myUserName;
    QIcon icon;
    QColor color;
    bool isActive;
    QWidget* widget;
    int currentTabIndex;

signals:
    void sendCommand( Command );
    void sendInput(QString input);
    void closeMe();

public slots:
    virtual void receiveInput( QString input ) = 0;
    virtual void receiveCommand( Command command ) = 0;

protected:
    QMap<QString, QString>* userNameCountryCodeMap;
    QStandardItem* item;
};
Q_DECLARE_METATYPE(AbstractLobbyTab*);

inline void AbstractLobbyTab::setItem(QStandardItem* item) {
    this->item = item;
    item->setText(objectName());
}

inline QStandardItem* AbstractLobbyTab::getItem() {
    return item;
}

#endif
