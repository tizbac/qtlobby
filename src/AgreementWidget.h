//
// C++ Interface: AgreementWidget
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AGREEMENTWIDGET_H
#define AGREEMENTWIDGET_H

#include <QDialog>
#include "ui_agreementWidget.h"

/**
	@author Joachim Schiele <js@lastlog.de>
*/
class AgreementWidget : public QDialog, public Ui::agreementWidget{
Q_OBJECT
public:
    AgreementWidget(QWidget *parent = 0);
    ~AgreementWidget();
};

#endif
