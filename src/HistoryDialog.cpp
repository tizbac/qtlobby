#include "HistoryDialog.h"
#include "ui_HistoryDialog.h"
#include "Channel.h"
#include "PrivateChannel.h"
#include "History.h"

HistoryDialog::HistoryDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::HistoryDialog) {
    m_ui->setupUi(this);
    m_ui->textBrowser->setContextMenuPolicy(Qt::DefaultContextMenu);
    m_ui->fromDateEdit->setDate(QDate::currentDate());
    m_ui->toDateEdit->setDate(QDate::currentDate().addDays(1));
    m_channel = 0;
    connect(this, SIGNAL(needReplay(QDate,QDate)), History::getInstance(), SLOT(play(QDate,QDate)));
}

HistoryDialog::~HistoryDialog() {
    delete m_ui;
}

void HistoryDialog::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void HistoryDialog::replayHistory() {
    QString type = m_ui->typeComboBox->currentText();
    if(m_channel) delete m_channel;
    if(type == "Channel") {
        m_channel = new Channel(m_ui->nameLineEdit->text(), this);
    } else if(type == "Private") {
        m_channel = new PrivateChannel(m_ui->nameLineEdit->text(), this);
    }
    m_channel->setChannelBrowser(m_ui->textBrowser);
    m_channel->setHistoryMode(true);
    connect(History::getInstance(), SIGNAL(historyMessage(QDateTime, QString)),
            m_channel, SLOT(historyMessage(QDateTime,QString)));
    emit needReplay(m_ui->fromDateEdit->date(), m_ui->toDateEdit->date());
}

void HistoryDialog::on_typeComboBox_activated(QString type) {
    replayHistory();
}

void HistoryDialog::on_nameLineEdit_editingFinished(){
    replayHistory();
}

void HistoryDialog::on_fromDateEdit_editingFinished() {
    replayHistory();
}

void HistoryDialog::on_toDateEdit_editingFinished() {
    replayHistory();
}
