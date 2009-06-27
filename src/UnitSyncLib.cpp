//
// C++ Implementation: UnitSyncLib
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "UnitSyncLib.h"

UnitSyncLib::UnitSyncLib( QObject *parent ) : QObject( parent ) {
    unitsynclib = new QLibrary( this );
    settings = Settings::Instance();
    library_loaded = false;
    QString libstring = settings->value("unitsync").toString();

    if(!unitsynclib->isLoaded()) // maybe this was useless after all (aj)
    {
        if ( loadLibrary( libstring ) ) {
            //     qDebug() << "library is usable";
            TestCall();
            //     QMessageBox::information( NULL, "unitSyncLib", "library loaded");
        } else {
            qDebug() << "library is not usable!";
        }
    }

    m_gameOptionKeys << "diminishingmms"
            << "disablemapdamage"
            << "fixedallies"
            << "ghostedbuildings"
            << "limitdgun"
            << "deathmode"
            << "gamemode";
}

bool UnitSyncLib::loadLibrary( QString lib_with_path ) {
    QFileInfo fi( lib_with_path );

#ifdef WIN32
    /*  Change current dir cause otherwise windows cant load unitsync (aj) */
    QString originaldir = QDir::currentPath();
    QDir::setCurrent(fi.absolutePath());
#endif

    /* the next line is very important to load/find the shared libraries used by
     the shared library we are using, namely unitsync.dll (js)*/
    QApplication::instance()->addLibraryPath(fi.absolutePath());
    QApplication::instance()->addLibraryPath(".");

    //   QMessageBox::information( NULL, "unitSyncLib", QString("%1")
    //       .arg(fi.absolutePath()));

    unitsynclib->setFileName( lib_with_path );
    //   unitsynclib->setLoadHints(QLibrary::ExportExternalSymbolsHint|QLibrary::LoadArchiveMemberHint);
    unitsynclib->load();

    if ( !unitsynclib->isLoaded() ){
        QMessageBox::information( NULL, "unitSyncLib - library not loaded",
                                  QString( "%1 -- %2" )
                                  .arg( unitsynclib->errorString() )
                                  .arg( lib_with_path ) );
        return false;
    }

#ifdef WIN32
    /* Change current dir back to default */
    QDir::setCurrent(originaldir);
#endif

    m_GetSpringVersion  = ( GetSpringVersion ) unitsynclib->resolve( "GetSpringVersion" );
    m_Message  = ( Message ) unitsynclib->resolve( "Message" );
    m_UnInit  = ( UnInit ) unitsynclib->resolve( "UnInit" );
    m_Init  = ( Init ) unitsynclib->resolve( "Init" );
    m_InitArchiveScanner  = ( InitArchiveScanner ) unitsynclib->resolve( "InitArchiveScanner" );
    m_ProcessUnits  = ( ProcessUnits ) unitsynclib->resolve( "ProcessUnits" );
    m_ProcessUnitsNoChecksum  = ( ProcessUnitsNoChecksum ) unitsynclib->resolve( "ProcessUnitsNoChecksum" );
    m_GetCurrentList  = ( GetCurrentList ) unitsynclib->resolve( "GetCurrentList" );
    m_AddClient  = ( AddClient ) unitsynclib->resolve( "AddClient" );
    m_RemoveClient  = ( RemoveClient ) unitsynclib->resolve( "RemoveClient" );
    m_GetClientDiff  = ( GetClientDiff ) unitsynclib->resolve( "GetClientDiff" );
    m_InstallClientDiff  = ( InstallClientDiff ) unitsynclib->resolve( "InstallClientDiff" );
    m_GetUnitCount  = ( GetUnitCount ) unitsynclib->resolve( "GetUnitCount" );
    m_GetUnitName  = ( GetUnitName ) unitsynclib->resolve( "GetUnitName" );
    m_GetFullUnitName  = ( GetFullUnitName ) unitsynclib->resolve( "GetFullUnitName" );
    m_IsUnitDisabled  = ( IsUnitDisabled ) unitsynclib->resolve( "IsUnitDisabled" );
    m_IsUnitDisabledByClient  = ( IsUnitDisabledByClient ) unitsynclib->resolve( "IsUnitDisabledByClient" );
    m_AddArchive  = ( AddArchive ) unitsynclib->resolve( "AddArchive" );
    m_AddAllArchives  = ( AddAllArchives ) unitsynclib->resolve( "AddAllArchives" );
    m_GetArchiveChecksum  = ( GetArchiveChecksum ) unitsynclib->resolve( "GetArchiveChecksum" );
    m_GetArchivePath  = ( GetArchivePath ) unitsynclib->resolve( "GetArchivePath" );
    m_GetMapCount  = ( GetMapCount ) unitsynclib->resolve( "GetMapCount" );
    m_GetMapName  = ( GetMapName ) unitsynclib->resolve( "GetMapName" );
    m_GetMapInfoEx  = ( GetMapInfoEx ) unitsynclib->resolve( "GetMapInfoEx" );
    m_GetMapInfo  = ( GetMapInfo ) unitsynclib->resolve( "GetMapInfo" );
    m_GetMapArchiveCount  = ( GetMapArchiveCount ) unitsynclib->resolve( "GetMapArchiveCount" );
    m_GetMapArchiveName  = ( GetMapArchiveName ) unitsynclib->resolve( "GetMapArchiveName" );
    m_GetMapChecksum  = ( GetMapChecksum ) unitsynclib->resolve( "GetMapChecksum" );
    m_GetMapChecksumFromName  = ( GetMapChecksumFromName ) unitsynclib->resolve( "GetMapChecksumFromName" );
    m_GetMinimap  = ( GetMinimap ) unitsynclib->resolve( "GetMinimap" );
    m_GetPrimaryModCount  = ( GetPrimaryModCount ) unitsynclib->resolve( "GetPrimaryModCount" );
    m_GetPrimaryModName  = ( GetPrimaryModName ) unitsynclib->resolve( "GetPrimaryModName" );
    m_GetPrimaryModShortName  = ( GetPrimaryModShortName ) unitsynclib->resolve( "GetPrimaryModShortName" );
    m_GetPrimaryModVersion  = ( GetPrimaryModVersion ) unitsynclib->resolve( "GetPrimaryModVersion" );
    m_GetPrimaryModMutator  = ( GetPrimaryModMutator ) unitsynclib->resolve( "GetPrimaryModMutator" );
    m_GetPrimaryModGame  = ( GetPrimaryModGame ) unitsynclib->resolve( "GetPrimaryModGame" );
    m_GetPrimaryModShortGame  = ( GetPrimaryModShortGame ) unitsynclib->resolve( "GetPrimaryModShortGame" );
    m_GetPrimaryModDescription  = ( GetPrimaryModDescription ) unitsynclib->resolve( "GetPrimaryModDescription" );
    m_GetPrimaryModArchive  = ( GetPrimaryModArchive ) unitsynclib->resolve( "GetPrimaryModArchive" );
    m_GetPrimaryModArchiveCount  = ( GetPrimaryModArchiveCount ) unitsynclib->resolve( "GetPrimaryModArchiveCount" );
    m_GetPrimaryModArchiveList  = ( GetPrimaryModArchiveList ) unitsynclib->resolve( "GetPrimaryModArchiveList" );
    m_GetPrimaryModIndex  = ( GetPrimaryModIndex ) unitsynclib->resolve( "GetPrimaryModIndex" );
    m_GetPrimaryModChecksum  = ( GetPrimaryModChecksum ) unitsynclib->resolve( "GetPrimaryModChecksum" );
    m_GetPrimaryModChecksumFromName  = ( GetPrimaryModChecksumFromName ) unitsynclib->resolve( "GetPrimaryModChecksumFromName" );
    m_GetSideCount  = ( GetSideCount ) unitsynclib->resolve( "GetSideCount" );
    m_GetSideName  = ( GetSideName ) unitsynclib->resolve( "GetSideName" );
    m_GetLuaAICount  = ( GetLuaAICount ) unitsynclib->resolve( "GetLuaAICount" );
    m_GetLuaAIName  = ( GetLuaAIName ) unitsynclib->resolve( "GetLuaAIName" );
    m_GetLuaAIDesc  = ( GetLuaAIDesc ) unitsynclib->resolve( "GetLuaAIDesc" );
    m_GetMapOptionCount  = ( GetMapOptionCount ) unitsynclib->resolve( "GetMapOptionCount" );
    m_GetModOptionCount  = ( GetModOptionCount ) unitsynclib->resolve( "GetModOptionCount" );
    m_GetOptionKey  = ( GetOptionKey ) unitsynclib->resolve( "GetOptionKey" );
    m_GetOptionName  = ( GetOptionName ) unitsynclib->resolve( "GetOptionName" );
    m_GetOptionDesc  = ( GetOptionDesc ) unitsynclib->resolve( "GetOptionDesc" );
    m_GetOptionType  = ( GetOptionType ) unitsynclib->resolve( "GetOptionType" );
    m_GetOptionBoolDef  = ( GetOptionBoolDef ) unitsynclib->resolve( "GetOptionBoolDef" );
    m_GetOptionNumberDef  = ( GetOptionNumberDef ) unitsynclib->resolve( "GetOptionNumberDef" );
    m_GetOptionNumberMin  = ( GetOptionNumberMin ) unitsynclib->resolve( "GetOptionNumberMin" );
    m_GetOptionNumberMax  = ( GetOptionNumberMax ) unitsynclib->resolve( "GetOptionNumberMax" );
    m_GetOptionNumberStep  = ( GetOptionNumberStep ) unitsynclib->resolve( "GetOptionNumberStep" );
    m_GetOptionSection = ( GetOptionSection ) unitsynclib->resolve( "GetOptionSection" );
    m_GetOptionStringDef  = ( GetOptionStringDef ) unitsynclib->resolve( "GetOptionStringDef" );
    m_GetOptionStringMaxLen  = ( GetOptionStringMaxLen ) unitsynclib->resolve( "GetOptionStringMaxLen" );
    m_GetOptionListCount  = ( GetOptionListCount ) unitsynclib->resolve( "GetOptionListCount" );
    m_GetOptionListDef  = ( GetOptionListDef ) unitsynclib->resolve( "GetOptionListDef" );
    m_GetOptionListItemKey  = ( GetOptionListItemKey ) unitsynclib->resolve( "GetOptionListItemKey" );
    m_GetOptionListItemName  = ( GetOptionListItemName ) unitsynclib->resolve( "GetOptionListItemName" );
    m_GetOptionListItemDesc  = ( GetOptionListItemDesc ) unitsynclib->resolve( "GetOptionListItemDesc" );
    m_GetModValidMapCount  = ( GetModValidMapCount ) unitsynclib->resolve( "GetModValidMapCount" );
    m_GetModValidMap  = ( GetModValidMap ) unitsynclib->resolve( "GetModValidMap" );
    m_OpenFileVFS  = ( OpenFileVFS ) unitsynclib->resolve( "OpenFileVFS" );
    m_CloseFileVFS  = ( CloseFileVFS ) unitsynclib->resolve( "CloseFileVFS" );
    m_ReadFileVFS  = ( ReadFileVFS ) unitsynclib->resolve( "ReadFileVFS" );
    m_FileSizeVFS  = ( FileSizeVFS ) unitsynclib->resolve( "FileSizeVFS" );
    m_InitFindVFS  = ( InitFindVFS ) unitsynclib->resolve( "InitFindVFS" );
    m_FindFilesVFS  = ( FindFilesVFS ) unitsynclib->resolve( "FindFilesVFS" );
    m_OpenArchive  = ( OpenArchive ) unitsynclib->resolve( "OpenArchive" );
    m_CloseArchive  = ( CloseArchive ) unitsynclib->resolve( "CloseArchive" );
    m_FindFilesArchive  = ( FindFilesArchive ) unitsynclib->resolve( "FindFilesArchive" );
    m_OpenArchiveFile  = ( OpenArchiveFile ) unitsynclib->resolve( "OpenArchiveFile" );
    m_ReadArchiveFile  = ( ReadArchiveFile ) unitsynclib->resolve( "ReadArchiveFile" );
    m_CloseArchiveFile  = ( CloseArchiveFile ) unitsynclib->resolve( "CloseArchiveFile" );
    m_SizeArchiveFile  = ( SizeArchiveFile ) unitsynclib->resolve( "SizeArchiveFile" );
    m_GetSpringConfigString  = ( GetSpringConfigString ) unitsynclib->resolve( "GetSpringConfigString" );
    m_GetSpringConfigInt  = ( GetSpringConfigInt ) unitsynclib->resolve( "GetSpringConfigInt" );
    m_GetSpringConfigFloat  = ( GetSpringConfigFloat ) unitsynclib->resolve( "GetSpringConfigFloat" );
    m_SetSpringConfigString  = ( SetSpringConfigString ) unitsynclib->resolve( "SetSpringConfigString" );
    m_SetSpringConfigInt  = ( SetSpringConfigInt ) unitsynclib->resolve( "SetSpringConfigInt" );
    m_SetSpringConfigFloat  = ( SetSpringConfigFloat ) unitsynclib->resolve( "SetSpringConfigFloat" );

    m_Init( 0, 0 );
    library_loaded = true;
    return true;
}

