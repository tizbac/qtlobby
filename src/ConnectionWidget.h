//
// C++ Interface: ConnectionWidget
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CONNECTIONWIDGET_H
#define CONNECTIONWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QVariant>
#include <QStringList>
#include <QUrl>
#include <QMessageBox>
#include <QWizard>
#include <QSound>

#include <QProgressDialog>
#include <QTimer>

#include "ui_connectionWidget.h"
#include "ServerContextState.h"
#include "Settings.h"

/**
 @author Joachim Schiele <js@lastlog.de>
*/
class ConnectionWidget : public QDialog, public Ui::connectionWidget {
  Q_OBJECT
public:
  ConnectionWidget( ServerContextState*, QWidget *parent = 0 );
  ~ConnectionWidget();
signals:
  void emitConfiguration( QUrl );
  void establishConnection_();
  void usernameChanged(QString username);
public slots:
  void show_if_wanted();
  void modifyServerProfile(signed int index, QUrl url);
  void logWrite( QString );
  void toggleAutoLogin();
  void toggleRememberPassword();
  void establishConnection();
  void addDefaultServers();
  void comboBoxCurrentIndexChanged(int index);
  void updateComboBoxes();
  void saveModifiedProfile();
  void createNewProfile();
  void delSelectedProfile();
  void registerNewAccount();
  void renameLoginNameFeedbackSuccess(QString);
  void renameLoginNameFeedbackFailure(QString);
  void changePasswordSuccess( QString );
  void changePasswordFailure( QString );
  void onLogin();
  void updateCountdown();
  void establishSimpleConnection();
  void simpleViewChanged();
private slots:
  void connectionStatusChanged(ConnectionState state);
  void renameLoginName();
  void changePassword();
private:
  QTimer *countdownTimer;
  QProgressDialog *countdownDialog;
  bool connected;
  int countdown;
  QSettings* settings;
  ServerContextState* serverContextState;
  void lockInterface();
  void unlockInterface();
  void unlockRenameAndChangePassword();
  void lockRenameAndChangePassword();
};

#endif
