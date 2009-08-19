// $Id$
// QtLobby released under the GPLv3, see COPYING for details.

#include "Preference.h"
#include "UnitSyncLib.h"

Preference::Preference( QDialog* parent ) : QDialog( parent ) {
    setupUi( this );
    setUpPathForm();
    settings = Settings::Instance();

    connect( okPushButton, SIGNAL( clicked() ),
             this, SLOT( onOk() ) );
    connect( applyPushButton, SIGNAL( clicked() ),
             this, SLOT( onApply() ) );
    connect( cancelPushButton, SIGNAL( clicked() ),
             this, SLOT( onCancel() ) );
    connect(languageComboBox, SIGNAL(activated(QString)),
            this, SLOT(onLanguageChanged(QString)));
    loadPreferences();
}

#define INIT_PREF(key, value) if(!settings->contains(key)) \
        settings->setValue(key, value);

//Moved it out from constructor for cake of clarity (ko)
void Preference::loadPreferences() {
    /*Chat*/
    INIT_PREF("Chat/joinMain", true);
    joinMainCheckBox->setChecked(settings->value("Chat/joinMain").toBool());
    INIT_PREF("Chat/joinQtlobby", true);
    joinQtlobbyCheckBox->setChecked(settings->value("Chat/joinQtlobby").toBool());
    INIT_PREF("Chat/showFlags", true);
    chatShowFlagsCheckBox->setChecked(settings->value("Chat/showFlags").toBool());
    INIT_PREF("Chat/highlightUserName", true);
    chatHighlightUserNameCheckBox->setChecked(settings->value("Chat/highlightUserName").toBool());
    INIT_PREF("Chat/popupNewPrivateChannel", false);
    popupNewPrivateChannelCheckBox->setChecked(settings->value("Chat/popupNewPrivateChannel").toBool());
    INIT_PREF("Chat/notifyNewPrivateMessages", true);
    notifyNewPrivateMessagesCheckBox->setChecked(settings->value("Chat/notifyNewPrivateMessages").toBool());

    /*Map viewing*/
    INIT_PREF("MapViewing/colorizedHeightmap", true);
    colrizedHeightMapCheckBox->setChecked(settings->value("MapViewing/colorizedHeightmap").toBool());
    grayScaleHeightmapCheckBox->setChecked(!settings->value("MapViewing/colorizedHeightmap").toBool());
    INIT_PREF("MapViewing/downscaleHeightmap", 1);
    downscaleHeightmapSpinBox->setValue(settings->value("MapViewing/downscaleHeightmap").toInt());
    INIT_PREF("MapViewing/metalmapSuperposition", false);
    metalmapSuperpositionCheckBox->setChecked(settings->value("MapViewing/metalmapSuperposition").toBool());

    /*Start Positions*/
    INIT_PREF("MapViewing/startPos/showOnMinimapCheckBox", false);
    startPosShowOnMinimapCheckBox->setChecked(settings->value("MapViewing/startPos/showOnMinimapCheckBox").toBool());
    INIT_PREF("MapViewing/startPos/showOnHeightmapCheckBox", false);
    startPosShowOnHeightmapCheckBox->setChecked(settings->value("MapViewing/startPos/showOnHeightmapCheckBox").toBool());
    INIT_PREF("MapViewing/startPos/showOnMetalmapCheckBox", false);
    startPosShowOnMetalmapCheckBox->setChecked(settings->value("MapViewing/startPos/showOnMetalmapCheckBox").toBool());
    INIT_PREF("MapViewing/startPos/showOn3DMapCheckBox", false);
    startPosShowOn3DMapCheckBox->setChecked(settings->value("MapViewing/startPos/showOn3DMapCheckBox").toBool());
    INIT_PREF("MapViewing/startPos/startRect/brushNumber", 0);
    startRectBrushComboBox->setCurrentIndex(settings->value("MapViewing/startPos/startRect/brushNumber").toInt());
    INIT_PREF("MapViewing/startPos/startRect/borderWidth", 1);
    startRectBorderWidthSpinBox->setValue(settings->value("MapViewing/startPos/startRect/borderWidth").toInt());
    INIT_PREF("MapViewing/startPos/startRect/alpha", 100);
    startRectAlphaSpinBox->setValue(settings->value("MapViewing/startPos/startRect/alpha").toInt());
}

Preference::~Preference() { }

void Preference::onOk() {
    onApply();
    if (UnitSyncLib::getInstance()->libraryLoaded())
        hide();
}

