#include "src/ScriptingDialog.h"
#include "ui_ScriptingDialog.h"
#include <Qsci/qscilexerjavascript.h>
#include "Settings.h"

ScriptingDialog::ScriptingDialog(QScriptEngine* engine, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ScriptingDialog)
{
    m_ui->setupUi(this);
    QsciLexerJavaScript* lexer = new QsciLexerJavaScript(this);
    m_ui->scriptTextEdit->setLexer(lexer);
    this->engine = engine;
    m_ui->splitter->restoreState(Settings::Instance()->value("scriptingSplitterState").toByteArray());
}

ScriptingDialog::~ScriptingDialog()
{
    delete m_ui;
}

void ScriptingDialog::changeEvent(QEvent *e)
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

void ScriptingDialog::on_executePushButton_clicked() {
    engine->evaluate(m_ui->scriptTextEdit->text());
}

/*void ScriptingDialog::onPythonStdOut(const QString &str) {
    m_ui->outputTextBrowser->insertPlainText(str);
}

void ScriptingDialog::onPythonStdErr(const QString &str) {
    m_ui->outputTextBrowser->insertPlainText(str);
}*/

void ScriptingDialog::hideEvent(QHideEvent* /*event*/) {
    Settings::Instance()->setValue("scriptingSplitterState", m_ui->splitter->saveState());
}
