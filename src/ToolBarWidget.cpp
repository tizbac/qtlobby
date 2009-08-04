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
