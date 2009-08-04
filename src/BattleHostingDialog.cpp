// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "BattleHostingDialog.h"
#include "ui_BattleHostingDialog.h"
#include "UnitSyncLib.h"

BattleHostingDialog::BattleHostingDialog(QProcess* spring, CommandAssigner* assigner, LobbyTabs* lobbyTabs, QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::BattleHostingDialog) {
    m_ui->setupUi(this);
    QSettings *settings = Settings::Instance();
    if( settings->contains("BattleHost/Description") )
        m_ui->descriptionLineEdit->setText(settings->value("BattleHost/Description").toString());
    m_assigner = assigner;
    m_tabs = lobbyTabs;
    m_spring = spring;
    m_battleHost = 0;
    connect(UnitSyncLib::getInstance(), SIGNAL(rebooted()), SLOT(onReboot()));
    onReboot();
}

BattleHostingDialog::~BattleHostingDialog() {
    delete m_ui;
}

void BattleHostingDialog::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void BattleHostingDialog::on_buttonBox_accepted() {
    if(m_battleHost) {
        QMessageBox::critical(this, "Error", "You have a battle running!\nClose it first!");
        return;
    }
    m_battleHost = new BattleHost(m_currentUsername, this);
    QString passwd = m_ui->passwordLineEdit->text();
    m_battleHost->setHostingParams(0,
                                   0,
                                   passwd.isEmpty() ? "*" : passwd,
                                   m_ui->portSpinBox->value(),
                                   m_ui->maxPlayersSpinBox->value(),
                                   m_ui->rankComboBox->currentIndex(),
                                   m_ui->mapComboBox->currentText(),
                                   m_ui->modComboBox->currentText(),
                                   m_ui->descriptionLineEdit->text()
                                   );
    QSettings * settings = Settings::Instance();
    settings->setValue("BattleHost/Description", m_ui->descriptionLineEdit->text());
    connect( m_assigner, SIGNAL( battleHostCommand( Command ) ),
             m_battleHost, SLOT( receiveCommand( Command ) ) );
    connect( m_battleHost, SIGNAL( sendCommand( Command ) ),
             m_assigner, SLOT( sendCommand( Command ) ) );
    connect( m_battleHost, SIGNAL(sendFakeMessage(QString)),
             m_assigner, SLOT( receiveMessage(QString)));
    connect( m_battleHost, SIGNAL(hosted(int)),
             m_tabs, SLOT(onBattleHosted(int)));
    connect( m_battleHost, SIGNAL(closed()),
             this, SLOT(onClosed()));
    connect(m_spring, SIGNAL(finished (int, QProcess::ExitStatus)),
            m_battleHost, SIGNAL(stopped()));
    m_battleHost->start();
    accept();
}

void BattleHostingDialog::onClosed() {
    m_battleHost->deleteLater();
    m_battleHost = 0;
}

void BattleHostingDialog::setCurrentUsername(QString username) {
    m_currentUsername = username;
}

void BattleHostingDialog::onReboot() {
    UnitSyncLib* unitSyncLib = UnitSyncLib::getInstance();
    m_ui->modComboBox->clear();
    m_ui->modComboBox->addItems(unitSyncLib->getModNames());
    m_ui->mapComboBox->clear();
    m_ui->mapComboBox->addItems(unitSyncLib->getMapNames());
}
