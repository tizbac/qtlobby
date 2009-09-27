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
    void setInlineHistoryMode(bool b);

public slots:
    virtual void receiveInput( QString input ) = 0;
    virtual void receiveCommand( Command command ) = 0;
    virtual void historyMessage( QDateTime time, QString message );
    void scrollToMaximum();
    void requestHistoryReplay(QDate from, QDate to);

signals:
    void needReplay(QDate from, QDate to);

private slots:
    void onScrollBarValueChanged(int value);
    void onHistoryFinished();

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
    QString flag( const QString userName );

    ChannelTextBrowser * channelTextBrowser;
    QGridLayout * gridLayout;
    bool historyMode;
    QDateTime historyDateTime;
    QDateTime previous;
    bool firstBlock;
    bool scrollToMax;
    bool inlineHistoryMode;
    QStringList historyBuffer;

protected slots:
    void anchorClicked(QUrl url);
};

#endif
