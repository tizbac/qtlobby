// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "AbstractChannel.h"
#include "PathManager.h"
#include "ServerProfilesModel.h"
#include "Users.h"
#include "MainWindow.h"

AbstractChannel::AbstractChannel( QString name, QObject * parent ) : AbstractLobbyTab( parent ) {
    setObjectName( name );
    isActive = false;
    historyMode = false;
    inlineHistoryMode = false;
    previous = QDateTime::currentDateTime();
    firstBlock = true;
    firstExtraBlock = true;
    scrollToMax = true;
}

AbstractChannel::~AbstractChannel() {}

void AbstractChannel::setupUi( QWidget * channelTabWidget ) {
    channelTabWidget->setObjectName( QString::fromUtf8( "channelTabWidget" ) + objectName() );
    splitter = new QSplitter( channelTabWidget );
    channelTextBrowser = new ChannelTextBrowser( channelTabWidget );
    channelTextBrowser->setObjectName( "channelTextBrowser_" + objectName() );
    setChannelBrowser(channelTextBrowser);
    channelTextBrowser->document()->setMaximumBlockCount(500);
    channelExtraTextBrowser = new QTextBrowser( channelTabWidget );
    channelExtraTextBrowser->document()->setMaximumBlockCount(500);
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(channelExtraTextBrowser);
    splitter->addWidget(channelTextBrowser);
    splitter->setSizes(QList<int>() << 1 << 100000);
    gridLayout = new QGridLayout( channelTabWidget );
    gridLayout->setContentsMargins(0,0,0,0);
    gridLayout->setObjectName( QString::fromUtf8( "channelGridLayout" ) + objectName() );
    gridLayout->addWidget( splitter, 1, 0, 1, 1 );
    connect(channelTextBrowser, SIGNAL(anchorClicked(QUrl)),
            this, SLOT(anchorClicked(QUrl)));
    connect(channelTextBrowser->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(onScrollBarValueChanged(int)));
}

void AbstractChannel::setActive( bool isActive ) {
    this->isActive = isActive;
    if(isActive)
        item->setData(QVariant(), Qt::ForegroundRole);
}

