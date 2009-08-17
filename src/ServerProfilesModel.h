#ifndef SERVERPROFILESMODEL_H
#define SERVERPROFILESMODEL_H

#include <QAbstractTableModel>
#include <QUrl>

#include "Settings.h"
#include "Singleton.h"

class ServerProfilesModel : public QAbstractTableModel, public Singleton<ServerProfilesModel> {
    Q_OBJECT
    friend class Singleton<ServerProfilesModel>;
public:
    static QUrl parseUrl(const QString& str);
    int rowCount (const QModelIndex & parent = QModelIndex()) const;
    int columnCount (const QModelIndex & parent = QModelIndex()) const;
    QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;
    void setAutologin(const QModelIndex& index);
    bool isAutologin(const QModelIndex& index) const;
    QModelIndex getAutologinIndex() const;
    bool hasAutologin() const;
    void createProfile();
    void createProfile(const QUrl& url);
    void modifyProfile(const QModelIndex& profile, const QUrl& url);
    void removeProfile(const QModelIndex& profile);
    void setActiveProfile(const QModelIndex& profile);
    QUrl getActiveProfile();
private:
    ServerProfilesModel(QObject* parent = 0);
    void save();
    void load();

    QList<QUrl> m_profiles;
    QSettings* m_settings;
    int m_autologin;
};

#endif // SERVERPROFILESMODEL_H