void Preference::onApply() {
    for ( int i = 0; i < pathElements.size(); ++i )
        pathElements[i]->saveElement();
    UnitSyncLib::getInstance()->loadLibrary();

    /*Chat*/
    settings->setValue("Chat/joinMain", joinMainCheckBox->isChecked());
    settings->setValue("Chat/joinQtlobby", joinQtlobbyCheckBox->isChecked());
    settings->setValue("Chat/showFlags", chatShowFlagsCheckBox->isChecked());
    settings->setValue("Chat/highlightUserName", chatHighlightUserNameCheckBox->isChecked());
    settings->setValue("Chat/popupNewPrivateChannel", popupNewPrivateChannelCheckBox->isChecked());
    settings->setValue("Chat/notifyNewPrivateMessages", notifyNewPrivateMessagesCheckBox->isChecked());

    /*Map viewing*/
    settings->setValue("MapViewing/colorizedHeightmap", colrizedHeightMapCheckBox->isChecked());
    settings->setValue("MapViewing/downscaleHeightmap", downscaleHeightmapSpinBox->value());
    settings->setValue("MapViewing/metalmapSuperposition", metalmapSuperpositionCheckBox->isChecked());

    /*Start Positions*/
    settings->setValue("MapViewing/startPos/showOnMinimapCheckBox", startPosShowOnMinimapCheckBox->isChecked());
    settings->setValue("MapViewing/startPos/showOnHeightmapCheckBox", startPosShowOnHeightmapCheckBox->isChecked());
    settings->setValue("MapViewing/startPos/showOnMetalmapCheckBox", startPosShowOnMetalmapCheckBox->isChecked());
    settings->setValue("MapViewing/startPos/showOn3DMapCheckBox", startPosShowOn3DMapCheckBox->isChecked());
    settings->setValue("MapViewing/startPos/startRect/brushNumber", startRectBrushComboBox->currentIndex());
    settings->setValue("MapViewing/startPos/startRect/borderWidth", startRectBorderWidthSpinBox->value());
    settings->setValue("MapViewing/startPos/startRect/alpha", startRectAlphaSpinBox->value());
}

void Preference::onCancel() {
    for ( int i = 0; i < pathElements.size(); ++i )
        pathElements[i]->resetConfiguration();

    /*Chat*/
    joinMainCheckBox->setChecked(settings->value("Chat/joinMain").toBool());
    joinQtlobbyCheckBox->setChecked(settings->value("Chat/joinQtlobby").toBool());
    chatShowFlagsCheckBox->setChecked(settings->value("Chat/showFlags").toBool());
    chatHighlightUserNameCheckBox->setChecked(settings->value("Chat/highlightUserName").toBool());
    popupNewPrivateChannelCheckBox->setChecked(settings->value("Chat/popupNewPrivateChannel").toBool());
    notifyNewPrivateMessagesCheckBox->setChecked(settings->value("Chat/notifyNewPrivateMessages").toBool());

    /*Map viewing*/
    colrizedHeightMapCheckBox->setChecked(settings->value("MapViewing/colorizedHeightmap").toBool());
    grayScaleHeightmapCheckBox->setChecked(!settings->value("MapViewing/colorizedHeightmap").toBool());
    downscaleHeightmapSpinBox->setValue(settings->value("MapViewing/downscaleHeightmap").toInt());
    metalmapSuperpositionCheckBox->setChecked(settings->value("MapViewing/metalmapSuperposition").toBool());

    /*Start Positions*/
    startPosShowOnMinimapCheckBox->setChecked(settings->value("MapViewing/startPos/showOnMinimapCheckBox").toBool());
    startPosShowOnHeightmapCheckBox->setChecked(settings->value("MapViewing/startPos/showOnHeightmapCheckBox").toBool());
    startPosShowOnMetalmapCheckBox->setChecked(settings->value("MapViewing/startPos/showOnMetalmapCheckBox").toBool());
    startPosShowOn3DMapCheckBox->setChecked(settings->value("MapViewing/startPos/showOn3DMapCheckBox").toBool());
    startRectBrushComboBox->setCurrentIndex(settings->value("MapViewing/startPos/startRect/brushNumber").toInt());
    startRectBorderWidthSpinBox->setValue(settings->value("MapViewing/startPos/startRect/borderWidth").toInt());
    startRectAlphaSpinBox->setValue(settings->value("MapViewing/startPos/startRect/alpha").toInt());

    hide();
}

