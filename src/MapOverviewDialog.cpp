#include "src/MapOverviewDialog.h"
#include "ui_MapOverviewDialog.h"

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

void MapOverviewDialog::setSource(QImage minimap, RawHeightMap heightmap) {
    m_ui->glWidget->setSource(minimap, heightmap);
}
