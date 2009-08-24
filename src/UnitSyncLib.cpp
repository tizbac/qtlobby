// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "UnitSyncLib.h"
#include <QColor>
#include <QMutex>
#include <QMutexLocker>

#ifdef PURE_WINAPI_UNITSYNC_LOADER
#include <windows.h>
#endif

QMutex unitsync_mutex;

//for locking debugging:
//#define NON_REENTRANT qDebug() << "Locking at" << __LINE__; QMutexLocker no_reentrance_mutex_locker(&unitsync_mutex); qDebug() << "Locked";
//Normal
#define NON_REENTRANT QMutexLocker no_reentrance_mutex_locker(&unitsync_mutex);
#define MANUAL_LOCK unitsync_mutex.lock();
#define MANUAL_UNLOCK unitsync_mutex.unlock();

UnitSyncLib::UnitSyncLib( QObject *parent ) : QObject( parent ) {
    unitsynclib = new QLibrary( this );
    settings = Settings::Instance();
    library_loaded = false;

    if (!unitsynclib->isLoaded()) { // maybe this was useless after all (aj)
        if ( loadLibrary() ) {
            //     qDebug() << "library is usable";
            TestCall();
            //     QMessageBox::information( NULL, "unitSyncLib", "library loaded");
            initMapNamesCache();
            initModNamesCache();
        } else {
            qDebug() << tr("UnitSyncLibrary is not usable!");
        }
    }

    m_gameOptionKeys << "diminishingmms"
            << "disablemapdamage"
            << "fixedallies"
            << "ghostedbuildings"
            << "limitdgun"
            << "deathmode"
            << "gamemode"
            << "startpostype";
}

