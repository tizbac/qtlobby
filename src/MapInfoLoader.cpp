#include "MapInfoLoader.h"
#include <QMutex>
#include <QMutexLocker>
#include <QTimer>

QMutex mutex;
QMutex clean;

MapInfoLoader::MapInfoLoader(QObject* parent) : QThread(parent) {
    mapinfo.description = m_description;
    mapinfo.author = m_author;
    m_autoclean = false;
}

void MapInfoLoader::setMap(QString mapName) {
    m_mapName = mapName;
}

#define USELESS_CHECK \
clean.lock(); \
if(m_autoclean) { \
                  qDebug() << "Early data cleaning..."; \
                  clearData(); \
                  clean.unlock(); \
                  return; \
              } \
clean.unlock();

void MapInfoLoader::run() {
    qDebug() << "Loader working for " << m_mapName << "...";
    QMutexLocker lock(&mutex);
    UnitSyncLib* unitSyncLib = UnitSyncLib::getInstance();
    if(unitSyncLib->mapIndex(m_mapName) > 0) {
        USELESS_CHECK
                minimap = unitSyncLib->getMinimapQImage(m_mapName, 2);
        USELESS_CHECK
                heightmap = unitSyncLib->getHeightMapQImage(m_mapName);
        USELESS_CHECK
                rawHeightmap = unitSyncLib->getHeightMapRaw(m_mapName);
        USELESS_CHECK
                metalmap = unitSyncLib->getMetalMapQImage(m_mapName);
        USELESS_CHECK
                unitSyncLib->getMapInfo(m_mapName, &mapinfo);
        mapPresent = true;
    } else {
        mapPresent = false;
    }
    qDebug() << "Loader finished...";
    QMutexLocker lock2(&clean);
    if(!m_autoclean) {
        qDebug() << "Signaling...";
        emit loadCompleted(m_mapName);
    } else {
        qDebug() << "Clearing data...";
        clearData();
    }
    qDebug() << "Loader done";
}

void MapInfoLoader::cleanup() {
    QMutexLocker lock(&clean);
    m_autoclean = true;
}

void MapInfoLoader::clearData() {
    minimap = QImage();
    heightmap = QImage();
    rawHeightmap = RawHeightMap();
    metalmap = QImage();
    memset(&mapinfo, 0, sizeof(mapinfo));
    memset(m_description, 0, sizeof(m_description));
    memset(m_author, 0, sizeof(m_author));
    mapinfo.description = m_description;
    mapinfo.author = m_author;
    QTimer::singleShot(200, this, SLOT(deleteLater()));
    QThread::exit(0);
}
