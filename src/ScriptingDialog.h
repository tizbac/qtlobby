// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef SCRIPTINGDIALOG_H
#define SCRIPTINGDIALOG_H

#include <QtGui/QDialog>
#include <QScriptEngine>

namespace Ui {
    class ScriptingDialog;
}

class ScriptingDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(ScriptingDialog)
public:
    explicit ScriptingDialog(QScriptEngine* engine, QWidget *parent = 0);
    virtual ~ScriptingDialog();


private slots:
    //void onPythonStdOut(const QString &str);
    //void onPythonStdErr(const QString &str);
protected:
    virtual void changeEvent(QEvent *e);
    virtual void hideEvent(QHideEvent* event);

private:
    Ui::ScriptingDialog *m_ui;
    QScriptEngine* engine;

private slots:
    void on_executePushButton_clicked();
};

#endif // SCRIPTINGDIALOG_H
