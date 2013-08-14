// $Id$
// QtLobby released under the GPLv3, see COPYING for details.

#include "Downloader.h"

#include "IDownloader.h"
#include <QUrl>
#include <QDebug>
#include <QApplication>
#include <QTimer>
#include <QMutex>
#include "Settings.h"


#include <iostream>
#define MAX_TRIES 10
enum dltype
{
  DLTYPE_RAPID = 0,
  DLTYPE_HTTP = 1,
  DLTYPE_PLASMA = 2,
  
};
QMutex prdownloader_mutex;





dltype searchFunc(IDownload::category icat, QString name, std::list<IDownload*>& searchres )
{
  std::string searchname = name.toStdString();
  prdownloader_mutex.lock();
  rapidDownload->search(searchres, searchname.c_str(), icat);
  dltype t = DLTYPE_RAPID;
  if (searchres.empty()) {
       t = DLTYPE_HTTP;
       httpDownload->search(searchres, searchname.c_str(), icat);
       if (searchres.empty()) {
	 t = DLTYPE_PLASMA;
	 plasmaDownload->search(searchres, searchname.c_str(), icat);
       }
  }
  std::cout << "t=" << t << std::endl;

 

  
  
  prdownloader_mutex.unlock();
}

bool Downloader::addDepends(std::list< IDownload* >& dls)
{
	std::list<IDownload*>::iterator it;
	for (it = dls.begin(); it != dls.end(); ++it) {
		if ((*it)->depend.empty()) {
			continue;
		}
		std::list<std::string>::iterator stit;
		for (stit = (*it)->depend.begin(); stit != (*it)->depend.end(); ++stit) {
			std::list<IDownload*> depends;
			const std::string& depend = (*stit);
			
			
			searchFunc(IDownload::CAT_GAMES,QString::fromStdString(depend),depends);

			dls.merge(depends);
			depends.clear();
			searchFunc(IDownload::CAT_MAPS,QString::fromStdString(depend),depends);

			dls.merge(depends);
		}
	}
	return true;
}

/**
Ctor. Gets resource name and resource type
*/
int Downloader::init = 0;
Downloader::Downloader(QString name, bool map, QObject* parent) : QObject(parent) {
    m_resourceName = name;
    m_map = map;
    m_manager = 0;
    m_received = 0;
    m_downloading = false;
    m_getFromJobjol = false;
    if ( !init )
    {
      qRegisterMetaType< QList<IDownload*> >("QList<IDownload*>");
      init = 1;
    }
    //qDebug() << "ctor, name=" + name;
}

Downloader::~Downloader() {
    if(m_manager) delete m_manager;
}

/**
 main function.
*/
void Downloader::start() {
    m_downloading = false;
    //qDebug() << "run, name=" + m_resourceName;
   /* m_manager = new QNetworkAccessManager(this);
    m_resolve.setUrl(resolverUrl.arg(m_map ? "map" : "mod").arg(m_resourceName));
    m_reply = m_manager->get(m_resolve);
    connect(m_reply, SIGNAL(finished()), this, SLOT(onResolverFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(onError(QNetworkReply::NetworkError)));*/
    emit stateChanged(m_resourceName, tr("Resolving filename"));
    
    SearchThread * th = new SearchThread(this,m_map ? IDownload::CAT_MAPS : IDownload::CAT_GAMES ,m_resourceName);
    /*int count = 0;//DownloadSearch(DL_ANY,CAT_MAP,m_resourceName);
    if ( count == 0 )
    {
      emit stateChanged(m_resourceName, tr("Error: ") + tr("Cannot find requested map"));
      emit finished(m_resourceName, false);
      return;
    }*/
    connect(th,SIGNAL(searchCompleted(QList<IDownload*>,int)),this,SLOT(onResolverFinished(QList<IDownload*>,int)));
    connect(th,SIGNAL(finished()),th,SLOT(deleteLater()));
    th->start();
    /* for (int i=0; i<count; i++) {
	      DownloadAdd(i);
    }*/
      

}

void SearchThread::run()
{
  std::list<IDownload*> res;
  dltype t = searchFunc(m_cat,m_name,res);
  emit searchCompleted(QList<IDownload*>::fromStdList(res),(int)t);
 
}