bool AbstractChannel::executeChannelInput( QString input ) {
    if ( input.contains( "\n" ) ) {
        QStringList lines = input.split( "\n" );
        foreach( QString l, lines )
            receiveInput( l );
        return true;
    }
    QStringList inputList = input.split( " " );
    QString firstWord = inputList.takeFirst();
    Command ret;
    if ( QString( "/j,/join" ).split( "," ).contains( firstWord, Qt::CaseInsensitive ) ) {
        ret.name = "JOIN";
        ret.attributes << inputList;
        QStringList channels = inputList.join( " " ).split( "," );
        if ( channels.count() > 0 ) {
            foreach( QString c, channels ) {
                ret.attributes = QStringList();
                ret.attributes << c.trimmed();
                ret.attributes[0].replace( QRegExp( "^#" ), "" );
                emit sendCommand( ret );
            }
            return true;
        }
    } else if ( QString( "/channels,/list" ).split( "," ).contains( firstWord, Qt::CaseInsensitive ) ) {
        ret.name = "CHANNELS";
    } else if ( QString( "/msg,/query" ).split( "," ).contains( firstWord, Qt::CaseInsensitive ) ) {
        ret.name = "SAYPRIVATE";
        ret.attributes << inputList;
    } else if ( QString( "/h,/help" ).split( "," ).contains( firstWord, Qt::CaseInsensitive ) ) {
        ret.name = "CLIENTMSG";
        QString row( "<tr><td style=\"padding:0;padding-right: 10px;border:none;\">%1</td><td style=\"padding:0;border:none;\">%2</td></tr>" );
        ret.attributes << objectName() << tr("Chat Help").append(" <table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">%1</table>" ).arg(
                row.arg( "/help, /h", tr( "Display this help" ) ) +
                row.arg( "/channels, /list", tr( "Display channel list" ) ) +
                row.arg( "/join, /j - &lt;channel&gt; [&lt;password&gt; &lt;channel2&gt; &lt;password2&gt; ... ]",tr( "Join channel(s)" ) ) +
                row.arg( "/me &lt;text&gt;", tr( "Say highlighted" ) ) +
                row.arg( "/sayver", tr( "Say /me is using QtLobby vXXX revYYY" ) ) +
                row.arg( "/slap &lt;user&gt;", tr( "Say the mIRC slap sentence" ) ) +
                row.arg( "/query, /msg &lt;user&gt;", tr( "Open private chat" ) ) +
                row.arg( "/leave, /wc", tr( "Leave the channel" ) ) +
                row.arg( "/away,/back", tr( "Toggles away mode" ) ) +
                row.arg( "/rename &lt;new username&gt;", tr( "Renames account" ) ) +
                row.arg( "/changepassword,/chpw &lt;old password&gt; &lt;new password&gt;", tr( "Change your account password" ) )
                );
        receiveCommand( ret );
        return true;
    } else if ( firstWord.toLower() == "/ingame" ) {
        ret.name = "GETINGAMETIME";
        ret.attributes << inputList;
    } else if ( firstWord.toLower() == "/ring" ) {
        ret.name = "RING";
        ret.attributes << inputList;
    } else if ( firstWord.toLower() == "/ip" ) {
        ret.name = "GETIP";
        ret.attributes << inputList;
    } else if ( firstWord.toLower() == "/lastlogin" ) {
        ret.name = "GETLASTLOGINTIME";
        ret.attributes << inputList;
    } else if ( firstWord.toLower() == "/findip" ) {
        ret.name = "FINDIP";
        ret.attributes << inputList;
    } else if ( firstWord.toLower() == "/lastip" ) {
        ret.name = "LASTIP";
        ret.attributes << inputList;
    } else if ( firstWord.toLower() == "/away" ) {
        User me = Users::getCurrentUsers()->getUser(ServerProfilesModel::getInstance()->getActiveProfile().userName());
        me.userState.setAway(true);
        Users::getCurrentUsers()->onMyStateChanged(me);
        return true;
    } else if ( firstWord.toLower() == "/back" ) {
        User me = Users::getCurrentUsers()->getUser(ServerProfilesModel::getInstance()->getActiveProfile().userName());
        me.userState.setAway(false);
        Users::getCurrentUsers()->onMyStateChanged(me);
        return true;
    } else if ( firstWord.toLower() == "/rename" ) {
        ConnectionWidget* cw = MainWindow::getInstance()->getConnectionDialog();
        if(inputList.size())
            cw->showRename(inputList.takeFirst());
        else
            cw->showRename(QString());
        return true;
    } else if ( firstWord.toLower() == "/changepassword" || firstWord.toLower() == "/chpw" ) {
        ConnectionWidget* cw = MainWindow::getInstance()->getConnectionDialog();
        if(inputList.size() >= 2)
            cw->showChangePassword(inputList[0], inputList[1]);
        else
            cw->showChangePassword(QString(), QString());
        return true;
    } else
        return false;
    emit sendCommand( ret );
    return true;
}

void AbstractChannel::insertBlock(QTextCursor& c, bool& var) {
    if(var) {
        var = false;
        return;
    }
    c.insertBlock(QTextBlockFormat());
}

