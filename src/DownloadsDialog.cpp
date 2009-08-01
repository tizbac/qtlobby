// $Id: InfoChannel.cpp 229 2009-07-23 02:18:19Z mirko.windhoff $
// QtLobby released under the GPLv3, see COPYING for details.
#include "src/DownloadsDialog.h"
#include "ui_DownloadsDialog.h"
#include "DownloadsModel.h"
#include "ProgressBarItemDelegate.h"

DownloadsDialog::DownloadsDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::DownloadsDialog) {
    m_ui->setupUi(this);
    m_ui->downloadsTableView->setItemDelegateForColumn(2, new ProgressBarItemDelegate());
    m_ui->downloadsTableView->setModel(DownloadsModel::getInstance());
}

DownloadsDialog::~DownloadsDialog() {
    delete m_ui;
}

void DownloadsDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
