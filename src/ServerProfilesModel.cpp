#include "ServerProfilesModel.h"
#include "PathManager.h"
#include <QIcon>
#include <QBuffer>
#include <QDebug>

ServerProfilesModel::ServerProfilesModel(QObject* parent) : QAbstractTableModel(parent)  {
    m_settings = Settings::Instance();
    load();
}

int ServerProfilesModel::rowCount ( const QModelIndex & /*parent*/ ) const {
    return m_profiles.size();
}

int ServerProfilesModel::columnCount ( const QModelIndex & /*parent*/ ) const {
    return 1;
}

QVariant ServerProfilesModel::data ( const QModelIndex & index, int role ) const {
    if(!index.isValid()) return QVariant();
    const QUrl& url = m_profiles.at(index.row());
    if(role == Qt::DisplayRole) {
        QString result = "%1@%2";
        return result.arg(url.userName()).arg(url.host());
    } else if(role == Qt::DecorationRole) {
        //These are probably for Jakob
        //TODO: Make better icons for server profile and autologin
        //TODO2: And some icon for active profile?(currently connected one)
        if(index.row() == m_autologin)
            return QIcon(P("icons/open_game.xpm"));
        else
            return QIcon(P("icons/open_game_empty.xpm"));
    } else if(role == Qt::UserRole) {
        return m_profiles[index.row()];
    }
    return QVariant();
}

void ServerProfilesModel::save() {
    QStringList profiles;
    foreach(const QUrl& url, m_profiles) {
        QUrl tmp (url);
        tmp.setScheme("tas");
        profiles << tmp.toString();
    }
    m_settings->setValue("serverProfiles", profiles);
}

void ServerProfilesModel::load() {
    QStringList profiles = m_settings->value("serverProfiles").toStringList();
    foreach(const QString& str, profiles)
        m_profiles << parseUrl(str);
    m_autologin = m_settings->value("autologinProfile", -1).toInt();
}

void ServerProfilesModel::setAutologin(const QModelIndex& index) {
    QModelIndex last = this->index(m_autologin, 0);
    if(!index.isValid()) {
        m_autologin = -1;
    } else {
        m_autologin = index.row();
        emit dataChanged(index, index);
    }
    emit dataChanged(last, last);
    m_settings->setValue("autologinProfile", m_autologin);
}

bool ServerProfilesModel::isAutologin(const QModelIndex& index) const {
    return index.row() == m_autologin;
}

void ServerProfilesModel::createProfile() {
    QUrl url;
    url.setHost( "springrts.com" );
    url.setPort( 8200 );
    url.setUserName("New_profile_" + QString::number(m_profiles.size()));
    m_profiles << url;
    reset();
    save();
}

void ServerProfilesModel::createProfile(const QUrl& url) {
    m_profiles << url;
    reset();
    save();
}

void ServerProfilesModel::modifyProfile(const QModelIndex& profile, const QUrl& url) {
    m_profiles[profile.row()] = url;
    emit dataChanged(profile, profile);
    save();
}

void ServerProfilesModel::removeProfile(const QModelIndex& profile) {
    if(m_autologin > profile.row()) m_autologin--;
    m_profiles.removeAt(profile.row());
    reset();
    save();
}


QModelIndex ServerProfilesModel::getAutologinIndex() const {
    if(m_autologin >= 0)
        return index(m_autologin, 0);
    return QModelIndex();
}

bool ServerProfilesModel::hasAutologin() const {
    return m_autologin >= 0;
}

QUrl ServerProfilesModel::parseUrl(const QString& str) {
    QRegExp re("^tas://([^:]*)(?::(.*))?@(.*):(.*)$");
    re.setMinimal(true);
    if(re.exactMatch(str)) {
        QUrl url;
        url.setUserName(re.cap(1));
        url.setPassword(re.cap(2));
        url.setHost(re.cap(3));
        url.setPort(re.cap(4).toInt());
        return url;
    }
    return QUrl();
}

void ServerProfilesModel::setActiveProfile(const QModelIndex& profile) {
    m_active = m_profiles[profile.row()];
    m_activeIndex = profile;
}

void ServerProfilesModel::setActiveProfile(const QUrl& url) {
    m_active = url;
}

QUrl ServerProfilesModel::getActiveProfile() {
    return m_active;
}

void ServerProfilesModel::changeActiveProfileName(QString newname) {
    m_profiles[m_activeIndex.row()].setUserName(newname);
    save();
    emit dataChanged(m_activeIndex, m_activeIndex);
}

void ServerProfilesModel::changeActiveProfilePassword(QString password) {
    m_profiles[m_activeIndex.row()].setPassword(password);
    save();
    emit dataChanged(m_activeIndex, m_activeIndex);
}

QModelIndex ServerProfilesModel::getActiveProfileIndex() {
    return m_activeIndex;
}
