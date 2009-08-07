// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "AbstractChannel.h"

QString AbstractChannel::currentUsername;

AbstractChannel::AbstractChannel( QString name, QObject * parent ) : AbstractLobbyTab( parent ) {
    setObjectName( name );
    activeIcon = QIcon( ":/icons/channel.xpm" );
    inactiveIcon = QIcon( ":/icons/channel_unread.xpm" );
    activeTextColor = QColor("black");
    inactiveTextColor = QColor("green");
}

AbstractChannel::~AbstractChannel() {}

void AbstractChannel::setupUi( QWidget * channelTabWidget ) {
    //FIXME inherit from AbstractChannelWidget
    channelTabWidget->setObjectName( QString::fromUtf8( "channelTabWidget" ) + objectName() );
    channelTextBrowser = new ChannelTextBrowser( channelTabWidget );
    channelTextBrowser->setObjectName( "channelTextBrowser" + objectName() );
    channelTextBrowser->setOpenExternalLinks( true );
    channelTextBrowser->setReadOnly( true );
    QTextDocument * channelTextDocument = new QTextDocument( channelTextBrowser );
    //after exeeding the 500 blocks the first block will be removed
    channelTextDocument->setMaximumBlockCount( 500 );
    channelTextBrowser->setDocument( channelTextDocument );
    gridLayout = new QGridLayout( channelTabWidget );
    gridLayout->setObjectName( QString::fromUtf8( "channelGridLayout" ) + objectName() );
    gridLayout->addWidget( channelTextBrowser, 1, 0, 1, 1 );
//TODO move somewhere, where it works
//    connect(channelTextBrowser, SIGNAL(anchorClicked(QUrl)),
//            this, SLOT(anchorClicked(QUrl)));
}

void AbstractChannel::setActive( bool isActive ) {
    this->isActive = isActive;
    if ( isActive ) {
        icon = activeIcon;
        color = activeTextColor;
    }
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
    if ( QString( "/j,/join,/peter" ).split( "," ).contains( firstWord, Qt::CaseInsensitive ) ) {
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
        QString row( "<tr><td style=\"padding:0;border:none;\">%1</td><td style=\"padding:0;border:none;\">%2</td></tr>" );
        ret.attributes << objectName() << tr("Chat Help").append(" <table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">%1</table>" ).arg(
                        row.arg( "/help, /h", tr( "Display this help" ) ) +
                        row.arg( "/channels, /list", tr( "Display channel list" ) ) +
                        row.arg( "/join, /j &lt;channel&gt;", tr( "Join channel" ) ) +
                        row.arg( "/j &lt;channel&gt; &lt;password&gt;", tr( "Join password protected channel" ) ) +
                        row.arg( "/j &lt;channel1&gt; &lt;password1&gt;, &lt;channel2&gt; ...  ", tr( "Join multiple channels" ) ) +
                        row.arg( "/me &lt;text&gt;", tr( "Say highlighted" ) ) +
                        row.arg( "/slap &lt;user&gt;", tr( "Say the mIRC slap sentence" ) ) +
                        row.arg( "/query, /msg &lt;user&gt;", tr( "Open private chat" ) ) +
                        row.arg( "/leave, /wc", tr( "Leave the channel" ) ) );
        receiveCommand( ret );
        return true;
    } else if ( QString( "/ingame" ).split( "," ).contains( firstWord, Qt::CaseInsensitive ) ) {
        ret.name = "GETINGAMETIME";
        ret.attributes << inputList;
    } else
        return false;
    emit sendCommand( ret );
    return true;
}

void AbstractChannel::insertLine( QString line ) {
    QString pattern = currentUsername;
    pattern.replace("[", "\\[").replace("]", "\\]");
    pattern += "(?!&gt;)";
    line.replace(QRegExp(pattern), "<span style=\" font-weight:600; text-decoration: underline; color:#aa0000;\">"+currentUsername+"</span>");
    QString timeString = QString( "<span style=\"color:gray;\">%1</span> " ).arg( QTime().currentTime().toString( "[hh:mm:ss]" ) );
    bool scrollToMaximum = channelTextBrowser->verticalScrollBar()->value() == channelTextBrowser->verticalScrollBar()->maximum();
    QTextCursor c = channelTextBrowser->textCursor();
    //go to end of document
    c.movePosition( QTextCursor::End );
    c.insertHtml( makeHtml( timeString.append( line ) ) );
    //if we don't read previous messages (by moving the scrollbar up)
    //we want to scroll down to see the new messages
    if ( scrollToMaximum ) {
        channelTextBrowser->verticalScrollBar()->setValue( channelTextBrowser->verticalScrollBar()->maximum() );
    }
    if ( !isActive ) {
        icon = inactiveIcon;
        color = inactiveTextColor;
    }
}

QString AbstractChannel::makeHtml( QString in ) {
    QString ret = in;
    ret.replace( "\n", "<br>" );
    return ret.prepend( "<p>" ).append( "</p>" );
}

void AbstractChannel::setCurrentUsername(QString user) {
    currentUsername = user;
}

/*
 2  	\b  	Ctrl+B  Bold (toggle)
15 	\o 	Ctrl+O 	Normal (turn off bold, italic, underline, fixed pitch, reverse, and colors)
17 	\f 	Ctrl+F 	Fixed pitch (toggle)
18 	\r 	Ctrl+R 	Reverse foreground/background (toggle)
29 	\s 	Ctrl+N 	Italic (toggle)
31 	\u 	Ctrl+U 	Underline (toggle)
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
    //   QRegExp urlPattern( "(http|ftp)s?://[^/?# ]+[^?# ]*(\?[^# ]*)?(#[^#? ]*)?" );
    QRegExp urlPattern( "(http|ftp)s?://[^\n<>\\[\\]\" ]+" );
    QStringList ct;
    int pos = 0;
    while (( pos = urlPattern.indexIn( in, pos ) ) != -1 ) {
        pos += urlPattern.matchedLength();
        ct << urlPattern.cap( 0 );
    }
    int count = ct.count();
    QString placeHolder = "__###PLACEHOLDER:%1###__";
    QString linkTag = "<a href=\"%1\">%1</a>";
    for ( int i = 0; i < count; ++i )
        in.replace( ct[i], placeHolder.arg( i ) );
    for ( int i = 0; i < count; ++i )
        in.replace( placeHolder.arg( i ), linkTag.arg( ct[i] ) );
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


QString AbstractChannel::processInput(QString input, bool fomatting) {
    input.replace( "<", "&lt;" ).replace( ">", "&gt;" );
    if(fomatting) {
        input.replace("\n","[br]");
        return processBBCodes(processIRCCodes(input));
    }
    else return urlify(input);
}
