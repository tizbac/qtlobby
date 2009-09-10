// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef ABSTRACTCHANNEL_H
#define ABSTRACTCHANNEL_H

#include <QTextBrowser>
#include <QScrollBar>
#include <QTime>
#include <QDateTime>
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
    void setChannelBrowser(ChannelTextBrowser* b);
    void setHistoryMode(bool b);

public slots:
    virtual void receiveInput( QString input ) = 0;
    virtual void receiveCommand( Command command ) = 0;
    virtual void historyMessage( QDateTime time, QString message );

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
    QString userNameLink( const QString userName );
    void insertBlock(QTextCursor& c);

    ChannelTextBrowser * channelTextBrowser;
    QGridLayout * gridLayout;
    QIcon activeIcon;
    QIcon inactiveIcon;
    QColor activeTextColor;
    QColor inactiveTextColor;
    bool historyMode;
    QDateTime historyDateTime;
    QDateTime previous;
    bool firstBlock;

protected slots:
    void anchorClicked(QUrl url);
};

#endif
