#include "MapInfoLoader.h"
#include <QMutex>
#include <QMutexLocker>
#include <QTimer>

QMutex mutex;
QMutex clean;

#define QMC_MAGIC 0xA0B0C0D0

static char author[200];
static char description[255];

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
                          clearData(); \
                          clean.unlock(); \
                          return; \
                      } \
clean.unlock();

void MapInfoLoader::run() {
    QMutexLocker lock(&mutex);
    UnitSyncLib* unitSyncLib = UnitSyncLib::getInstance();
    if (unitSyncLib->mapIndex(m_mapName) > 0) {
        if (loadCache()) {
            mapPresent=true;
        } else {
            USELESS_CHECK;
            minimap = unitSyncLib->getMinimapQImage(m_mapName, 0);
            USELESS_CHECK;
            heightmap = unitSyncLib->getHeightMapQImage(m_mapName);
            USELESS_CHECK;
            rawHeightmap = unitSyncLib->getHeightMapRaw(m_mapName);
            USELESS_CHECK;
            metalmap = unitSyncLib->getMetalMapQImage(m_mapName);
            USELESS_CHECK;
            unitSyncLib->getMapInfo(m_mapName, &mapinfo);
            mapPresent = true;
            saveCache();
        }
    } else {
        mapPresent = false;
    }
    QMutexLocker lock2(&clean);
    if (!m_autoclean) {
        emit loadCompleted(m_mapName);
    } else {
        clearData();
    }
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

QDataStream & operator>> (QDataStream& stream, MapInfo& info) {


    stream.readRawData(author, 200);
    info.author = author;
    stream.readRawData(description, 255);
    info.description = description;
    stream >> info.extractorRadius;
    stream >> info.gravity;
    stream >> info.height;
    stream >> info.maxMetal;
    stream >> info.maxWind;
    stream >> info.minWind;
    stream >> info.posCount;


    for (int i =0; i < info.posCount; i++) {
        stream >> info.positions[i].x;
        stream >> info.positions[i].z;
    }

    stream >> info.tidalStrength;
    stream >> info.width;
    return stream;
}

QDataStream & operator<< (QDataStream& stream, const MapInfo& info) {
    stream.writeRawData(info.author, 200);
    stream.writeRawData(info.description, 255);

    stream << info.extractorRadius
            << info.gravity
            << info.height
            << info.maxMetal
            << info.maxWind
            << info.minWind
            << info.posCount;

    for (int i =0; i < info.posCount; i++) {
        stream << info.positions[i].x;
        stream << info.positions[i].z;
    }
    return stream << info.tidalStrength
            << info.width;
}

QDataStream & operator>> (QDataStream& stream, RawHeightMap& rawhm) {
    int w,h;
    stream >> h;
    stream >> w;
    unsigned short *ptr = new unsigned short[w*h];
    stream.readRawData((char*)ptr,(w*h*sizeof(unsigned short)));
    RawHeightMap res(w,h,ptr);
    rawhm = res;
    return stream;

}

QDataStream & operator<< (QDataStream& stream, const RawHeightMap& rawhm) {
    int w = rawhm.getWidth();
    int h = rawhm.getHeight();
    stream << h << w;
    stream.writeRawData((char *)rawhm.getData(), (w*h*sizeof(unsigned short)));
    return stream;
}

void MapInfoLoader::saveCache() {
    QDir userDir(Settings::Instance()->value("spring_user_dir").toString());
    userDir.mkdir("qtlobby");
    userDir.cd("qtlobby");
    QFile file(QString("%1/%2.qmc")
               .arg(userDir.absolutePath())
               .arg(m_mapName));
    file.open(QIODevice::WriteOnly);
    QDataStream fileStream(&file);
    QByteArray buffer;
    QDataStream out(&buffer, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_4);

    out << (quint32)QMC_MAGIC;

    out << minimap;
    out << heightmap;
    out << metalmap;
    out << mapinfo;
    out << rawHeightmap;
    fileStream << qCompress(buffer, 9);
    file.close();
}

bool MapInfoLoader::loadCache() {
    QDir userDir(Settings::Instance()->value("spring_user_dir").toString());
    if (!userDir.exists("qtlobby")) return false;
    userDir.cd("qtlobby");
    QFile file(QString("%1/%2.qmc")
               .arg(userDir.absolutePath())
               .arg(m_mapName));
    if (!file.exists()) return false;
    file.open(QIODevice::ReadOnly);
    QDataStream fileStream(&file);
    QByteArray  buffer;
    fileStream >> buffer;
    buffer = qUncompress(buffer);
    QDataStream in(&buffer, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_4);

    quint32 magic;

    in >> magic;
    if (magic != QMC_MAGIC)
        return false;

    in >> minimap;
    in >> heightmap;
    in >> metalmap;
    in >> mapinfo;
    in >> rawHeightmap;

    return true;
}