void AbstractChannel::insertLine( QString line, bool extra ) {
    QDateTime t;
    if(historyMode || inlineHistoryMode)
        t = historyDateTime;
    else
        t = QDateTime::currentDateTime();
    QTextCursor c;
    if(!inlineHistoryMode) {
        scrollToMax = channelTextBrowser->verticalScrollBar()->value() == channelTextBrowser->verticalScrollBar()->maximum();
        if(extra)
            c = channelExtraTextBrowser->textCursor();
        else
            c = channelTextBrowser->textCursor();
        c.movePosition(QTextCursor::End);
    }
    if(previous.date().day() != t.date().day()) {
        if(inlineHistoryMode) {
            historyBuffer << makeHtml("<b>" + t.toString( "MMMM d, dddd, yyyy") + "</b>" );
        } else {
            insertBlock(c, extra ? firstExtraBlock : firstBlock);
            c.insertHtml( makeHtml("<b>" + t.toString( "MMMM d, dddd, yyyy" ) + "</b>" ) );
        }
    }
    previous = t;
    QString timeString = QString( "<span style=\"color:gray;\">%1</span> " ).arg( t.toString( "[hh:mm:ss]" ) );
    if(inlineHistoryMode) {
        historyBuffer << "<font color=\"gray\">"+timeString.append( line ).remove(QRegExp("<[a-zA-Z\\/][^>]*>"))+"</font>";
    } else {
        insertBlock(c, extra ? firstExtraBlock : firstBlock);
        c.insertHtml( makeHtml( timeString.append( line ) ) );
    }
    if(extra)
        channelExtraTextBrowser->verticalScrollBar()->setValue(channelExtraTextBrowser->verticalScrollBar()->maximum());
    else
        scrollToMaximum();
    if ( extra && !isActive) {
        item->setData(QBrush(Qt::gray), Qt::ForegroundRole);
    } else if ( !isActive ) {
        item->setData(QBrush(Qt::green), Qt::ForegroundRole);
    }
}

void AbstractChannel::scrollToMaximum() {
    //if we don't read previous messages (by moving the scrollbar up)
    //we want to scroll down to see the new messages
    if ( scrollToMax ) {
        channelTextBrowser->verticalScrollBar()->setValue( channelTextBrowser->verticalScrollBar()->maximum() );
    }
}

void AbstractChannel::onScrollBarValueChanged(int value) {
    scrollToMax = value == channelTextBrowser->verticalScrollBar()->maximum();
}

QString AbstractChannel::makeHtml( QString in ) {
    QString ret = in;
    ret.replace( "\n", "<br/>" );
    return ret;//.prepend( "<p>" ).append( "</p>" );
}

/*
 2  \b  Ctrl+B  Bold (toggle)
15  \o  Ctrl+O  Normal (turn off bold, italic, underline, fixed pitch, reverse, and colors)
17  \f  Ctrl+F  Fixed pitch (toggle)
18  \r  Ctrl+R  Reverse foreground/background (toggle)
29  \s  Ctrl+N  Italic (toggle)
31  \u  Ctrl+U  Underline (toggle)
*/

