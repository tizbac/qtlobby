// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "StylesheetDialog.h"
#include "ui_StylesheetDialog.h"
#include <Qsci/qscilexercss.h>

StylesheetDialog::StylesheetDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::StylesheetDialog) {
    m_ui->setupUi(this);
    QsciLexerCSS* lexer = new QsciLexerCSS(this);
    m_ui->styleSheetTextEdit->setLexer(lexer);
}

StylesheetDialog::~StylesheetDialog() {
    delete m_ui;
}

void StylesheetDialog::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void StylesheetDialog::save() {

}

void StylesheetDialog::load() {

}

void StylesheetDialog::apply() {
    qApp->setStyleSheet(m_ui->styleSheetTextEdit->text());
}

