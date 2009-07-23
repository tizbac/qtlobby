// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef BATTLEHOSTINGDIALOG_H
#define BATTLEHOSTINGDIALOG_H

#include <QtGui/QDialog>
#include <QProcess>
#include "BattleHost.h"
#include "CommandAssigner.h"
#include "LobbyTabs.h"

namespace Ui {
    class BattleHostingDialog;
}

class BattleHostingDialog : public QDialog {
    Q_OBJECT
public:
    explicit BattleHostingDialog(QProcess* spring, CommandAssigner* assigner, LobbyTabs* lobbyTabs, QWidget *parent = 0);
    virtual ~BattleHostingDialog();
    void setCurrentUsername(QString username);

protected:
    virtual void changeEvent(QEvent *e);

private slots:
    void on_buttonBox_accepted();
    void onClosed();
    void onReboot();

private:
    Ui::BattleHostingDialog *m_ui;
    BattleHost* m_battleHost;
    CommandAssigner* m_assigner;
    LobbyTabs* m_tabs;
    QString m_currentUsername;
    QProcess* m_spring;
};

#endif // BATTLEHOSTINGDIALOG_H
