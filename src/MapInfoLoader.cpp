#include "MapInfoLoader.h"
#include <QMutex>
#include <QMutexLocker>

QMutex mutex;

MapInfoLoader::MapInfoLoader(QString mapName, QObject* parent) : QThread(parent) {
    m_mapName = mapName;
    mapinfo.description = m_description;
    mapinfo.author = m_author;
}

void MapInfoLoader::run() {
    QMutexLocker lock(&mutex);
    UnitSyncLib* unitSyncLib = UnitSyncLib::getInstance();
    if(unitSyncLib->mapIndex(m_mapName) > 0) {
        minimap = unitSyncLib->getMinimapQImage(m_mapName, 2);
        heightmap = unitSyncLib->getHeightMapQImage(m_mapName);
        rawHeightmap = unitSyncLib->getHeightMapRaw(m_mapName);
        metalmap = unitSyncLib->getMetalMapQImage(m_mapName);
        unitSyncLib->getMapInfo(m_mapName, &mapinfo);
        mapPresent = true;
    } else {
        mapPresent = false;
    }
    emit loadCompleted(m_mapName);
}
