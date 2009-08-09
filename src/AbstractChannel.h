// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef ABSTRACTCHANNEL_H
#define ABSTRACTCHANNEL_H

#include <QTextBrowser>
#include <QScrollBar>
#include <QTime>
#include <QGridLayout>
#include <QDesktopServices>

#include "Command.h"
#include "AbstractLobbyTab.h"
#include "ChannelTextBrowser.h"

class AbstractChannel : public AbstractLobbyTab {
    Q_OBJECT
public:
    AbstractChannel( QString name, QObject * parent = 0 );
    virtual ~AbstractChannel();
    static void setCurrentUsername(QString user);

public slots:
    virtual void receiveInput( QString input ) = 0;
    virtual void receiveCommand( Command command ) = 0;
protected:
    void setActive( bool isActive );
    virtual void setupUi( QWidget * tab );
    bool executeChannelInput( QString input );
    void insertLine( QString line );
    QString makeHtml( QString in );
    QString processBBCodes(QString in);
    QString processInput(QString input, bool formatting = true);
    QString highlightUserName( QString input );
    QString processIRCCodes(QString in);
    QString urlify(QString in);
    ChannelTextBrowser * channelTextBrowser;
    QGridLayout * gridLayout;
    QIcon activeIcon;
    QIcon inactiveIcon;
    QColor activeTextColor;
    QColor inactiveTextColor;
    static QString currentUsername;
    QString userNameLink( const QString userName );
protected slots:
    void anchorClicked(QUrl url);
};

#endif