bool UnitSyncLib::loadLibrary() {
	//FIXME: removed locking because of reboot that already locks.
	//Probably this function will be not thread safe and if omeone
	//will be genious enough to call loadLibrary() from a thread
	//it might crash
    //NON_REENTRANT;
    QString lib_with_path = settings->value("unitsync").toString();
    QFileInfo fi( lib_with_path );

#ifdef Q_WS_WIN
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

    if (!QFile::exists(lib_with_path)) {
        QMessageBox::information( NULL, tr("unitSyncLib - library not found"),
                                  tr("Unitsync library was not found or is unusable."));
        return false;
    }
    unitsynclib->setFileName( lib_with_path );

#ifdef PURE_WINAPI_UNITSYNC_LOADER
    typedef bool (CALLBACK* SETDLLDIRECTORYA)(LPCSTR);
    HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
    SETDLLDIRECTORYA setDllDirectory = (SETDLLDIRECTORYA)GetProcAddress(kernel32, "SetDllDirectoryA");

    setDllDirectory(fi.absolutePath().toAscii().constData());
    
    HMODULE unitsynclibHandle;
    if(!(unitsynclibHandle=LoadLibraryA((char*)(fi.filePath().toAscii().constData())))){
        QMessageBox::information( NULL, tr("unitSyncLib - library not loaded"),
                                  QString( "could not load unitsync using winapi." ));
        return false;
    }
#define GetUnitsyncFunction(x) GetSpringVersion(GetProcAddress(unitsynclibHandle,x))
#else
    //   unitsynclib->setLoadHints(QLibrary::ExportExternalSymbolsHint|QLibrary::LoadArchiveMemberHint);
    unitsynclib->load();

    if ( !unitsynclib->isLoaded() ) {
        QMessageBox::information( NULL, tr("unitSyncLib - library not loaded"),
                                  QString( "%1 -- %2" )
                                  .arg( unitsynclib->errorString() )
                                  .arg( lib_with_path ) );
        return false;
    }
#define GetUnitsyncFunction(x) unitsynclib->resolve(x)
#endif

    m_GetSpringVersion  = ( GetSpringVersion ) GetUnitsyncFunction( "GetSpringVersion" );
    m_Message  = ( Message ) GetUnitsyncFunction( "Message" );
    m_UnInit  = ( UnInit ) GetUnitsyncFunction( "UnInit" );
    m_Init  = ( Init ) GetUnitsyncFunction( "Init" );
    m_InitArchiveScanner  = ( InitArchiveScanner ) GetUnitsyncFunction( "InitArchiveScanner" );
    m_ProcessUnits  = ( ProcessUnits ) GetUnitsyncFunction( "ProcessUnits" );
    m_ProcessUnitsNoChecksum  = ( ProcessUnitsNoChecksum ) GetUnitsyncFunction( "ProcessUnitsNoChecksum" );
    m_GetCurrentList  = ( GetCurrentList ) GetUnitsyncFunction( "GetCurrentList" );
    m_AddClient  = ( AddClient ) GetUnitsyncFunction( "AddClient" );
    m_RemoveClient  = ( RemoveClient ) GetUnitsyncFunction( "RemoveClient" );
    m_GetClientDiff  = ( GetClientDiff ) GetUnitsyncFunction( "GetClientDiff" );
    m_InstallClientDiff  = ( InstallClientDiff ) GetUnitsyncFunction( "InstallClientDiff" );
    m_GetUnitCount  = ( GetUnitCount ) GetUnitsyncFunction( "GetUnitCount" );
    m_GetUnitName  = ( GetUnitName ) GetUnitsyncFunction( "GetUnitName" );
    m_GetFullUnitName  = ( GetFullUnitName ) GetUnitsyncFunction( "GetFullUnitName" );
    m_IsUnitDisabled  = ( IsUnitDisabled ) GetUnitsyncFunction( "IsUnitDisabled" );
    m_IsUnitDisabledByClient  = ( IsUnitDisabledByClient ) GetUnitsyncFunction( "IsUnitDisabledByClient" );
    m_AddArchive  = ( AddArchive ) GetUnitsyncFunction( "AddArchive" );
    m_AddAllArchives  = ( AddAllArchives ) GetUnitsyncFunction( "AddAllArchives" );
    m_RemoveAllArchives  = ( RemoveAllArchives ) GetUnitsyncFunction( "RemoveAllArchives" );
    m_GetArchiveChecksum  = ( GetArchiveChecksum ) GetUnitsyncFunction( "GetArchiveChecksum" );
    m_GetArchivePath  = ( GetArchivePath ) GetUnitsyncFunction( "GetArchivePath" );
    m_GetMapCount  = ( GetMapCount ) GetUnitsyncFunction( "GetMapCount" );
    m_GetMapName  = ( GetMapName ) GetUnitsyncFunction( "GetMapName" );
    m_GetMapInfoEx  = ( GetMapInfoEx ) GetUnitsyncFunction( "GetMapInfoEx" );
    m_GetMapInfo  = ( GetMapInfo ) GetUnitsyncFunction( "GetMapInfo" );
    m_GetMapArchiveCount  = ( GetMapArchiveCount ) GetUnitsyncFunction( "GetMapArchiveCount" );
    m_GetMapArchiveName  = ( GetMapArchiveName ) GetUnitsyncFunction( "GetMapArchiveName" );
    m_GetMapChecksum  = ( GetMapChecksum ) GetUnitsyncFunction( "GetMapChecksum" );
    m_GetMapChecksumFromName  = ( GetMapChecksumFromName ) GetUnitsyncFunction( "GetMapChecksumFromName" );
    m_GetMinimap  = ( GetMinimap ) GetUnitsyncFunction( "GetMinimap" );
    m_GetInfoMapSize = ( GetInfoMapSize ) GetUnitsyncFunction( "GetInfoMapSize" );
    m_GetInfoMap = ( GetInfoMap ) GetUnitsyncFunction( "GetInfoMap" );
    m_GetPrimaryModCount  = ( GetPrimaryModCount ) GetUnitsyncFunction( "GetPrimaryModCount" );
    m_GetPrimaryModName  = ( GetPrimaryModName ) GetUnitsyncFunction( "GetPrimaryModName" );
    m_GetPrimaryModShortName  = ( GetPrimaryModShortName ) GetUnitsyncFunction( "GetPrimaryModShortName" );
    m_GetPrimaryModVersion  = ( GetPrimaryModVersion ) GetUnitsyncFunction( "GetPrimaryModVersion" );
    m_GetPrimaryModMutator  = ( GetPrimaryModMutator ) GetUnitsyncFunction( "GetPrimaryModMutator" );
    m_GetPrimaryModGame  = ( GetPrimaryModGame ) GetUnitsyncFunction( "GetPrimaryModGame" );
    m_GetPrimaryModShortGame  = ( GetPrimaryModShortGame ) GetUnitsyncFunction( "GetPrimaryModShortGame" );
    m_GetPrimaryModDescription  = ( GetPrimaryModDescription ) GetUnitsyncFunction( "GetPrimaryModDescription" );
    m_GetPrimaryModArchive  = ( GetPrimaryModArchive ) GetUnitsyncFunction( "GetPrimaryModArchive" );
    m_GetPrimaryModArchiveCount  = ( GetPrimaryModArchiveCount ) GetUnitsyncFunction( "GetPrimaryModArchiveCount" );
    m_GetPrimaryModArchiveList  = ( GetPrimaryModArchiveList ) GetUnitsyncFunction( "GetPrimaryModArchiveList" );
    m_GetPrimaryModIndex  = ( GetPrimaryModIndex ) GetUnitsyncFunction( "GetPrimaryModIndex" );
    m_GetPrimaryModChecksum  = ( GetPrimaryModChecksum ) GetUnitsyncFunction( "GetPrimaryModChecksum" );
    m_GetPrimaryModChecksumFromName  = ( GetPrimaryModChecksumFromName ) GetUnitsyncFunction( "GetPrimaryModChecksumFromName" );
    m_GetSideCount  = ( GetSideCount ) GetUnitsyncFunction( "GetSideCount" );
    m_GetSideName  = ( GetSideName ) GetUnitsyncFunction( "GetSideName" );
    m_GetLuaAICount  = ( GetLuaAICount ) GetUnitsyncFunction( "GetLuaAICount" );
    m_GetLuaAIName  = ( GetLuaAIName ) GetUnitsyncFunction( "GetLuaAIName" );
    m_GetLuaAIDesc  = ( GetLuaAIDesc ) GetUnitsyncFunction( "GetLuaAIDesc" );
    m_GetMapOptionCount  = ( GetMapOptionCount ) GetUnitsyncFunction( "GetMapOptionCount" );
    m_GetModOptionCount  = ( GetModOptionCount ) GetUnitsyncFunction( "GetModOptionCount" );
    m_GetOptionKey  = ( GetOptionKey ) GetUnitsyncFunction( "GetOptionKey" );
    m_GetOptionName  = ( GetOptionName ) GetUnitsyncFunction( "GetOptionName" );
    m_GetOptionDesc  = ( GetOptionDesc ) GetUnitsyncFunction( "GetOptionDesc" );
    m_GetOptionType  = ( GetOptionType ) GetUnitsyncFunction( "GetOptionType" );
    m_GetOptionBoolDef  = ( GetOptionBoolDef ) GetUnitsyncFunction( "GetOptionBoolDef" );
    m_GetOptionNumberDef  = ( GetOptionNumberDef ) GetUnitsyncFunction( "GetOptionNumberDef" );
    m_GetOptionNumberMin  = ( GetOptionNumberMin ) GetUnitsyncFunction( "GetOptionNumberMin" );
    m_GetOptionNumberMax  = ( GetOptionNumberMax ) GetUnitsyncFunction( "GetOptionNumberMax" );
    m_GetOptionNumberStep  = ( GetOptionNumberStep ) GetUnitsyncFunction( "GetOptionNumberStep" );
    m_GetOptionSection = ( GetOptionSection ) GetUnitsyncFunction( "GetOptionSection" );
    m_GetOptionStringDef  = ( GetOptionStringDef ) GetUnitsyncFunction( "GetOptionStringDef" );
    m_GetOptionStringMaxLen  = ( GetOptionStringMaxLen ) GetUnitsyncFunction( "GetOptionStringMaxLen" );
    m_GetOptionListCount  = ( GetOptionListCount ) GetUnitsyncFunction( "GetOptionListCount" );
    m_GetOptionListDef  = ( GetOptionListDef ) GetUnitsyncFunction( "GetOptionListDef" );
    m_GetOptionListItemKey  = ( GetOptionListItemKey ) GetUnitsyncFunction( "GetOptionListItemKey" );
    m_GetOptionListItemName  = ( GetOptionListItemName ) GetUnitsyncFunction( "GetOptionListItemName" );
    m_GetOptionListItemDesc  = ( GetOptionListItemDesc ) GetUnitsyncFunction( "GetOptionListItemDesc" );
    m_GetModValidMapCount  = ( GetModValidMapCount ) GetUnitsyncFunction( "GetModValidMapCount" );
    m_GetModValidMap  = ( GetModValidMap ) GetUnitsyncFunction( "GetModValidMap" );
    m_OpenFileVFS  = ( OpenFileVFS ) GetUnitsyncFunction( "OpenFileVFS" );
    m_CloseFileVFS  = ( CloseFileVFS ) GetUnitsyncFunction( "CloseFileVFS" );
    m_ReadFileVFS  = ( ReadFileVFS ) GetUnitsyncFunction( "ReadFileVFS" );
    m_FileSizeVFS  = ( FileSizeVFS ) GetUnitsyncFunction( "FileSizeVFS" );
    m_InitFindVFS  = ( InitFindVFS ) GetUnitsyncFunction( "InitFindVFS" );
    m_FindFilesVFS  = ( FindFilesVFS ) GetUnitsyncFunction( "FindFilesVFS" );
    m_OpenArchive  = ( OpenArchive ) GetUnitsyncFunction( "OpenArchive" );
    m_CloseArchive  = ( CloseArchive ) GetUnitsyncFunction( "CloseArchive" );
    m_FindFilesArchive  = ( FindFilesArchive ) GetUnitsyncFunction( "FindFilesArchive" );
    m_OpenArchiveFile  = ( OpenArchiveFile ) GetUnitsyncFunction( "OpenArchiveFile" );
    m_ReadArchiveFile  = ( ReadArchiveFile ) GetUnitsyncFunction( "ReadArchiveFile" );
    m_CloseArchiveFile  = ( CloseArchiveFile ) GetUnitsyncFunction( "CloseArchiveFile" );
    m_SizeArchiveFile  = ( SizeArchiveFile ) GetUnitsyncFunction( "SizeArchiveFile" );
    m_GetSpringConfigString  = ( GetSpringConfigString ) GetUnitsyncFunction( "GetSpringConfigString" );
    m_GetSpringConfigInt  = ( GetSpringConfigInt ) GetUnitsyncFunction( "GetSpringConfigInt" );
    m_GetSpringConfigFloat  = ( GetSpringConfigFloat ) GetUnitsyncFunction( "GetSpringConfigFloat" );
    m_SetSpringConfigString  = ( SetSpringConfigString ) GetUnitsyncFunction( "SetSpringConfigString" );
    m_SetSpringConfigInt  = ( SetSpringConfigInt ) GetUnitsyncFunction( "SetSpringConfigInt" );
    m_SetSpringConfigFloat  = ( SetSpringConfigFloat ) GetUnitsyncFunction( "SetSpringConfigFloat" );

    m_Init( 0, 0 );
    library_loaded = true;
#ifdef Q_WS_WIN
    /* Change current dir back to default */
    QDir::setCurrent(originaldir);
#endif
    return true;
}