QString AbstractChannel::processIRCCodes(QString in) {
    bool bold = false;
    bool fixed = false;
    bool italic = false;
    bool underline = false;
    bool color = false;
    QMap<int, QColor> colors;
    colors[0x0] = QColor::fromRgb(000,000,000);
    colors[0x1] = QColor::fromRgb(000,000,128);
    colors[0x2] = QColor::fromRgb(000,128,000);
    colors[0x3] = QColor::fromRgb(000,128,128);
    colors[0x4] = QColor::fromRgb(128,000,000);
    colors[0x5] = QColor::fromRgb(128,000,128);
    colors[0x6] = QColor::fromRgb(128,128,000);
    colors[0x7] = QColor::fromRgb(204,204,204);
    colors[0x8] = QColor::fromRgb(128,128,128);
    colors[0x9] = QColor::fromRgb(000,000,255);
    colors[0xA] = QColor::fromRgb(000,255,000);
    colors[0xB] = QColor::fromRgb(000,255,255);
    colors[0xC] = QColor::fromRgb(255,000,000);
    colors[0xD] = QColor::fromRgb(255,000,255);
    colors[0xE] = QColor::fromRgb(255,255,000);
    colors[0xF] = QColor::fromRgb(255,255,255);
    for (int i = 0; i < in.size(); i++) {
        if (in[i].toAscii() == 2) {
            if (!bold) {
                in.replace(i, 1, "<b>");
                bold = true;
            } else {
                in.replace(i, 1, "</b>");
                bold = false;
            }
        } else if (in[i].toAscii() == 17) {
            if (!fixed) {
                in.replace(i, 1, "<pre>");
                fixed = true;
            } else {
                in.replace(i, 1, "</pre>");
                fixed = false;
            }
        } else if (in[i].toAscii() == 29) {
            if (!italic) {
                in.replace(i, 1, "<i>");
                italic = true;
            } else {
                in.replace(i, 1, "</i>");
                italic = false;
            }
        } else if (in[i].toAscii() == 15) {
            QString r;
            if (bold) r+= "</b>";
            if (italic) r+= "</i>";
            if (underline) r+= "</u>";
            if (fixed) r+= "</pre>";
            in.replace(i, 1, "</b>");
            bold = false;
            italic = false;
            underline = false;
            fixed = false;
        } else if (in[i].toAscii() == 31) {
            if (!underline) {
                in.replace(i, 1, "<u>");
                underline = true;
            } else {
                in.replace(i, 1, "</u>");
                underline = false;
            }
        } else if (in[i].toAscii() == 3) {
            int numDigits = 0;
            if (i + 1 < in.length() - 1 && in[i+1].isDigit()) numDigits = 1;
            if (i + 2 < in.length() - 1 && in[i+2].isDigit()) numDigits = 2;
            if (numDigits == 0 && color) {
                in.replace(i, 1, "</span>");
                color = false;
            }
            QColor c = colors[in.mid(i+1, numDigits).toInt()];
            QString r;
            if (color) r = "</span>";
            r += QString("<span style=\"color: %1\">").arg(c.name());
            in.replace(i, 1+numDigits, r);
        }
    }
    return in;
}

QString AbstractChannel::urlify(QString in) {
    QRegExp urlPattern( "(http|ftp)s?://[^\n<>\\[\\]\" ]+" );
    QString replacePattern = "<a href=\"%1\">%1</a>";
    unsigned addedSize = replacePattern.size() - 4;
    int pos = 0;
    while (( pos = urlPattern.indexIn( in, pos ) ) != -1 ) {
        in.replace(pos, urlPattern.matchedLength(), replacePattern.arg( urlPattern.cap(0) ) );
        pos += urlPattern.matchedLength() + addedSize;
    }
    return in;
}

QString AbstractChannel::processBBCodes(QString in) {

    QRegExp bbcode("\\[([^\\]\\[]+)\\]");

    int idx = 0;
    while((idx = bbcode.indexIn(in, idx)) >= 0) {
        QString code = bbcode.capturedTexts()[1].toLower();
        int length = bbcode.capturedTexts()[0].length();
        QString rep;
        if(code.contains("=")) {
            QStringList tmp = code.split("=");
            if(tmp.size() != 2) continue;
            code = tmp.takeFirst();
            QString value = tmp.takeLast();
            if(code == "size") {
                int size = value.toInt();
                if(size < 5) size = 5;
                if(size > 20) size = 20;
                rep = QString("<span style=\"font-size:%1px;\">").arg(size);
            } else if(code == "color") {
                QColor c = QColor(value);
                if(c.value() > 150) c.setHsv(c.hue(), c.saturation(), 150);
                rep = QString("<span style=\"color: %1\">").arg(c.name());
            } else if(code == "url") {
                rep = value + " ";
            } else {
                rep = bbcode.capturedTexts()[0];
            }
        } else {
            if(code == "b") {
                rep = "<b>";
            } else if(code == "i") {
                rep = "<i>";
            } else if(code == "u") {
                rep = "<u>";
            } else if(code == "s") {
                rep = "<s>";
            } else if(code == "quote") {
                rep = "<blockquote>";
            } else if(code == "code") {
                rep = "<pre>";
            } else if(code == "/b") {
                rep = "</b>";
            } else if(code == "i") {
                rep = "</i>";
            } else if(code == "/u") {
                rep = "</u>";
            } else if(code == "/s") {
                rep = "</s>";
            } else if(code == "/quote") {
                rep = "</blockquote>";
            } else if(code == "/code") {
                rep = "</code>";
            } else if(code == "br") {
                rep = "<br/>";
            } else if(code == "url" || code == "/url") {
                rep = "";
            } else {
                rep = bbcode.capturedTexts()[0];
            }
        }
        in.replace(idx++, length, rep);
    }
    return urlify(in);
}

