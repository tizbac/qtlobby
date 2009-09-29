// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "DownloadsDialog.h"
#include "ui_DownloadsDialog.h"
#include "DownloadsModel.h"
#include "ProgressBarItemDelegate.h"
#include "PathManager.h"
#include <QDateTime>

DownloadsDialog::DownloadsDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::DownloadsDialog) {
    m_ui->setupUi(this);
    m_ui->downloadsTableView->setItemDelegateForColumn(2, new ProgressBarItemDelegate());
    m_ui->downloadsTableView->setModel(DownloadsModel::getInstance());
    qsrand(QDateTime::currentDateTime().toTime_t());
}

DownloadsDialog::~DownloadsDialog() {
    delete m_ui;
}

void DownloadsDialog::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DownloadsDialog::showEvent(QShowEvent* /*event*/) {
    QString file(P("icons/logos/jobjol/logo%1.xpm"));
    int idx = qrand() % 16;
    m_ui->jobjolLogoLabel->setPixmap(QPixmap(file.arg(idx)));
}

void DownloadsDialog::on_clearPushButton_clicked() {
    DownloadsModel::getInstance()->clearFinished();
}
