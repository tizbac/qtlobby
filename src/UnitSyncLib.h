//
// C++ Interface: UnitSyncLib
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef UNITSYNCLIB_H
#define UNITSYNCLIB_H

#include <QApplication>
#include <QObject>
#include <QLibrary>
#include <QDebug>
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QImage>
#include <QByteArray>
#include <QFileInfo>
#include <QDir>
#include <QIcon>

#include <Settings.h>

// ONLY FOR TESTING PLEASE REMOVE WHEN NOT USED ANYMORE
#include <QWidget>
#include <QLabel>
#include <QPixmap>

#include <math.h>
#include <QMessageBox>

#include "Singleton.h"
#include "RawHeightMap.h"

/**
 @author Joachim Schiele <js@lastlog.de>
*/

enum OptionType {
    UNDEFINED  = 0,
    BOOLEAN    = 1,
    LIST       = 2,
    FLOAT      = 3,
    STRING     = 4,
    SECTION    = 5
             };


struct StartPos {
    int x;
    int z;
};

struct MapInfo {
    char* description;
    int tidalStrength;
    int gravity;
    float maxMetal;
    int extractorRadius;
    int minWind;
    int maxWind;

    // 0.61b1+
    int width;
    int height;
    int posCount;
    StartPos positions[16];

    // VERSION>=1
    char* author; // max 200 chars
};

class UnitSyncLib : public QObject, public Singleton<UnitSyncLib> {
    Q_OBJECT
    friend class Singleton<UnitSyncLib>;
public:
    QImage getMinimapQImage( const QString, int miplevel = 0, bool scaled = true );
    QImage getHeightMapQImage( const QString mapFileName );
    RawHeightMap getHeightMapRaw( const QString mapFileName );
    QImage getMetalMapQImage( const QString mapFileName );
    void getMapInfo(QString mapFileName, MapInfo* info);
    void TestCall();
    bool loadLibrary( QString path );
    bool library_loaded;
    bool libraryLoaded();
    unsigned int mapChecksum( QString mapname );
    unsigned int modChecksum( QString mapname );
    signed int modIndex( QString modName );
    QString modArchive( int modIndex );
    QString sideName( QString modName, int modIndex );
    signed int mapIndex( QString mapName );
    QLibrary* unitsynclib;
    bool setCurrentMod(QString modname);
    QString getSpringVersion();
    int getModOptionCount();
    OptionType getOptionType(int optIndex);
    QString getOptionName(int optIndex);
    QString getOptionSection(int optIndex);
    QString getOptionKey(int optIndex);
    bool isGameOption(int optIndex);
    bool getOptionBoolDef(int optIndex);
    QString getOptionListDef(int optIndex);
    float getOptionNumberDef(int optIndex);
    QString getOptionStringDef(int optIndex);

