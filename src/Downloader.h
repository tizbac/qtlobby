#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringList>
#include <QTime>
#include <QFile>
#include <QVector>
#include <QTimerEvent>

/**
   that will do one download from user download list. It yet lacks signals for updating download state.
  */
class Downloader : public QObject {
    Q_OBJECT
public:
    Downloader(QString name, bool map, QObject* parent = 0);
    ~Downloader();
    void start();
signals:
    void downloadProgress(QString name, qint64 bytesReceived, qint64 bytesTotal);
    void stateChanged(QString name, QString state);
    void finished(QString name, bool success);
    void speedEtaChanged(QString name, int speed, int eta);
private slots:
    void onResolverFinished();
    void onMirrorListFinished();
    void onFileSizeFinished();
    void onFetchFinished();
    void download();
    void onDownloadFinished();
    void onJobjolSessionFinished();
    void onJobjolFinished();
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onError(QNetworkReply::NetworkError code);
protected:
    void timerEvent(QTimerEvent* e);
private:
    QVector<QVector<int> > getChunkRanges( QVector<int> hostSpeed /* Bytes/s */, int fileSize /* Bytes */ );
    void openFile();

    QStringList m_mirrors;
    QString m_resourceName;
    QString m_fileName;
    bool m_map;
    QNetworkAccessManager* m_manager;
    QNetworkRequest m_resolve;
    QNetworkRequest m_mirrorList;
    QNetworkRequest m_fetch;
    QNetworkReply* m_reply;
    QList<QNetworkRequest> m_requests;
    QList<QNetworkReply*> m_replies;
    QVector<int> m_hostSpeeds;
    QList<QTime> m_times;
    QVector<QVector<int> > m_ranges;
    qint64 m_fileSize;
    QVector<qint64> m_recievedList;
    float m_received;
    int m_downloadSources;
    int m_jobjolId;
    bool m_downloading;
    bool m_getFromJobjol;
    QString m_jobjolUrl;
    QFile m_out;
};

#endif // DOWNLOADER_H
