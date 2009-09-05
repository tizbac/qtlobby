// $Id$
// QtLobby released under the GPLv3, see COPYING for details.

#include "Preference.h"
#include "UnitSyncLib.h"

Preference::Preference( QDialog* parent ) : QDialog( parent ) {
    settings = Settings::Instance();
    setupUi( this );
    setUpPathForm();
    setUpLanguageComboBox();
    translator = new QTranslator(this);
    connect( okPushButton, SIGNAL( clicked() ), SLOT( onOk() ) );
    connect( applyPushButton, SIGNAL( clicked() ), SLOT( onApply() ) );
    connect( cancelPushButton, SIGNAL( clicked() ), SLOT( onCancel() ) );
    connect( languageComboBox, SIGNAL( activated( int ) ), SLOT( onLanguageChanged( int ) ) );
    loadPreferences();
    // show first page by default
    preferencesListWidget->setCurrentRow(0);
    stackedWidget->setCurrentIndex(0);
}

#define INIT_PREF(key, value) if(!settings->contains(key)) \
        settings->setValue(key, value);

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
    INIT_PREF("MapViewing/perspectiveProjectionType", true);
    projectionPerspectiveRadioButton->setChecked(settings->value("MapViewing/perspectiveProjectionType").toBool());
    projectionOrghogonalRadioButton->setChecked(!settings->value("MapViewing/perspectiveProjectionType").toBool());
    INIT_PREF("MapViewing/useShaders", true);
    useShadersCheckBox->setChecked(settings->value("MapViewing/useShaders").toBool());

    /*Start Positions*/
    INIT_PREF("MapViewing/startPos/showOnMinimapCheckBox", true);
    startPosShowOnMinimapCheckBox->setChecked(settings->value("MapViewing/startPos/showOnMinimapCheckBox").toBool());
    INIT_PREF("MapViewing/startPos/showOnHeightmapCheckBox", true);
    startPosShowOnHeightmapCheckBox->setChecked(settings->value("MapViewing/startPos/showOnHeightmapCheckBox").toBool());
    INIT_PREF("MapViewing/startPos/showOnMetalmapCheckBox", true);
    startPosShowOnMetalmapCheckBox->setChecked(settings->value("MapViewing/startPos/showOnMetalmapCheckBox").toBool());
    INIT_PREF("MapViewing/startPos/showOn3DMapCheckBox", true);
    startPosShowOn3DMapCheckBox->setChecked(settings->value("MapViewing/startPos/showOn3DMapCheckBox").toBool());
    INIT_PREF("MapViewing/startPos/startRect/brushNumber", 0);
    startRectBrushComboBox->setCurrentIndex(settings->value("MapViewing/startPos/startRect/brushNumber").toInt());
    INIT_PREF("MapViewing/startPos/startRect/borderWidth", 1);
    startRectBorderWidthSpinBox->setValue(settings->value("MapViewing/startPos/startRect/borderWidth").toInt());
    INIT_PREF("MapViewing/startPos/startRect/alpha", 100);
    startRectAlphaSpinBox->setValue(settings->value("MapViewing/startPos/startRect/alpha").toInt());

    /*General*/
    INIT_PREF("Battle/autoCloseFirst", false);
    battleAutoCloseFirstCheckBox->setChecked( settings->value("Battle/autoCloseFirst").toBool() );

    /*Language*/
    QString fullLocale = QLocale::system().name();
    qDebug() << "Your system locale is set to " << fullLocale << ".";
    INIT_PREF( "locale", fullLocale );
    fullLocale = settings->value( "locale" ).toString();
    int index = languageComboBox->findData( fullLocale, Qt::UserRole );
    if( index < 0 )
        index = languageComboBox->findData( fullLocale.section( '_', 0, 0 ), Qt::UserRole, Qt::MatchStartsWith );
    languageComboBox->setCurrentIndex( index );
    onLanguageChanged( index );
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
    settings->setValue("MapViewing/perspectiveProjectionType", projectionPerspectiveRadioButton->isChecked());
    settings->setValue("MapViewing/useShaders", useShadersCheckBox->isChecked());

    /*Start Positions*/
    settings->setValue("MapViewing/startPos/showOnMinimapCheckBox", startPosShowOnMinimapCheckBox->isChecked());
    settings->setValue("MapViewing/startPos/showOnHeightmapCheckBox", startPosShowOnHeightmapCheckBox->isChecked());
    settings->setValue("MapViewing/startPos/showOnMetalmapCheckBox", startPosShowOnMetalmapCheckBox->isChecked());
    settings->setValue("MapViewing/startPos/showOn3DMapCheckBox", startPosShowOn3DMapCheckBox->isChecked());
    settings->setValue("MapViewing/startPos/startRect/brushNumber", startRectBrushComboBox->currentIndex());
    settings->setValue("MapViewing/startPos/startRect/borderWidth", startRectBorderWidthSpinBox->value());
    settings->setValue("MapViewing/startPos/startRect/alpha", startRectAlphaSpinBox->value());

    /*General*/
    settings->setValue( "Battle/autoCloseFirst", battleAutoCloseFirstCheckBox->isChecked() );
    /*Language*/
    settings->setValue( "locale", languageComboBox->itemData( languageComboBox->currentIndex(), Qt::UserRole ).toString() );
}

void Preference::onCancel() {
    onResetFormToSettings();
    hide();
}

void Preference::onResetFormToSettings() {
    for ( int i = 0; i < pathElements.size(); ++i )
        pathElements[i]->resetConfiguration();
    loadPreferences();
}

void Preference::onLanguageChanged( int index ) {
    qApp->removeTranslator( translator );
    QString newLocale = languageComboBox->itemData( index, Qt::UserRole ).toString();
    if( translator->load( "qtlobby_" +  newLocale, ":/i18n/" ) )
        qApp->installTranslator( translator );
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
    // especially the unitsync should be found. (mw)
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

void Preference::setUpLanguageComboBox() {
    QStringList qmFileNames = findQmFiles();
    languageComboBox->addItem( QIcon( ":/flags/GB.xpm" ), "English", "en");
    foreach( QString qmFile, qmFileNames ) {
        translator = new QTranslator();
        QString fullLocale = qmFile.section( '.', 0, 0 ).section( '_', 1, -1 );
        translator->load( qmFile );
        languageComboBox->addItem( QIcon( ":/flags/" + translator->translate( "Preference", "GB" ) + ".xpm" ),
                                   translator->translate( "Preference", "English" ),
                                   fullLocale );
    }
    /* This has to be translated to "Deutsch" or "Русский" according to the target language.
       This variable has no other purpose than being found by lupdate to be translated. */
    QString languageName = tr( "English" );
    /* This has to be translated to "DE" or "RU" to the uppercase Top Level Domain of the country of the Locale.
       This variable has no other purpose than being found by lupdate to be translated. */
    QString languageCountryTLD = tr( "GB" );
}

QStringList Preference::findQmFiles() {
     QDir dir(":/i18n");
     QStringList fileNames = dir.entryList(QStringList("*.qm"), QDir::Files,
                                           QDir::Name);
     QMutableStringListIterator i(fileNames);
     while (i.hasNext()) {
         i.next();
         i.setValue(dir.filePath(i.value()));
     }
     return fileNames;
}

void Preference::changeEvent( QEvent* event ) {
    if( event->type() == QEvent::LanguageChange )
        retranslateUi(this);
    QDialog::changeEvent( event );
}
