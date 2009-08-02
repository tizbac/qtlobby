// $Id$
// QtLobby released under the GPLv3, see COPYING for details.

#ifndef CONFIGELEMENT_H
#define CONFIGELEMENT_H

#include <QWidget>
#include <QStringList>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>

#include "Settings.h"
#include "ui_PreferencePathElement.h"

class PreferencePathElement : public QWidget, public Ui::PreferencePathElement {
    Q_OBJECT
public:
    PreferencePathElement( QStringList config, QWidget* parent = 0 );
    ~PreferencePathElement();

protected:
    QStringList m_config;
    QSettings* settings;
    bool exists;
    bool isDirectory;
    QStringList examples;

public slots:
    void SaveElement();
    void ResetConfiguration();
protected slots:
    void openFileBrowser();
    void updateExistingState( QString a = "" );
};

#endif

