#ifndef MAPOVERVIEWDIALOG_H
#define MAPOVERVIEWDIALOG_H

#include <QtGui/QDialog>
#include "RawHeightMap.h"

namespace Ui {
    class MapOverviewDialog;
}

class MapOverviewDialog : public QDialog {
    Q_OBJECT
public:
    explicit MapOverviewDialog(QWidget *parent = 0);
    virtual ~MapOverviewDialog();
    void setSource(QString mapName, QString description, QImage minimap, RawHeightMap heightmap);

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::MapOverviewDialog *m_ui;
};

#endif // MAPOVERVIEWDIALOG_H
