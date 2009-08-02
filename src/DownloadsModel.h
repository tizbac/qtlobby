#ifndef DOWNLOADSMODEL_H
#define DOWNLOADSMODEL_H

#include <QAbstractTableModel>
#include <Singleton.h>

/**
  Struct representing one row of a model
  */
struct Download {
    Download() {
        percent = 0;
        speed = 0;
        eta = 0;
        downloaded = 0;
    }
    QString title;
    float percent;
    QString state;
    int downloaded;
    float speed;
    int eta;
};

/**
  Model for DownloadsDialog
  */
class DownloadsModel : public QAbstractTableModel, public Singleton<DownloadsModel> {
    Q_OBJECT
    friend class Singleton<DownloadsModel>;
public:
    void startMapDownload(QString mapName);
    void startModDownload(QString modName);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
private slots:
    void downloadProgressChanged(QString name,qint64 received,qint64 total);
    void stateChanged(QString name, QString state);
    void finished(QString name, bool success);
    void speedEtaChanged(QString name, int speed, int eta);
private:
    DownloadsModel(QObject* parent = 0);
    ~DownloadsModel();
    Download* findResource(QString title);

    //list of downloads
    QList<Download*> m_downloads;
};

#endif // DOWNLOADSMODEL_H
