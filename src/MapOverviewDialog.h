#ifndef MAPOVERVIEWDIALOG_H
#define MAPOVERVIEWDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class MapOverviewDialog;
}

class MapOverviewDialog : public QDialog {
    Q_OBJECT
public:
    explicit MapOverviewDialog(QWidget *parent = 0);
    virtual ~MapOverviewDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::MapOverviewDialog *m_ui;
};

#endif // MAPOVERVIEWDIALOG_H
