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

class Preference : public QDialog, private Ui::PreferenceWidget {
    Q_OBJECT
public:
    Preference( QDialog* parent = 0 );
    ~Preference();
signals:
    void preferencesChanged();
private:
    QSettings* settings;
    QList<PreferencePathElement*> pathElements;
    QStringList springSystemDirConfig;
    QStringList springExecutableFileConfig;
    QStringList unitsyncLibFileConfig;
    QStringList springUserDirConfig;
    QStringList springSettingsFileConfig;
    PreferencePathElement* springSystemDirPathElement;
    PreferencePathElement* springExecutableFilePathElement;
    PreferencePathElement* unitsyncLibFilePathElement;
    PreferencePathElement* springUserDirPathElement;
    PreferencePathElement* springSettingsFilePathElement;
    void initPathExamples();
    void setUpPathForm();
    void setUpLanguageComboBox();
    void loadPreferences();
    QStringList findQmFiles();
    QMap<QString, QString> languageNameLocaleMap;
    QTranslator* translator;
public slots:
    void onResetFormToSettings();
    void changeEvent( QEvent* event );
private slots:
    void onLanguageChanged( int index );
    void onOk();
    void onApply();
    void onCancel();
};

#endif