    //New
    int getSideNameCount(QString modname);
    QIcon getSideIcon(QString modname, QString sidename);

private:
    QSettings* settings;
    QString m_currentModName;
    QStringList m_gameOptionKeys;
    UnitSyncLib( QObject *parent = 0 );
    ~UnitSyncLib();
    typedef const char*( * GetSpringVersion )();
    typedef void ( * Message )( const char* p_szMessage );
    typedef void ( * UnInit )();
    typedef int ( * Init )( bool isServer, int id );
    typedef int ( * InitArchiveScanner )( void );
    typedef int ( * ProcessUnits )( void );
    typedef int ( * ProcessUnitsNoChecksum )( void );
    typedef const char * ( * GetCurrentList )();
    typedef void ( * AddClient )( int id, const char *unitList );
    typedef void ( * RemoveClient )( int id );
    typedef const char * ( * GetClientDiff )( int id );
    typedef void ( * InstallClientDiff )( const char *diff );
    typedef int ( * GetUnitCount )();
    typedef const char * ( * GetUnitName )( int unit );
    typedef const char * ( * GetFullUnitName )( int unit );
    typedef int ( * IsUnitDisabled )( int unit );
    typedef int ( * IsUnitDisabledByClient )( int unit, int clientId );
    typedef void ( * AddArchive )( const char* name );
    typedef void ( * AddAllArchives )( const char* root );
    typedef unsigned int ( * GetArchiveChecksum )( const char* arname );
    typedef const char*( * GetArchivePath )( const char* arname );
    typedef int ( * GetMapCount )();
    typedef const char*( * GetMapName )( int index );
    typedef int ( * GetMapInfoEx )( const char* name, MapInfo* outInfo, int version );
    typedef int ( * GetMapInfo )( const char* name, MapInfo* outInfo );
    typedef int ( * GetMapArchiveCount )( const char* mapName );
    typedef const char*( * GetMapArchiveName )( int index );
    typedef unsigned int ( * GetMapChecksum )( int index );
    typedef unsigned int ( * GetMapChecksumFromName )( const char* mapName );
    typedef void* ( * GetMinimap )( const char* filename, int miplevel );
    typedef int ( * GetInfoMapSize )( const char * filename, const char * name, int * width, int * height);
    typedef int ( * GetInfoMap )( const char * filename, const char * name, void * data, int typeHint);
    typedef int ( * GetPrimaryModCount )();
    typedef const char*( * GetPrimaryModName )( int index );
    typedef const char*( * GetPrimaryModShortName )( int index );
    typedef const char*( * GetPrimaryModVersion )( int index );
    typedef const char*( * GetPrimaryModMutator )( int index );
    typedef const char*( * GetPrimaryModGame )( int index );
    typedef const char*( * GetPrimaryModShortGame )( int index );
    typedef const char*( * GetPrimaryModDescription )( int index );
    typedef const char*( * GetPrimaryModArchive )( int index );
    typedef int ( * GetPrimaryModArchiveCount )( int index );
    typedef const char*( * GetPrimaryModArchiveList )( int arnr );
    typedef int ( * GetPrimaryModIndex )( const char* name );
    typedef unsigned int ( * GetPrimaryModChecksum )( int index );
    typedef unsigned int ( * GetPrimaryModChecksumFromName )( const char* name );
    typedef int ( * GetSideCount )( const char* modName );
    typedef const char*( * GetSideName )( int side );
    typedef int ( * GetLuaAICount )();
    typedef const char*( * GetLuaAIName )( int aiIndex );
    typedef const char*( * GetLuaAIDesc )( int aiIndex );
    typedef int ( * GetMapOptionCount )( const char* name );
    typedef int ( * GetModOptionCount )();
    typedef const char*( * GetOptionKey )( int optIndex );
    typedef const char*( * GetOptionName )( int optIndex );
    typedef const char*( * GetOptionDesc )( int optIndex );
    typedef int ( * GetOptionType )( int optIndex );
    typedef int ( * GetOptionBoolDef )( int optIndex );
    typedef float( * GetOptionNumberDef )( int optIndex );
    typedef float( * GetOptionNumberMin )( int optIndex );
    typedef float( * GetOptionNumberMax )( int optIndex );
    typedef float( * GetOptionNumberStep )( int optIndex );
    typedef const char*( * GetOptionSection )( int optIndex );
    typedef const char*( * GetOptionStringDef )( int optIndex );
    typedef int ( * GetOptionStringMaxLen )( int optIndex );
    typedef int ( * GetOptionListCount )( int optIndex );
    typedef const char*( * GetOptionListDef )( int optIndex );
    typedef const char*( * GetOptionListItemKey )( int optIndex, int itemIndex );
    typedef const char*( * GetOptionListItemName )( int optIndex, int itemIndex );
    typedef const char*( * GetOptionListItemDesc )( int optIndex, int itemIndex );
    typedef int ( * GetModValidMapCount )();
    typedef const char*( * GetModValidMap )( int index );
    typedef int ( * OpenFileVFS )( const char* name );
    typedef void ( * CloseFileVFS )( int handle );
    typedef void ( * ReadFileVFS )( int handle, void* buf, int length );
    typedef int ( * FileSizeVFS )( int handle );
    typedef int ( * InitFindVFS )( const char* pattern );
    typedef int ( * FindFilesVFS )( int handle, char* nameBuf, int size );
    typedef int ( * OpenArchive )( const char* name );
    typedef void ( * CloseArchive )( int archive );
    typedef int ( * FindFilesArchive )( int archive, int cur, char* nameBuf, int* size );
    typedef int ( * OpenArchiveFile )( int archive, const char* name );
    typedef int ( * ReadArchiveFile )( int archive, int handle, void* buffer, int numBytes );
    typedef void ( * CloseArchiveFile )( int archive, int handle );
    typedef int ( * SizeArchiveFile )( int archive, int handle );
    typedef const char*( * GetSpringConfigString )( const char* name, const char* defvalue );
    typedef int ( * GetSpringConfigInt )( const char* name, const int defvalue );
    typedef float( * GetSpringConfigFloat )( const char* name, const float defvalue );
    typedef void ( * SetSpringConfigString )( const char* name, const char* value );
    typedef void ( * SetSpringConfigInt )( const char* name, const int value );
    typedef void ( * SetSpringConfigFloat )( const char* name, const float value );
    GetSpringVersion m_GetSpringVersion;
    Message m_Message;
    UnInit m_UnInit;
    Init m_Init;
    InitArchiveScanner m_InitArchiveScanner;
    ProcessUnits m_ProcessUnits;
    ProcessUnitsNoChecksum m_ProcessUnitsNoChecksum;
    GetCurrentList m_GetCurrentList;
    AddClient m_AddClient;
    RemoveClient m_RemoveClient;
    GetClientDiff m_GetClientDiff;
    InstallClientDiff m_InstallClientDiff;
    GetUnitCount m_GetUnitCount;
    GetUnitName m_GetUnitName;
    GetFullUnitName m_GetFullUnitName;
    IsUnitDisabled m_IsUnitDisabled;
    IsUnitDisabledByClient m_IsUnitDisabledByClient;
    AddArchive m_AddArchive;
    AddAllArchives m_AddAllArchives;
    GetArchiveChecksum m_GetArchiveChecksum;
    GetArchivePath m_GetArchivePath;
    GetMapCount m_GetMapCount;
    GetMapName m_GetMapName;
    GetMapInfoEx m_GetMapInfoEx;
    GetMapInfo m_GetMapInfo;
    GetMapArchiveCount m_GetMapArchiveCount;
    GetMapArchiveName m_GetMapArchiveName;
    GetMapChecksum m_GetMapChecksum;
    GetMapChecksumFromName m_GetMapChecksumFromName;
    GetMinimap m_GetMinimap;
    GetInfoMapSize m_GetInfoMapSize;
    GetInfoMap m_GetInfoMap;
    GetPrimaryModCount m_GetPrimaryModCount;
    GetPrimaryModName m_GetPrimaryModName;
    GetPrimaryModShortName m_GetPrimaryModShortName;
    GetPrimaryModVersion m_GetPrimaryModVersion;
    GetPrimaryModMutator m_GetPrimaryModMutator;
    GetPrimaryModGame m_GetPrimaryModGame;
    GetPrimaryModShortGame m_GetPrimaryModShortGame;
    GetPrimaryModDescription m_GetPrimaryModDescription;
    GetPrimaryModArchive m_GetPrimaryModArchive;
    GetPrimaryModArchiveCount m_GetPrimaryModArchiveCount;
    GetPrimaryModArchiveList m_GetPrimaryModArchiveList;
    GetPrimaryModIndex m_GetPrimaryModIndex;
    GetPrimaryModChecksum m_GetPrimaryModChecksum;
    GetPrimaryModChecksumFromName m_GetPrimaryModChecksumFromName;
    GetSideCount m_GetSideCount;
    GetSideName m_GetSideName;
    GetLuaAICount m_GetLuaAICount;
    GetLuaAIName m_GetLuaAIName;
    GetLuaAIDesc m_GetLuaAIDesc;
    GetMapOptionCount m_GetMapOptionCount;
    GetModOptionCount m_GetModOptionCount;
    GetOptionKey m_GetOptionKey;
    GetOptionName m_GetOptionName;
    GetOptionDesc m_GetOptionDesc;
    GetOptionType m_GetOptionType;
    GetOptionBoolDef m_GetOptionBoolDef;
    GetOptionNumberDef m_GetOptionNumberDef;
    GetOptionNumberMin m_GetOptionNumberMin;
    GetOptionNumberMax m_GetOptionNumberMax;
    GetOptionNumberStep m_GetOptionNumberStep;
    GetOptionSection m_GetOptionSection;
    GetOptionStringDef m_GetOptionStringDef;
    GetOptionStringMaxLen m_GetOptionStringMaxLen;
    GetOptionListCount m_GetOptionListCount;
    GetOptionListDef m_GetOptionListDef;
    GetOptionListItemKey m_GetOptionListItemKey;
    GetOptionListItemName m_GetOptionListItemName;
    GetOptionListItemDesc m_GetOptionListItemDesc;
    GetModValidMapCount m_GetModValidMapCount;
    GetModValidMap m_GetModValidMap;
    OpenFileVFS m_OpenFileVFS;
    CloseFileVFS m_CloseFileVFS;
    ReadFileVFS m_ReadFileVFS;
    FileSizeVFS m_FileSizeVFS;
    InitFindVFS m_InitFindVFS;
    FindFilesVFS m_FindFilesVFS;
    OpenArchive m_OpenArchive;
    CloseArchive m_CloseArchive;
    FindFilesArchive m_FindFilesArchive;
    OpenArchiveFile m_OpenArchiveFile;
    ReadArchiveFile m_ReadArchiveFile;
    CloseArchiveFile m_CloseArchiveFile;
    SizeArchiveFile m_SizeArchiveFile;
    GetSpringConfigString m_GetSpringConfigString;
    GetSpringConfigInt m_GetSpringConfigInt;
    GetSpringConfigFloat m_GetSpringConfigFloat;
    SetSpringConfigString m_SetSpringConfigString;
    SetSpringConfigInt m_SetSpringConfigInt;
    SetSpringConfigFloat m_SetSpringConfigFloat;
};

#endif
