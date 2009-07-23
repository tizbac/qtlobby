// $Id$
// QtLobby released under the GPLv3, see COPYING for details.

#ifndef QTLOBBYPREFERENCE_H
#define QTLOBBYPREFERENCE_H

#include <QDialog>
#include <QScrollArea>
#include <QDebug>
#include <QDir>
#include <QList>

#include "ui_preferenceWidget.h"
#include "Settings.h"
#include "ConfigElement.h"

class UserPreference : public QDialog, private Ui::PreferenceWidget {
    Q_OBJECT
public:
    UserPreference( QDialog* parent = 0 );
    ~UserPreference();
private:
    QSettings* settings;
    QList<ConfigElement*> elements;
private slots:
    void okClicked();
    void applyClicked();
    void cancelClicked();
};

#endif

