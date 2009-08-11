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
    void chooseColor();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::StylesheetDialog *m_ui;
    static QString m_stylesheet;
};

#endif // STYLESHEETDIALOG_H