UnitSyncLib::~UnitSyncLib() {
}

void UnitSyncLib::TestCall() {
    NON_REENTRANT;
    if ( !library_loaded )
        return;
    qDebug() << tr("UnitSyncLib was compiled against Spring version: ") << m_GetSpringVersion();
    int mapcount = m_GetMapCount();
    int modcount = m_GetPrimaryModCount();
    qDebug() << tr("Found %1 maps and %2 mods.").arg(mapcount).arg(modcount);
//    for ( int i = 0; i < mapcount; ++i ) {
//        unsigned int checksum = m_GetMapChecksum( i );
//        QString mapName = m_GetMapName( i );
//        qDebug() << "  " <<  i << " mapName: " << mapName << ", checksum: " << checksum;
//    }
//    for ( int i = 0; i < modcount; ++i ) {
//        qDebug() << "  " << i << " modName " << m_GetPrimaryModName( i ) << m_GetPrimaryModChecksum( i );
//        qDebug() << "        based on: " << m_GetPrimaryModArchive( i );
//    }
}

QImage UnitSyncLib::getMinimapQImage( const QString mapFileName, int miplevel, bool scaled ) {
    NON_REENTRANT;
    if ( !libraryLoaded() )
        return QImage();
    const unsigned int height = 1 << ( 10 - miplevel );
    const unsigned int width  = 1 << ( 10 - miplevel );
    QByteArray map(( char* ) m_GetMinimap( mapFileName.toAscii(), miplevel ), width*height*2 );
    QImage mapImage(( uchar* ) map.constData(), width, height, QImage::Format_RGB16 );
    if (scaled) {
        int height;
        int width;
        m_GetInfoMapSize(mapFileName.toAscii(), "height", &width, &height);
        mapImage = mapImage.scaled(width, height);
    }
    return mapImage.copy();
}

