// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "MapOverviewDialog.h"

MapOverviewDialog::MapOverviewDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::MapOverviewDialog) {
    m_ui->setupUi(this);
}

MapOverviewDialog::~MapOverviewDialog() {
    delete m_ui;
}

void MapOverviewDialog::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MapOverviewDialog::setSource(QString mapName, QString mapDescription, QImage minimap, QImage metalmap, RawHeightMap heightmap) {
    m_ui->glWidget->setSource(mapName, minimap, metalmap, heightmap);
    setWindowTitle(mapName);
    m_ui->mapDescriptionLabel->setText(mapDescription);
}