SearchThread::SearchThread(QObject* parent, IDownload::category category, QString name): QThread(parent) , m_name(name) , m_cat(category)
{
  
}


//http://springfiles.com/download.php?maincategory=1&subcategory=2&file=1944_BocageSkirmish.sd7
//http://springfiles.com/download.php?maincategory=1&subcategory=5&file=Supreme+Annihilation+U21+V1.0.sd7

/**
Slot for getting resource filename from resource name via Mirko's gateway
*/
void Downloader::onResolverFinished(QList< IDownload* > searchres,int t) {
   if ( searchres.empty() )
   {
     emit stateChanged(m_resourceName, tr("Error: ") + tr("Cannot find requested map"));
     emit finished(m_resourceName, false);
     return;
   }
   std::cout << "Ok download " << m_resourceName.toStdString() << " " << searchres.size() << std::endl;
   DownloadThread * dth = new DownloadThread(searchres,t,this);
   connect(dth,SIGNAL(downloadFinished(bool)),this,SLOT(onDownloadFinished(bool)));
   connect(dth,SIGNAL(progressReport(qint64,qint64)),this,SLOT(onDownloadProgress(qint64,qint64)));
   connect(dth,SIGNAL(finished()),dth,SLOT(deleteLater()));
   dth->start();
   /* QString result = m_reply->readAll().trimmed();
    if(result.isEmpty()) {
        emit stateChanged(m_resourceName, tr("File not found"));
        emit finished(m_resourceName, false);
        return;
    }
    m_jobjolId = result.split(",").takeFirst().toInt();
    m_fileName =result.split(",").takeLast();
    //qDebug() << "onResolverFinished, filename=" + m_fileName;
    QString url;
    if(m_map) {
        url = mirrorListUrl.arg(m_fileName, "maps");
        m_jobjolUrl = jobjol.arg(2).arg(m_fileName);
    } else {
        url = mirrorListUrl.arg(m_fileName, "mods");
        m_jobjolUrl = jobjol.arg(5).arg(m_fileName);
    }
    m_mirrorList.setUrl(url);
    m_mirrorList.setRawHeader("Referer", mirrorListReferer.arg(m_jobjolId).toAscii());
    m_reply = m_manager->get(m_mirrorList);
    connect(m_reply, SIGNAL(finished()), this, SLOT(onMirrorListFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(onError(QNetworkReply::NetworkError)));
    emit stateChanged(m_resourceName, tr("Retrieving mirror list"));*/
}
DownloadThread::DownloadThread(QList< IDownload* > searchres, int t, QObject* parent): QThread(parent) , m_searchres(searchres), m_t(t)
{
  m_progress_timer = new QTimer(this);
  m_progress_timer->setSingleShot(false);
  m_progress_timer->setInterval(500);
  connect(m_progress_timer,SIGNAL(timeout()),this,SLOT(reportProgress()));
  m_progress_timer->start();
}

void DownloadThread::reportProgress()
{
  qint64 totalsize = 0.0;
  qint64 downloaded_size = 0.0;
  for ( QList<IDownload*>::iterator it = m_searchres.begin(); it != m_searchres.end(); it++ )
  {
    
    totalsize += (*it)->size;
    downloaded_size += (*it)->getProgress();//hopefully piece.size will not change during download
  }
  if ( totalsize != 0 )
    emit progressReport(downloaded_size,totalsize);
}


void DownloadThread::run()
{
  std::list<IDownload*> dl = m_searchres.toStdList();
  bool result = true;

  Downloader::addDepends(dl);

  prdownloader_mutex.lock();
  for ( std::list<IDownload*>::iterator it = dl.begin(); it != dl.end(); it++ )
  {
    if ( (*it)->dltype == IDownload::TYP_RAPID )
      result = rapidDownload->download(*it);
    if ( (*it)->dltype == IDownload::TYP_HTTP )
      result = httpDownload->download(*it);
    if ( m_t == DLTYPE_PLASMA )
      result = plasmaDownload->download(*it);
  }
  prdownloader_mutex.unlock();
  std::cout << "DLTHREAD: Finished " << result << std::endl;
  emit downloadFinished(result);
  IDownloader::freeResult(dl);
}