QImage UnitSyncLib::getHeightMapQImage( const QString mapFileName ) {
    NON_REENTRANT;
    if ( !libraryLoaded() )
        return QImage();
    int height;
    int width;
    const int HueLow = 210;
    const int HueHigh = 0;
    int shortMax = 65535;
    m_GetInfoMapSize(mapFileName.toAscii(), "height", &width, &height);
    unsigned short *ptr = new unsigned short[width*height];
    unsigned int *rgb = new unsigned int[width*height];
    m_GetInfoMap(mapFileName.toAscii(), "height", ptr, 2);
    for (int i = 0; i < width * height; i++) {
        rgb[i] = QColor::fromHsv(ptr[i]/(float)shortMax*(HueHigh-HueLow)+HueLow, 255, ptr[i]/(float)shortMax*155+100).rgb();
    }
    QByteArray map((char*)rgb, width*height*4);
    QImage mapImage(( uchar* ) map.constData(), width, height, QImage::Format_RGB32 );
    delete[] ptr;
    delete[] rgb;
    return mapImage.copy();
}

QImage UnitSyncLib::getGrayscaleHeightMapQImage( const QString mapFileName ) {
    NON_REENTRANT;
    if ( !libraryLoaded() )
        return QImage();
    int height;
    int width;
    int shortMax = 65535;
    m_GetInfoMapSize(mapFileName.toAscii(), "height", &width, &height);
    unsigned short *ptr = new unsigned short[width*height];
    unsigned int *rgb = new unsigned int[width*height];
    m_GetInfoMap(mapFileName.toAscii(), "height", ptr, 2);
    for (int i = 0; i < width * height; i++) {
        quint8 data[4];
        quint8 value = ptr[i]/(float)shortMax*255;
        data[0] = value;
        data[1] = value;
        data[2] = value;
        data[3] = 0;
        memcpy(rgb+i, data, sizeof(int));
    }
    QByteArray map((char*)rgb, width*height*4);
    QImage mapImage(( uchar* ) map.constData(), width, height, QImage::Format_RGB32 );
    delete[] ptr;
    delete[] rgb;
    return mapImage.copy();
}


