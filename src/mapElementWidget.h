//
// C++ Interface: mapElementWidget
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
