// $Id: InfoChannel.cpp 229 2009-07-23 02:18:19Z mirko.windhoff $
// QtLobby released under the GPLv3, see COPYING for details.
#include "DownloadsModel.h"
#include "Downloader.h"
#include "UnitSyncLib.h"

const int Gb = 1024*1024*1024;
const int Mb = 1024*1024;
const int Kb = 1024;

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
    connect(d, SIGNAL(speedEtaChanged(QString,int,int)),
            this, SLOT(speedEtaChanged(QString,int,int)));
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
    connect(d, SIGNAL(speedEtaChanged(QString,int,int)),
            this, SLOT(speedEtaChanged(QString,int,int)));
    d->start();
    reset();
}

int DownloadsModel::rowCount(const QModelIndex & /*parent*/) const {
    return m_downloads.size();
}

int DownloadsModel::columnCount(const QModelIndex & /*parent*/) const {
    return 6;
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
            int downloaded = m_downloads[index.row()]->downloaded;
            if(downloaded > Gb)
                return tr("%1 GiB").arg(QString::number((float)downloaded/Gb,'f',2));
            if(downloaded > Mb)
                return tr("%1 MiB").arg(QString::number((float)downloaded/Mb,'f',2));
            if(downloaded > Kb)
                return tr("%1 KiB").arg(QString::number((float)downloaded/Kb,'f',2));
            return QString::number(downloaded) + " B";
        } else if(index.column() == 4) {
            int speed = m_downloads[index.row()]->speed;
            if(speed > Gb)
                return tr("%1 GiB/s").arg(QString::number((float)speed/Gb,'f',2));
            if(speed > Mb)
                return tr("%1 MiB/s").arg(QString::number((float)speed/Mb,'f',2));
            if(speed > Kb)
                return tr("%1 KiB/s").arg(QString::number((float)speed/Kb,'f',2));
            return tr("%1 B/s").arg(QString::number(speed));
        } else if(index.column() == 5) {
            int eta = m_downloads[index.row()]->eta;
            QTime t(0,0,0);
            t = t.addSecs(eta);
            return t.toString();
        }
    } else if ( role == Qt::TextAlignmentRole ) {
        switch( index.column() ) {
            case 3:
            case 4:
                return Qt::AlignRight;
            default:
                return Qt::AlignLeft;
        }
    }
    return QVariant();
}

QVariant DownloadsModel::headerData ( int section, Qt::Orientation /*rientation*/, int role) const {
    if(role == Qt::DisplayRole) {
        if(section == 0) {
            return tr("Title");
        } else if(section == 1) {
            return tr("State");
        } else if(section == 2) {
            return tr("Progress");
        } else if(section == 3) {
            return tr("Downloaded");
        } else if(section == 4) {
            return tr("Speed");
        } else if(section == 5) {
            return tr("ETA");
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
    d->downloaded = received;
    if(total == -1) {
        d->percent = -1;
    } else {
        d->percent = qRound(received/(double)total*100);
    }
    emit dataChanged(index(m_downloads.indexOf(d), 2), index(m_downloads.indexOf(d), 3));
}

void DownloadsModel::stateChanged(QString name, QString state) {
    Download* d = findResource(name);
    d->state = state;
    emit dataChanged(index(m_downloads.indexOf(d), 3), index(m_downloads.indexOf(d), 3));
}

void DownloadsModel::finished(QString name, bool success) {
    Download* d = findResource(name);
    d->finished = true;
    if(success)
        UnitSyncLib::getInstance()->reboot();
    sender()->deleteLater();
}

void DownloadsModel::speedEtaChanged(QString name, int speed, int eta) {
    Download* d = findResource(name);
    d->speed = speed;
    d->eta = eta;
    emit dataChanged(index(m_downloads.indexOf(d), 4), index(m_downloads.indexOf(d), 5));
}

void DownloadsModel::clearFinished() {
    for(int i = 0; i < m_downloads.size(); i++) {
        if(m_downloads.at(i)->finished)
            m_downloads.removeAt(i);
    }
    reset();
}
