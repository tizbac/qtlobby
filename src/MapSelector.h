// $Id$
// QtLobby released under the GPLv3, see COPYING for details.

#ifndef MapSelector_H
#define MapSelector_H

#include <QDialog>
#include <QGridLayout>
#include <QScrollArea>

#include "mapElementWidget.h"

class MapSelector : public QDialog {
    Q_OBJECT

public:
    MapSelector(QDialog* parent = 0 );
    ~MapSelector();
    QScrollArea* scrollArea;
public slots:
    /*$PUBLIC_SLOTS$*/

protected:
    /*$PROTECTED_FUNCTIONS$*/

protected slots:
    /*$PROTECTED_SLOTS$*/

};

#endif

