// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
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
#include <QKeyEvent>

#include <QProgressDialog>
#include <QTimer>

#include "ui_ConnectionWidget.h"
#include "ServerContextState.h"
#include "Settings.h"
#include "ServerProfilesModel.h"

/**
 * Handles connections to the Server. Manages the profiles.
 */
class ConnectionWidget : public QDialog, public Ui::ConnectionWidget {
    Q_OBJECT
public:
    ConnectionWidget( ServerContextState*, QWidget *parent = 0 );
    ~ConnectionWidget();
signals:
    void establishConnection_();
public slots:
    void show_if_wanted();
    void logWrite( QString );
    void toggleAutoLogin(bool checked);
    void toggleRememberPassword(bool checked);
    void establishConnection();
//    void addDefaultServers();
    void comboBoxCurrentIndexChanged(QModelIndex current, QModelIndex previous);
    void createNewProfile();
    void delSelectedProfile();
    void registerNewAccount();
    void renameLoginNameFeedbackSuccess(QString);
    void renameLoginNameFeedbackFailure(QString);
    void registrationSuccessFeedback(QString);
    void registrationFailFeedback(QString);
    void changePasswordSuccess( QString );
    void changePasswordFailure( QString );
    void onLogin();
    void establishSimpleConnection();
    void simpleViewChanged();
    void saveModifiedProfile();
protected:
    void keyPressEvent(QKeyEvent* event);
    void hideEvent(QHideEvent* event);
private slots:
    void on_simpleAutologinChechbox_toggled(bool checked);
    void on_profilePortSpinBox_valueChanged(int );
    void on_profileServerAddressLineEdit_textChanged(QString );
    void on_passwordLineEdit_textChanged(QString );
    void on_profileUserNameLineEdit_textChanged(QString );
    void connectionStatusChanged(ConnectionState state);
    void renameLoginName();
    void changePassword();
    void serverContextStateLoopback(QString message);
private:
    QTimer *countdownTimer;
    QProgressDialog *countdownDialog;
    bool connected;
    int countdown;
    QSettings* settings;
    ServerContextState* serverContextState;
    bool showSimple;
    bool simpleAutologin;
    QUrl simpleUrl;
    QUrl configuration;
    ServerContextState* registrationServerContextState;

    void lockInterface();
    void unlockInterface();
    void unlockRenameAndChangePassword();
    void lockRenameAndChangePassword();
};

#endif
