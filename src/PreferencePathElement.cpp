// $Id$
// QtLobby released under the GPLv3, see COPYING for details.

#include "PreferencePathElement.h"

PreferencePathElement::PreferencePathElement( QWidget* parent ) : QWidget( parent ), Ui::PreferencePathElement() {
    setupUi( this );
    settings = Settings::Instance();
    m_hasSetting = false;
    m_isDirectory = false;
    connect( fileBrowserToolButton, SIGNAL( clicked() ),
        this, SLOT( openFileBrowser() ) );
    connect( lineEdit, SIGNAL( textChanged( QString ) ),
        this, SLOT( updateExistingState( QString ) ) );
}

PreferencePathElement::~PreferencePathElement() { }

bool PreferencePathElement::isDirectory() {
    return m_isDirectory;
}

void PreferencePathElement::setIsDirectory( bool isDirectory ) {
    m_isDirectory = isDirectory;
}

QString PreferencePathElement::variableName() {
    return m_variableName;
}

void PreferencePathElement::setVariableName( const QString & variableName ) {
    m_variableName = variableName;
    variableNameLabel->setText( m_variableName );
    if ( settings->contains( m_variableName ) ) {
        QString s = settings->value( m_variableName ).toString();
        if ( !s.isEmpty() ) {
            m_hasSetting = true;
            lineEdit->setText( s );
        }
    }
}

QStringList PreferencePathElement::examples() {
    return m_examples;
}

void PreferencePathElement::setExamples( const QStringList & examples ) {
    m_examples = examples;
    searchInExamples();
}

void PreferencePathElement::searchInExamples() {
    int s = m_examples.size();
    for ( int i = 0; i < s; ++i ) {
        m_examples[i] = QDir::toNativeSeparators( m_examples[i] );
        QFileInfo fi( m_examples[i] );
        if( !m_hasSetting && fi.exists() ) {
            lineEdit->setText( m_examples[i] );
            m_hasSetting = true;
        }
    }
}

void PreferencePathElement::setDescription( const QString & description ) {
    descriptionLabel->setText( QString( "<span style=\"font-size:7pt;\">%1</span>" ).arg( description ) );
}

void PreferencePathElement::setConfig( QStringList config ) {
    setVariableName( config.takeFirst() );
    setDescription( config.takeFirst() );
    setExamples( config.first().split(";") );
    updateExistingState();
}

void PreferencePathElement::onBaseDirectoryChanged( QString baseDirectory ) {
    m_baseDirectory = QDir::toNativeSeparators( QDir( baseDirectory ).path() + "/" );
    QString newPath = QDir::cleanPath( m_baseDirectory + m_examples.last() );
    if( isExistingPath( newPath ) && !m_examples.contains( newPath ) )
        m_examples.prepend( newPath );
    searchInExamples();
    updateExistingState();
}

void PreferencePathElement::saveElement() {
    QString configparameter = settings->value( m_variableName ).toString();
    if ( m_exists ) {
        if ( m_isDirectory )
            lineEdit->setText( QDir::toNativeSeparators( QDir( lineEdit->text().trimmed() ).path() ) );
        lineEdit->setText( QDir::toNativeSeparators( lineEdit->text().trimmed() ) );
        settings->setValue( m_variableName, lineEdit->text() );
    }
    updateExistingState();
}

void PreferencePathElement::resetConfiguration() {
    lineEdit->setText( settings->value( m_variableName ).toString() );
}

void PreferencePathElement::openFileBrowser() {
    QFileDialog fileDialog( this, m_variableName, m_baseDirectory );
    QString fileOrDirName;
    updateExistingState();
    if ( m_isDirectory ) {
        QString path;
        if ( m_exists )
            path = lineEdit->text().trimmed();
        else if ( isExistingPath( m_baseDirectory ) )
            path = m_baseDirectory;
        else
            path = QDir::homePath();
        fileOrDirName = fileDialog.getExistingDirectory( this, tr( "Select Directory" ) + " " + m_variableName, path, QFileDialog::ShowDirsOnly );
        emit pathChanged( fileOrDirName );
    } else {
        QFileInfo fi = QFileInfo( lineEdit->text().trimmed() );
        QString path;
        if ( fi.exists() )
            path = fi.absoluteFilePath();
        else if ( isExistingPath( m_baseDirectory ) )
            path = m_baseDirectory;
        else
            path = QDir::homePath();
        QString filter = m_examples.last().contains(".") ? m_examples.last().split(".").last().prepend("*.") : "*";
        fileOrDirName = fileDialog.getOpenFileName( this, tr( "Select File" ) + " " + m_variableName, path, filter );
    }
    if ( fileOrDirName != "" )
        lineEdit->setText( QDir::toNativeSeparators( fileOrDirName ) );
    updateExistingState();
}

bool PreferencePathElement::isExistingPath( const QString & path ) {
    return QFileInfo( path.trimmed() ).exists();
}

void PreferencePathElement::updateExistingState( QString ) {
    QFileInfo fi = QFileInfo( lineEdit->text().trimmed() );
    m_exists = m_isDirectory ? fi.exists() && fi.isDir() : fi.exists();
    QString iconName = m_exists ? ":/icons/open_game.xpm" : ":/icons/closed_game.xpm";
    iconLabel->setPixmap( QPixmap( iconName ) );
}
