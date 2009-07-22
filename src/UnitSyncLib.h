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

#ifdef Q_WS_WIN
#define CONV __stdcall
#else
#define CONV
#endif

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
    bool loadLibrary();
    bool library_loaded;
    bool libraryLoaded();
    unsigned int mapChecksum( QString mapname );
    unsigned int modChecksum( QString mapname );
    signed int modIndex( QString modName );
    QString modArchive( int modIndex );
    signed int mapIndex( QString mapName );
    QLibrary* unitsynclib;
    bool setCurrentMod(QString modname);
    QString getSpringVersion();
    QStringList getModNames();
    QStringList getMapNames();
    int getModOptionCount();
    OptionType getOptionType(int optIndex);
    QString getOptionName(int optIndex);
    QString getOptionSection(int optIndex);
    QString getOptionKey(int optIndex);
    bool isGameOption(int optIndex);
    bool isGameOption(QString key);
    bool getOptionBoolDef(int optIndex);
    QString getOptionListDef(int optIndex);
    QStringList getOptionListItems(int optIndex);
    float getOptionNumberDef(int optIndex);
    float getOptionNumberMin (int optIndex);
    float getOptionNumberMax (int optIndex);
    float getOptionNumberStep (int optIndex);
    QString getOptionStringDef(int optIndex);
    int getOptionStringMaxLen(int optIndex);

    //New
    int getSideNameCount();
    QString sideName( int index );
    QIcon getSideIcon(QString sidename);
public slots:
    void reboot();

signals:
    void rebooted();