/**
Slot for getting mirror list from jobjol.nl
*/
void Downloader::onMirrorListFinished() {

}

/**
Slot for getting size of a file. It aborts connection as soon as we got it
*/
void Downloader::onFileSizeFinished() {

}

/**
Slot for getting requested resource
*/
void Downloader::onFetchFinished() {

}

/**
This method opens target file
*/
void Downloader::openFile() {

}

/**
This method starts multisegmented download
*/
void Downloader::download() {

}

/**
Slot for processing completed segment
*/
void Downloader::onDownloadFinished(bool success) {
  if ( success )
  {
   emit stateChanged(m_resourceName, tr("Finished"));
   emit finished(m_resourceName, true);
  }else{
    emit stateChanged(m_resourceName, tr("Failed"));
   emit finished(m_resourceName, true);
  }
   
}

/**
Slot for getting jobjol cookies for download
*/
void Downloader::onJobjolSessionFinished() {

}

/**
Slot for processing completed request to jobjol.nl
*/
void Downloader::onJobjolFinished() {

}

/**
Informing the world about how we are doing
*/
void Downloader::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {

    emit downloadProgress(m_resourceName, bytesReceived, bytesTotal);

}

/**
  Gives the startpoint and number of bytes to download for downloading for each host.
  @param hostSpeed is a list of speeds in Bytes/s
  @param fileSize is the size of the file to download
  @return is a list of start position and number of Bytes to download from each host (might be 0 for skipping that host).
*/
/*QVector<QVector<int> > Downloader::getChunkRanges( QVector<int> hostSpeed, int fileSize ) {
    int maxSpeed = 0, sumSpeed = 0;
    QVector<float> fractions;
    foreach( int speed, hostSpeed ) {
        fractions.append((float) speed);
        maxSpeed = qMax( maxSpeed, speed );
        sumSpeed += speed;
    }
    float sumFraction =0;
    for( int i = 0; i < fractions.size(); i++ ) {
        fractions[i] = fractions[i]/sumSpeed; // get
        if( hostSpeed.at(i) == maxSpeed )
            fractions[i] *= 1.2; // give the fastest a little bit more than it's proportional part
        if( hostSpeed.at(i) < maxSpeed*0.2 )
            fractions[i] = 0; // don't download from too slow servers
        sumFraction += fractions[i];
    }
    QVector<QVector<int> > ret;
    int currentPos = 0;
    foreach( float fraction, fractions ) {
        QVector<int> entry;
        int startPos = currentPos;
        int bytes = (int) qRound(fraction/sumFraction*fileSize);
        currentPos += bytes;
        entry.append(startPos);
        entry.append(bytes);
        ret.append(entry);
    }
    ret.last()[1] += fileSize - currentPos; // due to round some bytes might be left, assign to last one
    return ret;
}*/

/**
Timer event for speed and ETA estimation
*/
void Downloader::timerEvent(QTimerEvent* /*e*/) {
   /* static quint64 lastReceived = 0;
    quint64 delta = m_received - lastReceived;
    int speed = qRound(delta/((float)timerInterval/1000));
    int eta = 0;
    if(m_fileSize)
        eta = qRound((m_fileSize - m_received)/speed);
    emit speedEtaChanged(m_resourceName, speed,eta);
    lastReceived = m_received;*/
}

void Downloader::onError(QNetworkReply::NetworkError /*code*/) {
   /* QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QString url = reply->url().toString();
    if(m_retriesPerUrl.contains(url))
        m_retriesPerUrl[url] = 0;
    else
        m_retriesPerUrl[url]++;
    qCritical() << tr("Failed to fetch %1. Error: %2").arg(url).arg(reply->errorString());
    if(m_retriesPerUrl[url] < MAX_TRIES) {
        m_manager->get(reply->request());
        qCritical() << "Retry #" + QString::number(m_retriesPerUrl[url]);
    } else {
        qCritical() << "Max retries reached. Fetch failed completely.";
        emit stateChanged(m_resourceName, tr("Error: ") + reply->errorString());
        emit finished(m_resourceName, false);
    }*/
}
