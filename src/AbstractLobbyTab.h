// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef ABSTRACTLOBBYTAB_H
#define ABSTRACTLOBBYTAB_H
#include <QWidget>
#include <QObject>
#include <QIcon>
#include <QMap>

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

    QString myUserName;
    QIcon icon;
    QColor color;
    bool isActive;
    QWidget* currentWidget;
    int currentTabIndex;

signals:
    void sendCommand( Command );
    void sendInput(QString input);
    void closeMe();

public slots:
    virtual void receiveInput( QString input ) = 0;
    virtual void receiveCommand( Command command ) = 0;

protected:
    QString flag( const QString userName );
    QMap<QString, QString>* userNameCountryCodeMap;
};

#endif
