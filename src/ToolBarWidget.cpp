#include "ToolBarWidget.h"

ToolBarWidget::ToolBarWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolBarWidget) {
    ui->setupUi(this);
    ui->tabBar->setTabsClosable(true);
}

ToolBarWidget::~ToolBarWidget() {
    delete ui;
}

void ToolBarWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ToolBarWidget::onNextTab() {
    ui->tabBar->setCurrentIndex((ui->tabBar->currentIndex() + 1) % ui->tabBar->count());
}

void ToolBarWidget::onPreviousTab() {
    ui->tabBar->setCurrentIndex((ui->tabBar->currentIndex() - 1 + ui->tabBar->count()) % ui->tabBar->count());
}

void ToolBarWidget::onCloseTab() {
    ui->tabBar->removeTab(ui->tabBar->currentIndex());
}
