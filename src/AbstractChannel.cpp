//
// C++ Implementation: AbstractChannel
//
// Description:
//
//
// Author: Mirko Windhoff <qtlobby.reg@ncnever.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "AbstractChannel.h"

AbstractChannel::AbstractChannel( QString name, QObject * parent ) : AbstractLobbyTab( parent ) {
  setObjectName( name );
  activeIcon = QIcon( ":/icons/channel.xpm" );
  inactiveIcon = QIcon( ":/icons/channel_unread.xpm" );
}

AbstractChannel::~AbstractChannel() {}

void AbstractChannel::setupUi( QWidget * channelTabWidget ) {
  //FIXME inherit from abstractChannelWidget
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
}

void AbstractChannel::setActive( bool isActive ) {
  this->isActive = isActive;
  if ( isActive )
    icon = activeIcon;
}

bool AbstractChannel::executeChannelInput( QString input ) {
  if ( input.contains( "\n" ) ) {
    QStringList lines = input.split( "\n" );
    foreach( QString l, lines )
    receiveInput( l );
    return true;
  }
  //input.replace( "<", "&lt;" );
  //input.replace( ">", "&gt;" );
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
  }
  else if ( QString( "/channels,/list" ).split( "," ).contains( firstWord, Qt::CaseInsensitive ) ) {
    ret.name = "CHANNELS";
  }
  else if ( QString( "/msg,/query" ).split( "," ).contains( firstWord, Qt::CaseInsensitive ) ) {
    ret.name = "SAYPRIVATE";
    ret.attributes << inputList;
  }
  else if ( QString( "/h,/help" ).split( "," ).contains( firstWord, Qt::CaseInsensitive ) ) {
    ret.name = "CLIENTMSG";
    QString row( "<tr><td style=\"padding:0;border:none;\">%1</td><td style=\"padding:0;border:none;\">%2</td></tr>" );
    ret.attributes << objectName()
    << QString( "Chat Help <table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">%1</table>" ).arg(
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
  }
  else
    return false;
  emit sendCommand( ret );
  return true;
}

void AbstractChannel::insertLine( QString line ) {
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
  if ( !isActive )
    icon = inactiveIcon;
}

QString AbstractChannel::makeHtml( QString in ) {
  QString ret = in;
//   QRegExp urlPattern( "(http|ftp)s?://[^/?# ]+[^?# ]*(\?[^# ]*)?(#[^#? ]*)?" );
  QRegExp urlPattern( "(http|ftp)s?://[^\n<> ]*" );
  QStringList ct;
  int pos = 0;
  while (( pos = urlPattern.indexIn( ret, pos ) ) != -1 ) {
    pos += urlPattern.matchedLength();
    ct << urlPattern.cap( 0 );
  }
  int count = ct.count();
  QString placeHolder = "__###PLACEHOLDER:%1###__";
  QString linkTag = "<a href=\"%1\">%2</a>";
  for ( int i = 0; i < count; ++i )
    ret.replace( ct[i], placeHolder.arg( i ) );
  for ( int i = 0; i < count; ++i )
    ret.replace( placeHolder.arg( i ), linkTag.arg( ct[i], ct[i] ) );
  ret.replace( "\n", "<br>" );
  return ret.prepend( "<p>" ).append( "</p>" );
}