private:
    QMap<QString, QIcon> m_sideIconsCache;
    QMap<int, QString> m_sideNamesCache;
    QSettings* settings;
    QString m_currentModName;
    QStringList m_gameOptionKeys;
    UnitSyncLib( QObject *parent = 0 );
    ~UnitSyncLib();
    typedef const char* (CONV * GetSpringVersion )();
    typedef void (CONV * Message )( const char* p_szMessage );
    typedef void (CONV * UnInit )();
    typedef int (CONV * Init )( bool isServer, int id );
    typedef int (CONV * InitArchiveScanner )( void );
    typedef int (CONV * ProcessUnits )( void );
    typedef int (CONV * ProcessUnitsNoChecksum )( void );
    typedef const char * (CONV * GetCurrentList )();
    typedef void (CONV * AddClient )( int id, const char *unitList );
    typedef void (CONV * RemoveClient )( int id );
    typedef const char * (CONV * GetClientDiff )( int id );
    typedef void (CONV * InstallClientDiff )( const char *diff );
    typedef int (CONV * GetUnitCount )();
    typedef const char * (CONV * GetUnitName )( int unit );
    typedef const char * (CONV * GetFullUnitName )( int unit );
    typedef int (CONV * IsUnitDisabled )( int unit );
    typedef int (CONV * IsUnitDisabledByClient )( int unit, int clientId );
    typedef void (CONV * AddArchive )( const char* name );
    typedef void (CONV * AddAllArchives )( const char* root );
    typedef unsigned int (CONV * GetArchiveChecksum )( const char* arname );
    typedef const char*(CONV * GetArchivePath )( const char* arname );
    typedef int (CONV * GetMapCount )();
    typedef const char*(CONV * GetMapName )( int index );
    typedef int (CONV * GetMapInfoEx )( const char* name, MapInfo* outInfo, int version );
    typedef int (CONV * GetMapInfo )( const char* name, MapInfo* outInfo );
    typedef int (CONV * GetMapArchiveCount )( const char* mapName );
    typedef const char*(CONV * GetMapArchiveName )( int index );
    typedef unsigned int (CONV * GetMapChecksum )( int index );
    typedef unsigned int (CONV * GetMapChecksumFromName )( const char* mapName );
    typedef void* (CONV * GetMinimap )( const char* filename, int miplevel );
    typedef int (CONV * GetInfoMapSize )( const char * filename, const char * name, int * width, int * height);
    typedef int (CONV * GetInfoMap )( const char * filename, const char * name, void * data, int typeHint);
    typedef int (CONV * GetPrimaryModCount )();
    typedef const char*(CONV * GetPrimaryModName )( int index );
    typedef const char*(CONV * GetPrimaryModShortName )( int index );
    typedef const char*(CONV * GetPrimaryModVersion )( int index );
    typedef const char*(CONV * GetPrimaryModMutator )( int index );
    typedef const char*(CONV * GetPrimaryModGame )( int index );
    typedef const char*(CONV * GetPrimaryModShortGame )( int index );
    typedef const char*(CONV * GetPrimaryModDescription )( int index );
    typedef const char*(CONV * GetPrimaryModArchive )( int index );
    typedef int (CONV * GetPrimaryModArchiveCount )( int index );
    typedef const char*(CONV * GetPrimaryModArchiveList )( int arnr );
    typedef int (CONV * GetPrimaryModIndex )( const char* name );
    typedef unsigned int (CONV * GetPrimaryModChecksum )( int index );
    typedef unsigned int (CONV * GetPrimaryModChecksumFromName )( const char* name );
    typedef int (CONV * GetSideCount )( );
    typedef const char*(CONV * GetSideName )( int side );
    typedef int (CONV * GetLuaAICount )();
    typedef const char*(CONV * GetLuaAIName )( int aiIndex );
    typedef const char*(CONV * GetLuaAIDesc )( int aiIndex );
    typedef int (CONV * GetMapOptionCount )( const char* name );
    typedef int (CONV * GetModOptionCount )();
    typedef const char*(CONV * GetOptionKey )( int optIndex );
    typedef const char*(CONV * GetOptionName )( int optIndex );
    typedef const char*(CONV * GetOptionDesc )( int optIndex );
    typedef int (CONV * GetOptionType )( int optIndex );
    typedef int (CONV * GetOptionBoolDef )( int optIndex );
    typedef float (CONV * GetOptionNumberDef )( int optIndex );
    typedef float (CONV * GetOptionNumberMin )( int optIndex );
    typedef float (CONV * GetOptionNumberMax )( int optIndex );
    typedef float (CONV * GetOptionNumberStep )( int optIndex );
    typedef const char*(CONV * GetOptionSection )( int optIndex );
    typedef const char*(CONV * GetOptionStringDef )( int optIndex );
    typedef int (CONV * GetOptionStringMaxLen )( int optIndex );
    typedef int (CONV * GetOptionListCount )( int optIndex );
    typedef const char* (CONV * GetOptionListDef )( int optIndex );
    typedef const char* (CONV * GetOptionListItemKey )( int optIndex, int itemIndex );
    typedef const char* (CONV * GetOptionListItemName )( int optIndex, int itemIndex );
    typedef const char* (CONV * GetOptionListItemDesc )( int optIndex, int itemIndex );
    typedef int (CONV * GetModValidMapCount )();
    typedef const char* (CONV * GetModValidMap )( int index );
    typedef int (CONV * OpenFileVFS )( const char* name );
    typedef void (CONV * CloseFileVFS )( int handle );
    typedef void (CONV * ReadFileVFS )( int handle, void* buf, int length );
    typedef int (CONV * FileSizeVFS )( int handle );
    typedef int (CONV * InitFindVFS )( const char* pattern );
    typedef int (CONV * FindFilesVFS )( int handle, char* nameBuf, int size );
    typedef int (CONV * OpenArchive )( const char* name );
    typedef void (CONV * CloseArchive )( int archive );
    typedef int (CONV * FindFilesArchive )( int archive, int cur, char* nameBuf, int* size );
    typedef int (CONV * OpenArchiveFile )( int archive, const char* name );
    typedef int (CONV * ReadArchiveFile )( int archive, int handle, void* buffer, int numBytes );
    typedef void (CONV * CloseArchiveFile )( int archive, int handle );
    typedef int (CONV * SizeArchiveFile )( int archive, int handle );
    typedef const char*(CONV * GetSpringConfigString )( const char* name, const char* defvalue );
    typedef int (CONV * GetSpringConfigInt )( const char* name, const int defvalue );
    typedef float(CONV * GetSpringConfigFloat )( const char* name, const float defvalue );
    typedef void (CONV * SetSpringConfigString )( const char* name, const char* value );
    typedef void (CONV * SetSpringConfigInt )( const char* name, const int value );
    typedef void (CONV * SetSpringConfigFloat )( const char* name, const float value );
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
