#ifndef QTLOBBY_DOWNLOADER_H
#define QTLOBBY_DOWNLOADER_H

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringList>
#include <QTime>
#include <QFile>
#include <QVector>
#include <QTimerEvent>
#include <QThread>
#include <QTimer>
/**
   that will do one download from user download list. It yet lacks signals for updating download state.
  */
#include "Download.h"
class Downloader : public QObject {
    Q_OBJECT
public:
    Downloader(QString name, bool map, QObject* parent = 0);
    ~Downloader();
    void start();
    static int init;
    static bool addDepends(std::list<IDownload*>& dls);
signals:
    void downloadProgress(QString name, qint64 bytesReceived, qint64 bytesTotal);
    void stateChanged(QString name, QString state);
    void finished(QString name, bool success);
    void speedEtaChanged(QString name, int speed, int eta);
private slots:
    void onResolverFinished(QList< IDownload* > searchres, int t);
    void onMirrorListFinished();
    void onFileSizeFinished();
    void onFetchFinished();
    void download();
    void onDownloadFinished(bool success);
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
    QMap<QString, int> m_retriesPerUrl;
    
};

class SearchThread : public QThread 
{
  Q_OBJECT
public:
explicit SearchThread(QObject* parent, IDownload::category category, QString name);
protected:
  virtual void run();
signals:
  void searchCompleted(QList<IDownload*> searchres,int t);
private:
  QString m_name;
  IDownload::category m_cat;
};

class DownloadThread : public QThread
{
    Q_OBJECT
public:
explicit DownloadThread(QList<IDownload*> searchres,int t, QObject* parent = 0);
signals:
  void downloadFinished(bool success);
  void progressReport(qint64 bytesReceived, qint64 bytesTotal);
protected:
virtual void run();
private:
  QList<IDownload*> m_searchres;
  int m_t;
QTimer* m_progress_timer;
public slots:
void reportProgress();
};

#endif // QTLOBBY_DOWNLOADER_H
