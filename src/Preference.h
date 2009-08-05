// $Id$
// QtLobby released under the GPLv3, see COPYING for details.

#ifndef QTLOBBYPREFERENCE_H
#define QTLOBBYPREFERENCE_H

#include <QDialog>
#include <QScrollArea>
#include <QDebug>
#include <QDir>
#include <QList>
#include <QStringListModel>
#include <QTranslator>

#include "ui_PreferenceWidget.h"
#include "Settings.h"
#include "PreferencePathElement.h"

class UserPreference : public QDialog, private Ui::PreferenceWidget {
    Q_OBJECT
public:
    UserPreference( QDialog* parent = 0 );
    ~UserPreference();
private:
    QSettings* settings;
    QList<PreferencePathElement*> pathElements;

    QVector<QStringList> getPathElements();
private slots:
    void languageChanged(QString language);
    void okClicked();
    void applyClicked();
    void cancelClicked();
};

#endif
