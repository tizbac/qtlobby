// $Id: InfoChannel.cpp 229 2009-07-23 02:18:19Z mirko.windhoff $
// QtLobby released under the GPLv3, see COPYING for details.
#include "DownloadsModel.h"
#include "Downloader.h"
#include "UnitSyncLib.h"

DownloadsModel::DownloadsModel(QObject* parent) : QAbstractTableModel(parent) {

}

DownloadsModel::~DownloadsModel() {
    qDeleteAll(m_downloads.begin(), m_downloads.end());
}

void DownloadsModel::startMapDownload(QString mapName) {
    QRegExp re("(.*)\\..*");
    re.setMinimal(true);
    if(re.indexIn(mapName) >= 0)
        mapName = re.cap(1);
    if(findResource(mapName)) return;
    Download* dl = new Download();
    dl->title = mapName;
    m_downloads << dl;
    Downloader* d = new Downloader(mapName, true, this);
    connect(d, SIGNAL(downloadProgress(QString,qint64,qint64)),
            this, SLOT(downloadProgressChanged(QString,qint64,qint64)));
    connect(d, SIGNAL(stateChanged(QString,QString)),
            this, SLOT(stateChanged(QString,QString)));
    connect(d, SIGNAL(finished(QString,bool)),
            this, SLOT(finished(QString,bool)));
    d->start();
    reset();
}

void DownloadsModel::startModDownload(QString modName) {
    if(findResource(modName)) return;
    Download* dl = new Download();
    dl->title = modName;
    m_downloads << dl;
    Downloader* d = new Downloader(modName, false, this);
    connect(d, SIGNAL(downloadProgress(QString,qint64,qint64)),
            this, SLOT(downloadProgressChanged(QString,qint64,qint64)));
    connect(d, SIGNAL(stateChanged(QString,QString)),
            this, SLOT(stateChanged(QString,QString)));
    connect(d, SIGNAL(finished(QString,bool)),
            this, SLOT(finished(QString,bool)));
    d->start();
    reset();
}

int DownloadsModel::rowCount(const QModelIndex & /*parent*/) const {
    return m_downloads.size();
}

int DownloadsModel::columnCount(const QModelIndex & /*parent*/) const {
    return 4;
}

QVariant DownloadsModel::data(const QModelIndex & index, int role) const {
    if(role == Qt::DisplayRole) {
        if(index.column() == 0) {
            return m_downloads[index.row()]->title;
        } else if(index.column() == 1) {
            return m_downloads[index.row()]->state;
        } else if(index.column() == 2) {
            return m_downloads[index.row()]->percent;
        } else if(index.column() == 3) {
            return m_downloads[index.row()]->speed;
        }
    }
    return QVariant();
}

QVariant DownloadsModel::headerData ( int section, Qt::Orientation /*rientation*/, int role) const {
    if(role == Qt::DisplayRole) {
        if(section == 0) {
            return "Title";
        } else if(section == 1) {
            return "State";
        } else if(section == 2) {
            return "Progress";
        } else if(section == 3) {
            return "Speed";
        }
    }
    return QVariant();
}

Download* DownloadsModel::findResource(QString title) {
    for(int i = 0; i < m_downloads.size(); i++) {
        if(m_downloads.at(i)->title == title)
            return m_downloads[i];
    }
    return (Download*)0;
}

void DownloadsModel::downloadProgressChanged(QString name,qint64 received,qint64 total) {
    Download* d = findResource(name);
    if(received == total) {
        sender()->deleteLater();
        d->percent = 100;
    } else {
        d->percent = qRound(received/(double)total*100);
    }
    emit dataChanged(index(m_downloads.indexOf(d), 2), index(m_downloads.indexOf(d), 2));
}

void DownloadsModel::stateChanged(QString name, QString state) {
    Download* d = findResource(name);
    d->state = state;
    emit dataChanged(index(m_downloads.indexOf(d), 3), index(m_downloads.indexOf(d), 3));
}

void DownloadsModel::finished(QString /*name*/, bool success) {
    //Download* d = findResource(name);
    if(success)
        UnitSyncLib::getInstance()->reboot();
}
