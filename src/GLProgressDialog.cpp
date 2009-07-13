#include "src/GLProgressDialog.h"
#include "ui_GLProgressDialog.h"

GLProgressDialog::GLProgressDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::GLProgressDialog) {
    m_ui->setupUi(this);
}

GLProgressDialog::~GLProgressDialog() {
    delete m_ui;
}

void GLProgressDialog::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void GLProgressDialog::updateProgress(int percent) {
    m_ui->progressBar->setValue(percent);
}

void GLProgressDialog::setMax(int max) {
    m_ui->progressBar->setMaximum(max);
}