RawHeightMap UnitSyncLib::getHeightMapRaw( const QString mapFileName ) {
    NON_REENTRANT;
    if ( !libraryLoaded() )
        return RawHeightMap(0,0,0);
    int height;
    int width;
    m_GetInfoMapSize(mapFileName.toAscii(), "height", &width, &height);
    unsigned short *ptr = new unsigned short[width*height];
    m_GetInfoMap(mapFileName.toAscii(), "height", ptr, 2);
    return RawHeightMap(width,height,ptr);
}

QImage UnitSyncLib::getMetalMapQImage( const QString mapFileName ) {
    NON_REENTRANT;
    if ( !libraryLoaded() )
        return QImage();
    int height;
    int width;
    m_GetInfoMapSize(mapFileName.toAscii(), "metal", &width, &height);
    unsigned char *ptr = new unsigned char[width*height];
    unsigned int *rgb = new unsigned int[width*height];
    m_GetInfoMap(mapFileName.toAscii(), "metal", ptr, 1);
    for (int i = 0; i < width * height; i++) {
        QColor c = QColor::fromHsv(150, 255, 255);
        c.setAlpha(ptr[i]);
        rgb[i] = c.rgba();
    }
    QByteArray map((char*)rgb, width*height*4);
    QImage mapImage(( uchar* ) map.constData(), width, height, QImage::Format_ARGB32 );
    delete[] ptr;
    delete[] rgb;
    return mapImage.copy();
}

void UnitSyncLib::getMapInfo(QString mapFileName, MapInfo* info) {
    NON_REENTRANT;
    if (!libraryLoaded()) return;
    m_GetMapInfoEx(mapFileName.toAscii(), info, 1);
    //qDebug() << "UNITSYNC_DUMP: " << "GetMapInfoEx";
}

unsigned int UnitSyncLib::mapChecksum( QString mapName ) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetMapChecksumFromName";
    return libraryLoaded() ? m_GetMapChecksumFromName( mapName.toAscii() ) : 0;
}

