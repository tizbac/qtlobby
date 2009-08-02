// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef MAPELEMENTWIDGET_H
#define MAPELEMENTWIDGET_H

#include <QWidget>

#include "ui_MapSelectorWidget.h"

/**
	@author Joachim Schiele <js@lastlog.de>
*/
class MapElementWidget : public QWidget, private Ui::MapSelectorWidget {
    Q_OBJECT
public:
    MapElementWidget(QWidget *parent = 0);
    ~MapElementWidget();

};

#endif
