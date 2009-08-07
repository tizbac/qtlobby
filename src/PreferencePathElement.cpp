// $Id$
// QtLobby released under the GPLv3, see COPYING for details.

#include "PreferencePathElement.h"

PreferencePathElement::PreferencePathElement( QStringList config, QWidget* parent ) : QWidget( parent ), Ui::PreferencePathElement() {
    setupUi( this );
    settings = Settings::Instance();
    m_config = config;
    variableName->setText( config[0] );
    isDirectory = config[0].contains( QRegExp( "dir$" ) ) ? true : false;
    examples = ( config[3] ).split( ";" );

    for ( int i = 0; i < examples.size(); ++i )
        examples[i] = QDir::toNativeSeparators( examples[i] );
    bool hasSetting = settings->contains( config[0] );
    if ( hasSetting )
        lineEdit->setText( settings->value( config[0], config[1] ).toString() );
    QStringList tmp;

    for ( int i = 0; i < examples.size(); ++i ) {
        QFileInfo fi( examples[i] );
        tmp << QString( "<font style=\"color:%1;\">%2</font>" )
            .arg( fi.exists() ? "green" : "red" )
            .arg( examples[i] );
        if( !hasSetting && fi.exists() ) {
            lineEdit->setText( examples[i] );
            hasSetting = true;
        }
    }

    descriptionLabel->setText( QString( "<span style=\"font-size:7pt;\">%1<br>%2</span>" ).arg( config[2], tmp.join(" ") ) );
    connect( fileBrowserToolButton, SIGNAL( clicked() ),
        this, SLOT( openFileBrowser() ) );
    connect( lineEdit, SIGNAL( textChanged( QString ) ),
        this, SLOT( updateExistingState( QString ) ) );

    updateExistingState();
}

PreferencePathElement::~PreferencePathElement() { }

void PreferencePathElement::SaveElement() {
    QString configparameter = settings->value( m_config[0] ).toString();
    if ( exists ) {
        if ( isDirectory ) {
            QDir d( lineEdit->text().trimmed() );
            QString dstr = QDir::toNativeSeparators( d.path() );
            lineEdit->setText( dstr );
        }
        lineEdit->setText( QDir::toNativeSeparators( lineEdit->text() ) );
        settings->setValue( m_config[0], lineEdit->text() );
    }
    updateExistingState();
}

void PreferencePathElement::ResetConfiguration() {
    lineEdit->setText( settings->value( m_config[0], m_config[1] ).toString() );
}

void PreferencePathElement::openFileBrowser() {
    QFileDialog fileDialog( this, m_config[0], examples.first() );
    QString fileOrDirName;
    updateExistingState();
    if ( isDirectory ) {
        QString path = exists ? lineEdit->text().trimmed() : QDir::homePath();
        fileOrDirName = fileDialog.getExistingDirectory( this, tr( "Select Directory" ), path, QFileDialog::ShowDirsOnly );
        emit pathChanged(fileOrDirName);
    } else {
        QFileInfo fi = QFileInfo( lineEdit->text().trimmed() );
        QString path = fi.exists() ? fi.absoluteFilePath() : QDir::homePath();
        fileOrDirName = fileDialog.getOpenFileName( this, tr( "Select File" ), path );
    }
    if ( fileOrDirName != "" )
        lineEdit->setText( QDir::toNativeSeparators( fileOrDirName ) );
}

void PreferencePathElement::updateExistingState( QString ) {
    QFileInfo fi = QFileInfo( lineEdit->text().trimmed() );
    exists = isDirectory ? fi.exists() && fi.isDir() : fi.exists();
    QString iconName = exists ? ":/icons/open_game.xpm" : ":/icons/closed_game.xpm";
    iconLabel->setPixmap( QPixmap( iconName ) );
}