UnitSyncLib::~UnitSyncLib() {
}

void UnitSyncLib::TestCall() {
    if ( !library_loaded )
        return;

    //   qDebug() << mapChecksum("Victoria Crater.smf");
    //   qDebug() << modChecksum("BA61.sd7");
    qDebug() << "Unitsync was compiled against Spring version: " << m_GetSpringVersion();
    int mapcount = m_GetMapCount();
    qDebug() << "Found " << mapcount << " maps";
    for ( int i = 0; i < mapcount; ++i ) {
        unsigned int checksum = m_GetMapChecksum( i );
        QString mapName = m_GetMapName( i );
        qDebug() << "  " <<  i << " mapName: " << mapName << ", checksum: " << checksum;
    }
    int modcount = m_GetPrimaryModCount();
    qDebug() << "Found " << modcount << " mods";
    for ( int i = 0; i < modcount; ++i ) {
        qDebug() << "  " << i << " modName " << m_GetPrimaryModName( i ) << m_GetPrimaryModChecksum( i );
        qDebug() << "        based on: " << m_GetPrimaryModArchive( i );
    }

    //   example to get the checksum of a mod by name
}

QImage UnitSyncLib::GetMinimapQImage( const QString mapFileName, int miplevel ) {
    // FIXME this code produces some artefacts in the graphics which isn't nice (js)
    if ( !libraryLoaded() )
        return QImage();
    const unsigned int height = 1 << ( 10 - miplevel );
    const unsigned int width  = 1 << ( 10 - miplevel );

    //   unsigned short* colours = (unsigned short*) m_GetMinimap( mapFileName.toUtf8(), 0 );
    //   char* true_colours = ( char* ) malloc( width * height * 3 );
    //   for ( int i = 0; i < width*height; i++ ) {
    //     true_colours[( i*3 )] = char((( colours[i] >> 11 ) / 31.0 ) * 255.0 );
    //     true_colours[( i*3 )+1] = char(((( colours[i] >> 5 ) & 63 ) / 63.0 ) * 255.0 );
    //     true_colours[( i*3 )+2] = char((( colours[i] & 31 ) / 31.0 ) * 255.0 );
    //   }
    //   QByteArray map( true_colours, width*height*3 );

    QByteArray map(( char* ) m_GetMinimap( mapFileName.toUtf8(), miplevel ), width*height*3 );

    QImage mapImage(( uchar* ) map.constData(), width, height, QImage::Format_RGB16 );
    return mapImage.copy();
}