unsigned int UnitSyncLib::modChecksum( QString modName ) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetPrimaryModChecksumFromName";
    return libraryLoaded() ? m_GetPrimaryModChecksumFromName( modName.toAscii() ) : 0;
}

signed int UnitSyncLib::modIndex( QString modName ) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetPrimaryModInde";
    return libraryLoaded() ? m_GetPrimaryModIndex( modName.toAscii() ) : -1;
}

signed int UnitSyncLib::mapIndex( QString mapName ) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetMapArchiveCount";
    return libraryLoaded() ? m_GetMapArchiveCount( mapName.toAscii() ) : -1;
}

QString UnitSyncLib::modArchive( int modIndex ) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetPrimaryModCount";
    return libraryLoaded() && modIndex < m_GetPrimaryModCount() ? QString( m_GetPrimaryModArchive( modIndex ) ) : "";
}

bool UnitSyncLib::setCurrentMod(QString modname) {
    NON_REENTRANT;
    if (!libraryLoaded()) return false;
    if (m_currentModName == modname) return true;
    //qDebug() << "UNITSYNC_DUMP: " << "GetPrimaryModIndex";
    int index = m_GetPrimaryModIndex(modname.toAscii());
    if (index < 0) return false;
    //qDebug() << "UNITSYNC_DUMP: " << "RemoveAllArchives";
    //qDebug() << "UNITSYNC_DUMP: " << "GetPrimaryModArchive";
    //qDebug() << "UNITSYNC_DUMP: " << "AddAllArchives";

    m_RemoveAllArchives();
    m_AddAllArchives(m_GetPrimaryModArchive(index));
    m_currentModName = modname;
    m_sideIconsCache.clear();
    m_sideNamesCache.clear();
    return true;
}

QIcon UnitSyncLib::getSideIcon(QString sidename) {
    if (m_sideIconsCache.contains(sidename))
        return m_sideIconsCache[sidename];
    if (sidename.isEmpty()) return QIcon();
    NON_REENTRANT;
    // Maybe some cleaning needed here (aj)
    // Done (ko)
    if (!libraryLoaded()) return QIcon();

    QString filepath = QString("SidePics/%1.bmp").arg(sidename.toUpper());
    //qDebug() << "UNITSYNC_DUMP: " << "OpenFileVFS";
    int ret = m_OpenFileVFS(filepath.toAscii());
    if (ret < 0) return QIcon();
    //qDebug() << "UNITSYNC_DUMP: " << "FileSizeVFS";
    int filesize = m_FileSizeVFS(ret);
    if (filesize == 0) return QIcon();

    QByteArray sideicon;
    sideicon.resize(filesize);
    //qDebug() << "UNITSYNC_DUMP: " << "ReadFileVFS";
    m_ReadFileVFS(ret, sideicon.data(), filesize);

    QPixmap rawicon;
    rawicon.loadFromData(sideicon);
    QIcon ico = QIcon(rawicon);
    m_sideIconsCache[sidename] = ico;
    return ico;
}

QString UnitSyncLib::getSpringVersion() {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetSpringVersion";
    return libraryLoaded() ? QString(m_GetSpringVersion()) : "";
}

int UnitSyncLib::getSideNameCount() {
    if (m_currentModName.isEmpty()) return 0;
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetSideCount";
    return libraryLoaded() ? m_GetSideCount( ) : -1;
}

QString UnitSyncLib::sideName( int index ) {
    if (m_sideNamesCache.contains(index))
        return m_sideNamesCache[index];
    if (m_currentModName.isEmpty()) return QString();
    NON_REENTRANT;
    if ( libraryLoaded() ) {
        //qDebug() << "UNITSYNC_DUMP: " << "GetSideCount";
        if ( m_GetSideCount(  ) > index ) {
            //qDebug() << "UNITSYNC_DUMP: " << "GetSideName";
            QString name = m_GetSideName( index );
            m_sideNamesCache[index] = name;
            return name;
        }
    }
    return "";
}

bool UnitSyncLib::libraryLoaded() {
    if ( !library_loaded ) {
        qDebug() << tr("Your library is not loaded, you can't use it!");
        return false;
    }
    return true;
}

int UnitSyncLib::getModOptionCount() {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetModOptionCount";
    return m_GetModOptionCount();
}

