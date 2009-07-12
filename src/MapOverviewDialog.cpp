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

void MapOverviewDialog::setSource(QString mapName, QString mapDescription, QImage minimap, RawHeightMap heightmap) {
    m_ui->glWidget->setSource(mapName, minimap, heightmap);
    m_ui->mapNameLabel->setText("<b>"+mapName+"</b");
    m_ui->mapDescriptionLabel->setText(mapDescription);
}

void MapOverviewDialog::addStartRect(int ally, QRect r) {
    m_ui->glWidget->addStartRect(ally, r);
}

void MapOverviewDialog::setMyAllyTeam(int n) {
    m_ui->glWidget->setMyAllyTeam(n);
}

void MapOverviewDialog::removeStartRect(int ally) {
    m_ui->glWidget->removeStartRect(ally);
}
