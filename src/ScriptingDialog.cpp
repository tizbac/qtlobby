#include "src/ScriptingDialog.h"
#include "ui_ScriptingDialog.h"
#include <Qsci/qscilexerpython.h>
#include "Settings.h"

ScriptingDialog::ScriptingDialog(PythonQtObjectPtr main, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ScriptingDialog)
{
    m_ui->setupUi(this);
    QsciLexerPython* lexer = new QsciLexerPython(this);
    m_ui->scriptTextEdit->setLexer(lexer);
    mainModule = main;
    connect(PythonQt::self(), SIGNAL(pythonStdOut(QString)), this, SLOT(onPythonStdOut(QString)));
    connect(PythonQt::self(), SIGNAL(pythonStdErr(QString)), this, SLOT(onPythonStdErr(QString)));
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
    mainModule.evalScript(m_ui->scriptTextEdit->text());
}

void ScriptingDialog::onPythonStdOut(const QString &str) {
    m_ui->outputTextBrowser->insertPlainText(str);
}

void ScriptingDialog::onPythonStdErr(const QString &str) {
    m_ui->outputTextBrowser->insertPlainText(str);
}

void ScriptingDialog::hideEvent(QHideEvent* event) {
    Settings::Instance()->setValue("scriptingSplitterState", m_ui->splitter->saveState());
}
