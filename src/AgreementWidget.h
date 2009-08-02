// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef AGREEMENTWIDGET_H
#define AGREEMENTWIDGET_H

#include <QDialog>
#include "ui_AgreementWidget.h"

/**
        @author Joachim Schiele <js@lastlog.de>
*/
class AgreementWidget : public QDialog, public Ui::AgreementWidget {
    Q_OBJECT
public:
    AgreementWidget(QWidget *parent = 0);
    ~AgreementWidget();
};

#endif