void Preference::onLanguageChanged(QString language) {
    //FIXME language switch does not work yet
    //need to update the languageComboBox state for current locale in constructor (mw)
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

void Preference::setUpPathForm() {
    initPathExamples();
    QGridLayout *mainLayout = new QGridLayout();
    QWidget* container = new QWidget( this );
    container->setLayout( mainLayout );

    springSystemDirPathElement = new PreferencePathElement( container );
    springSystemDirPathElement->setIsDirectory( true );
    springSystemDirPathElement->setConfig( springSystemDirConfig );
    pathElements << springSystemDirPathElement;

    springExecutableFilePathElement = new PreferencePathElement( container );
    springExecutableFilePathElement->setConfig( springExecutableFileConfig );
    pathElements << springExecutableFilePathElement;

    unitsyncLibFilePathElement = new PreferencePathElement( container );
    unitsyncLibFilePathElement->setConfig( unitsyncLibFileConfig );
    pathElements << unitsyncLibFilePathElement;

    springUserDirPathElement = new PreferencePathElement( container );
    springUserDirPathElement->setIsDirectory( true );
    springUserDirPathElement->setConfig( springUserDirConfig );
    pathElements << springUserDirPathElement;

    springSettingsFilePathElement = new PreferencePathElement( container );
    springSettingsFilePathElement->setConfig( springSettingsFileConfig );
    pathElements << springSettingsFilePathElement;

    // under windows you hopefully have to specify only springSystemDir
    // TODO need testing, wheter this works as espected:
    // after changing the spring_system_dir all other red marked paths should get green
    // especially the unitsync should be found.
//#ifdef Q_WS_WIN
    connect( springSystemDirPathElement, SIGNAL( pathChanged( QString ) ),
             springExecutableFilePathElement, SLOT( onBaseDirectoryChanged( QString ) ) );
    connect( springSystemDirPathElement, SIGNAL( pathChanged( QString ) ),
             unitsyncLibFilePathElement, SLOT( onBaseDirectoryChanged( QString ) ) );
    connect( springSystemDirPathElement, SIGNAL( pathChanged( QString ) ),
             springUserDirPathElement, SLOT( onBaseDirectoryChanged( QString ) ) );
    connect( springSystemDirPathElement, SIGNAL( pathChanged( QString ) ),
             springSettingsFilePathElement, SLOT( onBaseDirectoryChanged( QString ) ) );
//#endif

    int h = 0;
    for ( int i = 0; i < pathElements.size(); ++i ) {
        mainLayout->addWidget( pathElements[i], i, 0, 1, 1 );
        h += pathElements[i]->height();
    }
    container->setMinimumHeight( h );
    QScrollArea* scrollArea = new QScrollArea( this );
    scrollArea->setWidget( container );
    scrollArea->setWidgetResizable( true );
    QGridLayout* f = new QGridLayout( pathWidget );
    f->addWidget( scrollArea, 0, 0, 1, 1 );
}

void Preference::initPathExamples() {
#ifdef Q_WS_WIN
    QString pfiles = getenv("ProgramW6432");
    QString pfiles2= getenv("ProgramFiles");
    if (pfiles == "")
        pfiles = "C:/Program Files";
#endif
/**
 Syntax for configurations:
    configurationName << variableName
            << tr("This is the variable description")
            << "/path/to/directory/xy;/path/to/directory/xy/try/2";
 */
    springSystemDirConfig << "spring_system_dir"
            << tr("This folder contains the most Spring data for example the directories: AI/, base/, fonts/ and startscripts/.")
#ifdef Q_WS_WIN
            <<
            pfiles+"/Spring/;"
            + pfiles2+"/Spring/;"
            "../;"
            "./;";
#else
            <<
            "/usr/local/share/games/spring/;"
            "/usr/share/games/spring/;";
#endif

    springExecutableFileConfig << "spring_executable_with_abs_path_to_it"
            << tr("To start Spring we need the location of the Spring binary.")
#ifdef Q_WS_WIN
            <<
            pfiles+"/Spring/Spring.exe;"
            + pfiles2+"/Spring/Spring.exe;"
            "../spring.exe;"
            "./spring.exe;";
#else
            <<
            "/usr/local/bin/spring;"
            "/usr/games/bin/spring;"
            "/usr/games/spring;"
            "/usr/bin/spring";
#endif

    unitsyncLibFileConfig <<  "unitsync"
            << tr("This library is used to retreive information about maps and mods.")
#ifdef Q_WS_WIN
            <<
            pfiles+"/Spring/unitsync.dll;"
            + pfiles2+"/Spring/unitsync.dll;"
            "../unitsync.dll;"
            "./unitsync.dll;";
#else
            <<
            "/usr/local/lib/libunitsync.so;"
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
    springUserDirConfig << "spring_user_dir"
            << tr("On multi user systems this path is used to store maps, mods and configurations which are user specific since most normal users won't have write access to the global spring directory.")
#ifdef Q_WS_WIN
            <<
            pfiles+"/Spring/;"
            + pfiles2+"/Spring/;"
            "../;"
            "./;";
#else
            <<
            QString( "%1/.spring/" ).arg( QDir::homePath() );
#endif
    springSettingsFileConfig << "springsettings+"
            << tr("Springsettings is a detailed and advanced configuration program. This binary is not in the qtlobby distribution yet so you might have to download it at springlobby.info")
#ifdef Q_WS_WIN
            <<
            pfiles+"/Spring/springsettings.exe;"
            + pfiles2+"/Spring/springsettings.exe;"
            "../springsettings.exe;"
            "./springsettings.exe;";
#else
            <<
            "/usr/local/bin/springsettings;"
            "/usr/games/bin/springsettings;"
            "/usr/games/springsettings;"
            "/usr/bin/springsettings";
#endif
}