unsigned int UnitSyncLib::mapChecksum( QString mapName ) {
    return libraryLoaded() ? m_GetMapChecksumFromName( mapName.toAscii() ) : 0;
}

unsigned int UnitSyncLib::modChecksum( QString modName ) {
    return libraryLoaded() ? m_GetPrimaryModChecksumFromName( modName.toAscii() ) : 0;
}

unsigned int UnitSyncLib::modIndex( QString modName ) {
    qDebug() << "modName: " << modName;
    return libraryLoaded() ? m_GetPrimaryModIndex( modName.toAscii() ) : -1;
}

unsigned int UnitSyncLib::mapIndex( QString mapName ) {
    //   qDebug() << "mapName: " << mapName;
    return libraryLoaded() ? m_GetMapArchiveCount( mapName.toAscii() ) : -1;
}

QString UnitSyncLib::modArchive( int modIndex ) {
    qDebug() << "modIndex: " << modIndex;
    return libraryLoaded() && modIndex < m_GetPrimaryModCount() ? QString( m_GetPrimaryModArchive( modIndex ) ) : "";
}

bool UnitSyncLib::setCurrentMod(QString modname) {
    if(!libraryLoaded()) return false;
    if(m_currentModName == modname) return true;
    int index = m_GetPrimaryModIndex(modname.toStdString().c_str());
    if(index < 0) return false;
    m_AddAllArchives(m_GetPrimaryModArchive(index));
    m_currentModName = modname;
    return true;
}