QString AbstractChannel::processInput(QString input, bool formatting) {
    input.replace( "<", "&lt;" ).replace( ">", "&gt;" );
    input = highlightUserName( input );
    if(formatting) {
        input.replace("\n","[br]");
        return processBBCodes(processIRCCodes(input));
    } else
        return urlify(input);
}

QString AbstractChannel::highlightUserName( QString input ) {
    if( !Settings::Instance()->value("Chat/highlightUserName").toBool() )
        return input;
    // highlight currentUsername
    QString username = ServerProfilesModel::getInstance()->getActiveProfile().userName();
    QString pattern = username;
    pattern.replace("[", "\\[").replace("]", "\\]");
    pattern += "(?!&gt;)";
    input.replace(QRegExp(pattern), "<font style=\" font-weight:600; color:#aa0000;\">"+username+"</font>");
    return input;
}

QString AbstractChannel::userNameLink( const QString userName ) {
    if( userName == ServerProfilesModel::getInstance()->getActiveProfile().userName() )
        return userName;
    QUrl url("qtlobby://query");
    url.addQueryItem("username",userName);
    return QString( "<a style=\"text-decoration:none; color:black;\" href=\"%1\">%2</a>" ).arg(url.toString()).arg(userName);
}

void AbstractChannel::anchorClicked(QUrl url) {
    if( url.scheme() == "qtlobby" ) {
        if( url.host() == "query" )
            emit sendInput( "/query " + url.queryItemValue("username") );
    } else {
        QDesktopServices::openUrl(url);
    }
}

void AbstractChannel::setChannelBrowser(ChannelTextBrowser* b) {
    channelTextBrowser = b;
    channelTextBrowser->setOpenLinks(false);
    channelTextBrowser->setReadOnly( true );
    QTextDocument * channelTextDocument = new QTextDocument( channelTextBrowser );
    channelTextBrowser->setDocument( channelTextDocument );
}

void AbstractChannel::historyMessage( QDateTime time, QString message ) {
    historyDateTime = time;
    if(!historyMode) inlineHistoryMode = true;
    receiveCommand(Command(message));
    inlineHistoryMode = false;
}

void AbstractChannel::setHistoryMode(bool b) {
    historyMode = b;
}

void AbstractChannel::requestHistoryReplay(QDate from, QDate to) {
    emit needReplay(from, to);
}

void AbstractChannel::setInlineHistoryMode(bool b) {
    inlineHistoryMode = b;
}

QString AbstractChannel::flag( const QString userName ) {
    if(historyMode || inlineHistoryMode) return QString();
    QString flag = "<img width=\"16\" height=\"16\" src=\""+P("flags/%1.xpm")+"\" />&nbsp;";
    if( Settings::Instance()->value("Chat/showFlags").toBool() && userNameCountryCodeMap )
        if ( userNameCountryCodeMap->contains( userName ) )
            return flag.arg( userNameCountryCodeMap->value( userName ) );
    return QString();
}


void AbstractChannel::onHistoryFinished() {
    if(!historyBuffer.size()) return;
    QTextCursor c = channelTextBrowser->textCursor();
    c.movePosition(QTextCursor::Start);
    firstBlock = true;
    foreach(QString s, historyBuffer) {
        insertBlock(c, firstBlock);
        c.insertHtml(s);
    }
    insertBlock(c, firstBlock);
    c.insertHtml("<b>-------------------</b><br/>");
    scrollToMax = true;
    scrollToMaximum();
}
