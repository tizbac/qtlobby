// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef MAPELEMENTWIDGET_H
#define MAPELEMENTWIDGET_H

#include <QWidget>

#include "ui_mapSelectorWidget.h"

/**
	@author Joachim Schiele <js@lastlog.de>
*/
class mapElementWidget : public QWidget, private Ui::MapSelectorWidget {
    Q_OBJECT
public:
    mapElementWidget(QWidget *parent = 0);
    ~mapElementWidget();

};

#endif
