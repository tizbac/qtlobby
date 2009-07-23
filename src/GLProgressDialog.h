// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef GLPROGRESSDIALOG_H
#define GLPROGRESSDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
class GLProgressDialog;
}

class GLProgressDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(GLProgressDialog)
public:
    explicit GLProgressDialog(QWidget *parent = 0);
    virtual ~GLProgressDialog();

public slots:
    void setMax(int max);
    void updateProgress(int percent);

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::GLProgressDialog *m_ui;
};

#endif // GLPROGRESSDIALOG_H