OptionType UnitSyncLib::getOptionType(int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionType";
    int type = m_GetOptionType(optIndex);
    switch (type) {
    case 1:
        return OT_BOOLEAN;
    case 2:
        return OT_LIST;
    case 3:
        return OT_FLOAT;
    case 4:
        return OT_STRING;
    case 5:
        return OT_SECTION;
    default:
        return OT_UNDEFINED;
    }
}

QString UnitSyncLib::getOptionName(int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionName";
    return m_GetOptionName(optIndex);
}

QString UnitSyncLib::getOptionSection(int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionSection";
    return m_GetOptionSection(optIndex);
}

QString UnitSyncLib::getOptionKey(int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionKey";
    return m_GetOptionKey(optIndex);
}

bool UnitSyncLib::isGameOption(int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionKey";
    return m_gameOptionKeys.contains(m_GetOptionKey(optIndex));
}

bool UnitSyncLib::isGameOption(QString key) {
    return m_gameOptionKeys.contains(key);
}

bool UnitSyncLib::getOptionBoolDef(int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionBoolDef";
    return m_GetOptionBoolDef(optIndex);
}

QString UnitSyncLib::getOptionListDef(int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionListDef";
    return m_GetOptionListDef(optIndex);
}

float UnitSyncLib::getOptionNumberDef(int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionNumberDef";
    return m_GetOptionNumberDef(optIndex);
}

float UnitSyncLib::getOptionNumberMin (int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionNumberMin";
    return m_GetOptionNumberMin(optIndex);
}

float UnitSyncLib::getOptionNumberMax (int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionNumberMax";
    return m_GetOptionNumberMax(optIndex);
}

float UnitSyncLib::getOptionNumberStep (int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionNumberStep";
    return m_GetOptionNumberStep(optIndex);
}

QString UnitSyncLib::getOptionStringDef(int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionStringDef";
    return m_GetOptionStringDef(optIndex);
}

int UnitSyncLib::getOptionStringMaxLen(int optIndex) {
    NON_REENTRANT;
    //qDebug() << "UNITSYNC_DUMP: " << "GetOptionStringMaxLen";
    return m_GetOptionStringMaxLen(optIndex);
}

void UnitSyncLib::reboot() {
    MANUAL_LOCK;
    if (library_loaded) {
        //qDebug() << "UNITSYNC_DUMP: " << "UnInit";
        m_RemoveAllArchives();
        //qDebug() << "UNITSYNC_DUMP: " << "RemoveAllArchives";
        m_UnInit();
		unitsynclib->unload();
		library_loaded = false;
		loadLibrary();
        //qDebug() << "UNITSYNC_DUMP: " << "GetPrimaryModIndex";
        int index = m_GetPrimaryModIndex(m_currentModName.toAscii());
        if (index >= 0) {
            //qDebug() << "UNITSYNC_DUMP: " << "GetPrimaryModArchive";
            //qDebug() << "UNITSYNC_DUMP: " << "AddAllArchives";
            m_AddAllArchives(m_GetPrimaryModArchive(index));
        }
        MANUAL_UNLOCK;
        initMapNamesCache();
        initModNamesCache();
        emit rebooted();
    }
}

void UnitSyncLib::initMapNamesCache() {
    m_mapNamesCache.clear();
    if( library_loaded ) {
        int mapcount = m_GetMapCount();
        for ( int i = 0; i < mapcount; ++i ) {
            m_mapNamesCache << m_GetMapName(i);
        }
    }
}

void UnitSyncLib::initModNamesCache() {
    m_modNamesCache.clear();
    if( library_loaded ) {
        int modcount = m_GetPrimaryModCount();
        for ( int i = 0; i < modcount; ++i ) {
            m_modNamesCache << m_GetPrimaryModName(i);
        }
    }
}

QSet<QString> UnitSyncLib::getModNames() {
    return m_modNamesCache;
}

QSet<QString> UnitSyncLib::getMapNames() {
    return m_mapNamesCache;
}

QStringList UnitSyncLib::getOptionListItems(int optIndex) {
    QStringList ret;
    if (library_loaded) {
        int count = m_GetOptionListCount(optIndex);
        for(int i = 0; i < count; i++) {
            ret << m_GetOptionListItemKey(i, optIndex);
        }
    }
    return ret;
}
