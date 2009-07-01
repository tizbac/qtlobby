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
        if(loadCache()){
            qDebug()<<"Cache Found!";
            mapPresent=true;
        }
        else {
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
            saveCache();
        }
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

QDataStream & operator>> (QDataStream& stream, MapInfo& info)
{

        char author[201];
        char descrip[256];

        stream.readRawData(author, 200);

        info.author = author;
        qDebug() << ">> GET auth: " << info.author;

        stream.readRawData(descrip, 255);
        info.description = descrip;

        qDebug() << ">> GET desc: " << info.description;

        stream >> info.extractorRadius;
        qDebug() << ">> GET extrac: " << info.extractorRadius;

        stream >> info.gravity;
        qDebug() << ">> GET grav: " << info.gravity;

        stream >> info.height;
        qDebug() << ">> GET height: " << info.height;

        stream >> info.maxMetal;
        qDebug() << ">> GET maxmetal: " << info.maxMetal;

        stream >> info.maxWind;
        qDebug() << ">> GET maxwind: " << info.maxWind;

         stream >> info.minWind;
        qDebug() << ">> GET minwind: " << info.minWind;

       stream >> info.posCount;
        qDebug() << ">> GET poscount: " << info.posCount;


        for(int i =0; i < info.posCount; i++){
            stream >> info.positions[i].x;
            qDebug() << ">> GET pos(" <<i<<")x: " << info.positions[i].x;
            stream >> info.positions[i].z;
            qDebug() << ">> GET pos(" <<i<<")z: " << info.positions[i].z;
        }
        stream >> info.tidalStrength;
        qDebug() << ">> GET tidal: " << info.tidalStrength;

       stream >> info.width;
        qDebug() << ">> GET width: " << info.width;



        return stream;
}
QDataStream & operator<< (QDataStream& stream, const MapInfo& info)
{
    qDebug()<< "save: " << info.author;


        qDebug() << ">> SET author: " << info.author;
        qDebug() << ">> SET desc: " << info.description;
        qDebug() << ">> SET grav: " << info.gravity;
        qDebug() << ">> SET height: " << info.height;
        qDebug() << ">> SET maxmetal: " << info.maxMetal;
        qDebug() << ">> SET maxwind: " << info.maxWind;
        qDebug() << ">> SET minwind: " << info.minWind;
         qDebug() << ">> SET poscount: " << info.posCount;

        stream.writeRawData(info.author, 200);
        stream.writeRawData(info.description, 255);

        stream << info.extractorRadius
        << info.gravity
        << info.height
        << info.maxMetal
        << info.maxWind
        << info.minWind
        << info.posCount;

        for(int i =0; i < info.posCount; i++){
            qDebug() << ">> SET pos(" <<i<<")x: " << info.positions[i].x;
            stream << info.positions[i].x;
            qDebug() << ">> SET pos(" <<i<<")z: " << info.positions[i].z;
            stream << info.positions[i].z;
        }
        qDebug() << ">> SET tidal" << info.tidalStrength;
        qDebug() << ">> SET width" << info.width;
       return stream << info.tidalStrength
        << info.width;
}

QDataStream & operator>> (QDataStream& stream, RawHeightMap& rawhm)
{
    int w,h;
    stream >> h;
    stream >> w;
    qDebug() << ">>GET raw height:"<< h;
    qDebug() << ">>GET raw width:"<< w;
    unsigned short *ptr = new unsigned short[w*h];
    stream.readRawData((char*)ptr,(w*h*sizeof(unsigned short)));

    for(int i=0;i<10;i++){
        qDebug()<< ">> CHAR:" << ptr[i];
    }

    RawHeightMap res(w,h,ptr);
    rawhm = res;
    //delete[] ptr;
    return stream;

}
QDataStream & operator<< (QDataStream& stream, const RawHeightMap& rawhm){

    stream << rawhm.getHeight() << rawhm.getWidth();
    int w,h;
    w = rawhm.getHeight();
    h = rawhm.getWidth();


    unsigned short *ptr = new unsigned short[w*h];
    ptr = (unsigned short *)rawhm.getData();

   for(int i=0;i<10;i++){
        qDebug()<< ">> SET CHAR:" << ptr[i];
    }

    qDebug()<<">> SET raw height"<<h;
    qDebug()<<">> SET raw width"<<w;
    stream.writeRawData((char *)rawhm.getData(), (w*h));

    return stream;
}

void MapInfoLoader::saveCache()
{
    QFile file(QString("qtlobby/%1.qmc").arg(m_mapName));
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);

    out.setVersion(QDataStream::Qt_4_4);

    qDebug()<<"START SAVING!";
    out << (quint32)0xA0B0C0D0;

    out << minimap;
    out << heightmap;
    out << metalmap;
    out << mapinfo;
    out << rawHeightmap;

    qDebug()<<"SAVING DONE!";

}

bool MapInfoLoader::loadCache()
{
    QFile file(QString("qtlobby/%1.qmc").arg(m_mapName));

    if(!file.exists()) return false;

    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_4);

    quint32 magic;

    qDebug()<<"START GETTING SHIT!";

    in >> magic;
    if(magic != 0xA0B0C0D0)
        return false;

    in >> minimap;
    in >> heightmap;
    in >> metalmap;
    in >> mapinfo;
    in >> rawHeightmap;

    qDebug()<<"GETTING SHIT DONE!";
    return true;
}
