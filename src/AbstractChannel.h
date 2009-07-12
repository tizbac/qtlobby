//
// C++ Interface: AbstractChannel
//
// Description:
//
//
// Author: Mirko Windhoff <qtlobby.reg@ncnever.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ABSTRACTCHANNEL_H
#define ABSTRACTCHANNEL_H

#include <QTextBrowser>
#include <QScrollBar>
#include <QTime>
#include <QGridLayout>
#include "Command.h"
#include "AbstractLobbyTab.h"
#include "ui_abstractChannelWidget.h"
#include "ChannelTextBrowser.h"

/**
 @author Mirko Windhoff <qtlobby.reg@ncnever.de>
*/
class AbstractChannel : public AbstractLobbyTab/*, public Ui_abstractChannelWidget*/ {
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
  QString processInput(QString input);
  QString processIRCCodes(QString in);
  ChannelTextBrowser * channelTextBrowser;
  QGridLayout * gridLayout;
  QIcon activeIcon;
  QIcon inactiveIcon;
  QColor activeTextColor;
  QColor inactiveTextColor;
  static QString currentUsername;
};

#endif
