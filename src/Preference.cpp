// $Id$
// QtLobby released under the GPLv3, see COPYING for details.

#include "Preference.h"
#include "UnitSyncLib.h"

UserPreference::UserPreference( QDialog* parent ) : QDialog( parent ) {

    setupUi( this );

    QVector<QStringList> elements = getPathElements();
    QGridLayout *mainLayout = new QGridLayout();
    QWidget* container = new QWidget( this );
    container->setLayout( mainLayout );
    int h = 0;
    for ( int i = 0; i < elements.size(); ++i ) {
        PreferencePathElement* pathElement = new PreferencePathElement( elements[i], container );
        mainLayout->addWidget( pathElement, i, 0, 1, 1 );
        h += pathElement->height();
        this->pathElements << pathElement;
    }
    container->setMinimumHeight( h );
    QScrollArea* scrollArea = new QScrollArea( this );
    scrollArea->setWidget( container );
    scrollArea->setWidgetResizable( true );
    QGridLayout* f = new QGridLayout( pathWidget );
    f->addWidget( scrollArea, 0, 0, 1, 1 );

    settings = Settings::Instance();

    connect( okPushButton, SIGNAL( clicked() ),
             this, SLOT( okClicked() ) );
    connect( applyPushButton, SIGNAL( clicked() ),
             this, SLOT( applyClicked() ) );
    connect( cancelPushButton, SIGNAL( clicked() ),
             this, SLOT( cancelClicked() ) );
    connect(languageComboBox, SIGNAL(activated(QString)),
            this, SLOT(languageChanged(QString)));
    if(!settings->contains("Chat/joinMain"))
        settings->setValue("Chat/joinMain", true);
    joinMainCheckBox->setChecked(settings->value("Chat/joinMain").toBool());
    if(!settings->contains("Chat/joinQtlobby"))
        settings->setValue("Chat/joinQtlobby", true);
    joinQtlobbyCheckBox->setChecked(settings->value("Chat/joinQtlobby").toBool());
}

UserPreference::~UserPreference() { }

void UserPreference::okClicked() {
    applyClicked();
    if (UnitSyncLib::getInstance()->libraryLoaded())
        hide();
}

void UserPreference::applyClicked() {
    for ( int i = 0; i < pathElements.size(); ++i )
        pathElements[i]->SaveElement();
    UnitSyncLib::getInstance()->loadLibrary();
    settings->setValue("Chat/joinMain", joinMainCheckBox->isChecked());
    settings->setValue("Chat/joinQtlobby", joinQtlobbyCheckBox->isChecked());
}

void UserPreference::cancelClicked() {
    for ( int i = 0; i < pathElements.size(); ++i )
        pathElements[i]->ResetConfiguration();
    joinMainCheckBox->setChecked(settings->value("Chat/joinMain").toBool());
    joinQtlobbyCheckBox->setChecked(settings->value("Chat/joinQtlobby").toBool());
    hide();
}

void UserPreference::languageChanged(QString language) {
    //language switch does not work yet
    //need to update the languageComboBox state for current locale in constructor
    QString locale;
    QTranslator translator;
    translator.load(QString(":/i18n/qtlobby_de"));
    if( language == "Deutsch" )
        locale = "de";
    else if( language == "English" ) {
        QCoreApplication::instance()->removeTranslator(&translator);
        qDebug() << "removing translator";
        return;
    } else return;
    QCoreApplication::instance()->installTranslator(&translator);
}

QVector<QStringList> UserPreference::getPathElements() {
#ifdef Q_WS_WIN
    QString pfiles = getenv("ProgramW6432");
    QString pfiles2= getenv("ProgramFiles");
    if (pfiles == "")
        pfiles = "C:\\Program Files";
#endif

    QVector<QStringList> elements;
    QStringList el;
    el << "spring_executable_with_abs_path_to_it"
            << "/usr/local/bin/spring"
            << "To execute spring we need the path to the spring binary."
#ifdef Q_WS_WIN
            <<
            pfiles+"/Spring/Spring.exe;" +
            pfiles2+"/Spring/Spring.exe;"
            "../spring.exe;"
            "./spring.exe;";
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
            <<
            pfiles+"/Spring/;" +
            pfiles2+"/Spring/;"
            "../;"
            "./;";
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
            <<
            pfiles+"/Spring/unitsync.dll;" +
            pfiles2+"/Spring/unitsync.dll;"
            "../unitsync.dll;"
            "./unitsync.dll;";
#else
    << "/usr/local/lib/libunitsync.so;"
            "/usr/lib/libunitsync.so;"
            "/usr/lib64/libunitsync.so;"
            "/usr/lib/spring/lib/libunitsync.so;"
            "/usr/games/lib/spring/libunitsync.so;"
            "/usr/local/lib/unitsync.so;"
            "/usr/lib/unitsync.so;"
            "/usr/lib/spring/lib/unitsync.so;"
            "/usr/games/lib/spring/unitsync.so;"
            "/usr/games/lib64/libunitsync.so";

#endif

    elements.append( el );

    el.clear();
    el << "spring_user_dir" // important that "dir" is at the and of the name!
            << QString( "%1/.spring/" ).arg( QDir::homePath() )
            << "On multiuser systems this path is used to store maps, mods and configurations which are user specific since most normal users won't have write-access to the global spring directory."
#ifdef Q_WS_WIN
            <<
            pfiles+"/Spring/;" +
            pfiles2+"/Spring/;"
            "../;"
            "./;";
#else
    << QString( "%1/.spring/" ).arg( QDir::homePath() );
#endif

    elements.append( el );
    el.clear();
    el <<  "springsettings+"
            << "/usr/bin/springsettings"
            << "Springsettings is a detailed and advanced configuration program. This binary is not in the qtlobby distribution yet so you might have to download it at springlobby.info"
#ifdef Q_WS_WIN
            <<
            pfiles+"/Spring/springsettings.exe;" +
            pfiles2+"/Spring/springsettings.exe;"
            "../springsettings.exe;"
            "./springsettings.exe;";
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
    return elements;
}