QString UnitSyncLib::getSpringVersion() {
    return libraryLoaded() ? QString(m_GetSpringVersion()) : "";
}

QString UnitSyncLib::sideName( QString modName, int index ) {
    if ( libraryLoaded() ) {
        m_AddAllArchives( m_GetPrimaryModArchive( m_GetPrimaryModIndex( modName.toAscii().constData() ) ) );
        if ( m_GetSideCount( modName.toAscii().constData() ) > index )
            return QString( m_GetSideName( index ) );
    }
    return "";
}

bool UnitSyncLib::libraryLoaded() {
    if ( !library_loaded ) {
        qDebug() << "Your library is not loaded, you can't use it!";
        return false;
    }
    return true;
}

int UnitSyncLib::getModOptionCount() {
    return m_GetModOptionCount();
}

OptionType UnitSyncLib::getOptionType(int optIndex) {
    int type = m_GetOptionType(optIndex);
    switch(type) {
    case 1: return BOOLEAN;
    case 2: return LIST;
    case 3: return FLOAT;
    case 4: return STRING;
    case 5: return SECTION;
    default: return UNDEFINED;
    }
}

QString UnitSyncLib::getOptionName(int optIndex) {
    return m_GetOptionName(optIndex);
}

QString UnitSyncLib::getOptionSection(int optIndex) {
    return m_GetOptionSection(optIndex);
}

QString UnitSyncLib::getOptionKey(int optIndex) {
    return m_GetOptionKey(optIndex);
}

bool UnitSyncLib::isGameOption(int optIndex) {
    return m_gameOptionKeys.contains(m_GetOptionKey(optIndex));
}

bool UnitSyncLib::getOptionBoolDef(int optIndex){
    return m_GetOptionBoolDef(optIndex);
}

QString UnitSyncLib::getOptionListDef(int optIndex){
    return m_GetOptionListDef(optIndex);
}

float UnitSyncLib::getOptionNumberDef(int optIndex){
    return m_GetOptionNumberDef(optIndex);
}

QString UnitSyncLib::getOptionStringDef(int optIndex){
    return m_GetOptionStringDef(optIndex);
}

