//
// C++ Implementation:
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "ConfigElement.h"

ConfigElement::ConfigElement( QStringList config, QWidget* parent ) : QWidget( parent ), Ui::configElement() {
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
  bool found = false;
  QString tmp;

  for ( int i = 0; i < examples.size(); ++i ) {
    QFileInfo fi( examples[i] );
    tmp.append( QString( "<font style=\"color:%1;\"><b>%2</b></font>%3" )
                .arg( fi.exists() ? "green" : "red" )
                .arg( examples[i] )
                .arg( i % 3 == 2 ? "<br>" : "&nbsp;" ) );
	if(fi.exists()) {
		lineEdit->setText( examples[i] );
	}
	//if ( !found ) {// && !hasSetting ) {
    //  found = true;
      
    //}
  }

  descriptionLabel->setText( QString( "<span style=\"font-size:8pt;\">%1<br>%2</span>" ).arg( config[2], tmp ) );
  connect( fileBrowserToolButton, SIGNAL( clicked() ),
           this, SLOT( openFileBrowser() ) );
  connect( lineEdit, SIGNAL( textChanged( QString ) ),
           this, SLOT( updateExistingState( QString ) ) );

  updateExistingState();
}

ConfigElement::~ConfigElement() { }

void ConfigElement::SaveElement() {
  QString configparameter = settings->value( m_config[0] ).toString();
  if ( configparameter != lineEdit->text() && exists ) {
    if ( isDirectory ) {
      QDir d( lineEdit->text().trimmed() );
      if ( lineEdit->text() != d.path() )
        lineEdit->setText( d.path() );
    }
    settings->setValue( m_config[0], lineEdit->text() );
  }
  updateExistingState();
}

void ConfigElement::ResetConfiguration() {
  lineEdit->setText( settings->value( m_config[0], m_config[1] ).toString() );
}

void ConfigElement::openFileBrowser() {
  QFileDialog fileDialog( this, m_config[0], examples.first() );
  QString fileOrDirName;
  updateExistingState();
  if ( isDirectory ) {
    QString path = exists ? lineEdit->text().trimmed() : QDir::homePath();
    fileOrDirName = fileDialog.getExistingDirectory( this, tr( "Select Directory" ), path, QFileDialog::ShowDirsOnly );
  } else {
    QFileInfo fi = QFileInfo( lineEdit->text().trimmed() );
    QString path = fi.exists() ? fi.absoluteFilePath() : QDir::homePath();
    fileOrDirName = fileDialog.getOpenFileName( this, tr( "Select File" ), path );
  }
  if ( fileOrDirName != "" )
    lineEdit->setText( fileOrDirName );
}

void ConfigElement::updateExistingState( QString ) {
  QFileInfo fi = QFileInfo( lineEdit->text().trimmed() );
  exists = isDirectory ? fi.exists() && fi.isDir() : fi.exists();
  QString iconName = exists ? ":/icons/open_game.xpm" : ":/icons/closed_game.xpm";
  iconLabel->setPixmap( QPixmap( iconName ) );
}
