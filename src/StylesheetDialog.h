// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef STYLESHEETDIALOG_H
#define STYLESHEETDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class StylesheetDialog;
}

class StylesheetDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(StylesheetDialog)
        public:
            explicit StylesheetDialog(QWidget *parent = 0);
    virtual ~StylesheetDialog();

private slots:
    void save();
    void load();
    void apply();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::StylesheetDialog *m_ui;
};

#endif // STYLESHEETDIALOG_H
