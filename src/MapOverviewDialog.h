// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef MAPOVERVIEWDIALOG_H
#define MAPOVERVIEWDIALOG_H

#include <QtGui/QDialog>
#include "RawHeightMap.h"
#include "ui_MapOverviewDialog.h"

namespace Ui {
    class MapOverviewDialog;
}

class MapOverviewDialog : public QDialog {
    Q_OBJECT
public:
    explicit MapOverviewDialog(QWidget *parent = 0);
    virtual ~MapOverviewDialog();
    void setSource(QString mapName, QString description, QImage minimap, QImage metalmap, RawHeightMap heightmap);
    void setDrawStartPositions(bool b);
public slots:
    void addStartRect(int ally, QRect r);
    void setMyAllyTeam(int n);
    void removeStartRect(int ally);

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::MapOverviewDialog *m_ui;
};

inline void MapOverviewDialog::addStartRect(int ally, QRect r) {
    m_ui->glWidget->addStartRect(ally, r);
}

inline void MapOverviewDialog::setMyAllyTeam(int n) {
    m_ui->glWidget->setMyAllyTeam(n);
}

inline void MapOverviewDialog::removeStartRect(int ally) {
    m_ui->glWidget->removeStartRect(ally);
}

inline void MapOverviewDialog::setDrawStartPositions(bool b) {
    m_ui->glWidget->setDrawStartPositions(b);
}

#endif // MAPOVERVIEWDIALOG_H
