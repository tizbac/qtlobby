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
#include "UnitSyncLib.h"

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
            << "/usr/local/bin/spring"
            << "To execute spring we need the path to the spring binary."
#ifdef Q_WS_WIN
            << "./spring.exe;"
            "../spring.exe;"
            "%programfiles%/Spring/Spring.exe;"
            "C:/Program Files/Spring/Spring.exe;"
            "D:/Program Files/Spring/Spring.exe;"
            "E:/Program Files/Spring/Spring.exe;"
            "F:/Program Files/Spring/Spring.exe;"
            "C:/Games/Spring/Spring.exe;"
            "D:/Games/Spring/Spring.exe;"
            "E:/Games/Spring/Spring.exe;"
            "F:/Games/Spring/Spring.exe;"
            "C:/Program Files (x86)/Spring/Spring.exe;"
            "D:/Program Files (x86)/Spring/Spring.exe;"
            "E:/Program Files (x86)/Spring/Spring.exe;"
            "F:/Program Files (x86)/Spring/Spring.exe;";
#else
    <<      "/usr/local/bin/spring;"
            "/usr/games/bin/spring;"
            "/usr/games/spring;"
            "/usr/bin/spring";
#endif
    elements.append( el );

    el.clear();
    el << "spring_system_dir" // important that "dir" is at the and of the name!
            << "/usr/local/share/games/spring/"
            << "This folder usually contains the most spring data as for example the directories: AI/, base/, fonts/ and startscripts/."
#ifdef Q_WS_WIN
            << "./;"
            "../;"
            "%programfiles%/Spring/;"
            "C:/Program Files/Spring/;"
            "D:/Program Files/Spring/;"
            "E:/Program Files/Spring/;"
            "F:/Program Files/Spring/;"
            "C:/Games/Spring/;"
            "D:/Games/Spring/;"
            "E:/Games/Spring/;"
            "F:/Games/Spring/;"
            "C:/Program Files (x86)/Spring/;"
            "D:/Program Files (x86)/Spring/;"
            "E:/Program Files (x86)/Spring/;"
            "F:/Program Files (x86)/Spring/;";
#else
    <<      "/usr/local/share/games/spring/;"
            "/usr/share/games/spring/;";
#endif
    elements.append( el );

    //   el.clear();
    //   el <<  "/usr/share/games/spring/base"
    //   << "spring_system_dir/base"
    //   << "Description: "
    //   << "/usr/share/games/spring/base";
    //   elements.append( el );

    el.clear();
    el <<  "unitsync"
            << "/usr/local/lib/libunitsync.so"
            << "This library will provide Qtlobby access to maps/mods which spring uses as well."
#ifdef Q_WS_WIN
            << "./unitsync.dll;"
            "../unitsync.dll;"
            "%programfiles%/Spring/unitsync.dll;"
            "C:/Program Files/Spring/unitsync.dll;"
            "D:/Program Files/Spring/unitsync.dll;"
            "E:/Program Files/Spring/unitsync.dll;"
            "F:/Program Files/Spring/unitsync.dll;"
            "C:/Games/Spring/unitsync.dll;"
            "D:/Games/Spring/unitsync.dll;"
            "E:/Games/Spring/unitsync.dll;"
            "F:/Games/Spring/unitsync.dll;"
            "C:/Program Files (x86)/Spring/unitsync.dll;"
            "D:/Program Files (x86)/Spring/unitsync.dll;"
            "E:/Program Files (x86)/Spring/unitsync.dll;"
            "F:/Program Files (x86)/Spring/unitsync.dll;";
#else
    << "/usr/local/lib/libunitsync.so;"
            "/usr/lib/libunitsync.so;"
            "/usr/lib/spring/lib/libunitsync.so;"
            "/usr/games/lib/spring/libunitsync.so;"
            "/usr/local/lib/unitsync.so;"
            "/usr/lib/unitsync.so;"
            "/usr/lib/spring/lib/unitsync.so;"
            "/usr/games/lib/spring/unitsync.so;";
#endif

    elements.append( el );

    el.clear();
    el << "spring_user_dir" // important that "dir" is at the and of the name!
            << QString( "%1/.spring/" ).arg( QDir::homePath() )
            << "On multiuser systems this path is used to store maps, mods and configurations which are user specific since most normal users won't have write-access to the global spring directory."
#ifdef Q_WS_WIN
            << "./;"
            "../;"
            "%programfiles%/Spring/;"
            "C:/Program Files/Spring/;"
            "D:/Program Files/Spring/;"
            "E:/Program Files/Spring/;"
            "F:/Program Files/Spring/;"
            "C:/Games/Spring/;"
            "D:/Games/Spring/;"
            "E:/Games/Spring/;"
            "F:/Games/Spring/;"
            "C:/Program Files (x86)/Spring/;"
            "D:/Program Files (x86)/Spring/;"
            "E:/Program Files (x86)/Spring/;"
            "F:/Program Files (x86)/Spring/;";
#else
    << QString( "%1/.spring/" ).arg( QDir::homePath() );
#endif

    elements.append( el );
    el.clear();
    el <<  "springsettings+"
            << "/usr/bin/springsettings"
            << "Springsettings is a detailed and advanced configuration program. This binary is not in the qtlobby distribution yet so you might have to download it at springlobby.info"
#ifdef Q_WS_WIN
            << "./springsettings.exe;"
            "../springsettings.exe;"
            "%programfiles%/Spring/springsettings.exe;"
            "C:/Program Files/Spring/springsettings.exe;"
            "D:/Program Files/Spring/springsettings.exe;"
            "E:/Program Files/Spring/springsettings.exe;"
            "F:/Program Files/Spring/springsettings.exe;"
            "C:/Games/Spring/springsettings.exe;"
            "D:/Games/Spring/springsettings.exe;"
            "E:/Games/Spring/springsettings.exe;"
            "F:/Games/Spring/springsettings.exe;"
            "C:/Program Files (x86)/Spring/springsettings.exe;"
            "D:/Program Files (x86)/Spring/springsettings.exe;"
            "E:/Program Files (x86)/Spring/springsettings.exe;"
            "F:/Program Files (x86)/Spring/springsettings.exe;";
#else
    << "/usr/local/bin/springsettings;"
            "/usr/games/bin/springsettings;"
            "/usr/games/springsettings;"
            "/usr/bin/springsettings";
#endif
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
        this->elements << configElement;
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
    for ( int i = 0; i < elements.size(); ++i ) {
        elements[i]->SaveElement();
    }
    if(UnitSyncLib::getInstance()->loadLibrary())
        hide();
}

void UserPreference::applyClicked(){
    for ( int i = 0; i < elements.size(); ++i ) {
        elements[i]->SaveElement();
    }
    UnitSyncLib::getInstance()->loadLibrary();
}

void UserPreference::cancelClicked(){
    for ( int i = 0; i < elements.size(); ++i ) {
        elements[i]->ResetConfiguration();
    }
    hide();
}
