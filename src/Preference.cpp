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

#include "Preference.h"

UserPreference::UserPreference( QDialog* parent ) : QDialog( parent ) {

  setupUi( this );
  settings = Settings::Instance();

  connect( okPushButton, SIGNAL( clicked() ),
           this, SLOT( okClicked() ) );
  connect( applyPushButton, SIGNAL( clicked() ),
           this, SLOT( applyClicked() ) );
  connect( cancelPushButton, SIGNAL( clicked() ),
           this, SLOT( cancelClicked() ) );

  QVector<QStringList> elements;
  QStringList el;
  el << "spring_executable_with_abs_path_to_it"
  << "/usr/games/bin/spring"
  << "To execute spring we need the path to the spring binary."
  << "/usr/games/bin/spring;/usr/games/spring;%ProgramFiles%/Spring/Spring.exe";
  elements.append( el );

  el.clear();
  el << "spring_system_dir" // important that "dir" is at the and of the name!
  << "/usr/share/games/spring/"
  << "This folder usually contains the most spring data as for example the directories: AI/, base/, fonts/ and startscripts/."
  << "/usr/share/games/spring/;%ProgramFiles%/Spring/";
  elements.append( el );

//   el.clear();
//   el <<  "/usr/share/games/spring/base"
//   << "spring_system_dir/base"
//   << "Description: "
//   << "/usr/share/games/spring/base";
//   elements.append( el );

  el.clear();
  el <<  "unitsync"
  << "/usr/games/lib/spring/unitsync.so"
  << "This library will provide Qtlobby access to maps/mods which spring uses as well."
  << "/usr/games/lib/spring/unitsync.so;/usr/lib/spring/unitsync.so;/usr/games/lib64/spring/unitsync.so;%ProgramFiles%/qtlobby/release/unitsync.dll;c:/qtlobby/release/unitsync.dll";

  elements.append( el );

  el.clear();
  el << "spring_user_dir" // important that "dir" is at the and of the name!
  << QString( "%1/.spring/" ).arg( QDir::homePath() )
  << "On multiuser systems this path is used to store maps, mods and configurations which are user specific since most normal users won't have write-access to the global spring directory."
  << QString( "%1/.spring/" ).arg( QDir::homePath() );
  elements.append( el );

//   el.clear();
//   el <<  "spring_user_dir/maps"
//   << "/maps"
//   << "Description: "
//   << "";
//   elements.append( el );
//
//   el.clear();
//   el <<  "spring_user_dir/mods"
//   << "/mods"
//   << "Description: "
//   << "";
//   elements.append( el );
//
  el.clear();
  el <<  "springsettings+"
  << "/usr/bin/springsettings"
  << "Springsettings is a detailed and advanced configuration program. This binary is not in the qtlobby distribution yet so you might have to download it at springlobby.info"
  << "/usr/bin/springsettings;%ProgramFiles%/Spring/settings.exe";
  elements.append( el );

//   el.clear();
//   el <<  "replays_dir"
//   << "~/.spring/demos"
//   << "Description: "

//   el.clear();
//   el << ""
//      << ""
//      << ""
//      << "";
//   elements.append( el );

  QGridLayout *mainLayout = new QGridLayout();
  QWidget* container = new QWidget( this );
  container->setLayout( mainLayout );
  int h = 0;
  for ( int i = 0; i < elements.size(); ++i ) {
    ConfigElement* configElement = new ConfigElement( elements[i], container );
    mainLayout->addWidget( configElement, i, 0, 1, 1 );
    h += configElement->height();
    connect( this, SIGNAL( SaveAllElements() ),
             configElement, SLOT( SaveElement() ) );
    connect( this, SIGNAL( ResetAllElements() ),
             configElement, SLOT( ResetConfiguration() ) );
  }
  container->setMinimumHeight( h );
  QScrollArea* scrollArea = new QScrollArea( this );
  scrollArea->setWidget( container );
  scrollArea->setWidgetResizable( true );
  QGridLayout* f = new QGridLayout( preferencesPlaceholder );
  f->addWidget( scrollArea, 0, 0, 1, 1 );
}

UserPreference::~UserPreference() { }

void UserPreference::okClicked(){
  emit SaveAllElements();
  hide();
}

void UserPreference::applyClicked(){
  emit SaveAllElements();
}

void UserPreference::cancelClicked(){
  emit ResetAllElements();
  hide();
}
