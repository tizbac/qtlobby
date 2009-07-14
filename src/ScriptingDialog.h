#ifndef SCRIPTINGDIALOG_H
#define SCRIPTINGDIALOG_H

#include <QtGui/QDialog>
#include <PythonQt.h>

namespace Ui {
    class ScriptingDialog;
}

class ScriptingDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(ScriptingDialog)
public:
    explicit ScriptingDialog(PythonQtObjectPtr main, QWidget *parent = 0);
    virtual ~ScriptingDialog();


private slots:
    void onPythonStdOut(const QString &str);
    void onPythonStdErr(const QString &str);
protected:
    virtual void changeEvent(QEvent *e);
    virtual void hideEvent(QHideEvent* event);

private:
    Ui::ScriptingDialog *m_ui;
    PythonQtObjectPtr mainModule;

private slots:
    void on_executePushButton_clicked();
};

#endif // SCRIPTINGDIALOG_H
