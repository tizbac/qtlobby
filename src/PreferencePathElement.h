// $Id$
// QtLobby released under the GPLv3, see COPYING for details.

#ifndef PREFERENCEPATHELEMENT_H
#define PREFERENCEPATHELEMENT_H

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
    PreferencePathElement( QWidget* parent = 0 );
    ~PreferencePathElement();
    bool isDirectory();
    void setIsDirectory(bool isDirectory);
    bool exists();
    bool hasSetting();
    QString variableName();
    void setVariableName( const QString & variableName );
    QStringList examples();
    void setExamples( const QStringList & examples );
    void setDescription( const QString & description );
    bool isExistingPath( const QString & path );
    void setConfig( QStringList config );
protected:
    QSettings* settings;
    bool m_exists;
    bool m_isDirectory;
    bool m_hasSetting;
    QString m_variableName;
    QStringList m_examples;
    QString m_baseDirectory;
    void searchInExamples();
signals:
    void pathChanged(QString dir);
public slots:
    void saveElement();
    void resetConfiguration();
    void onBaseDirectoryChanged( QString baseDirectory );
protected slots:
    void openFileBrowser();
    void updateExistingState( QString a = "" );
};

#endif
