#ifndef PATHMANAGER_H
#define PATHMANAGER_H

#include <QMap>
#include <QString>
#include "Singleton.h"

class PathManager : public Singleton<PathManager> {
    friend class Singleton<PathManager>;
public:
    void setResourceRoot(const QString& path);
    void setOverlayPath(const QString& path);
    QString resolvePath(const QString& path);
    QString resolveOverlayPath(const QString& path);
    QString resolveOverlayPath();
    bool isOverlayValid();
    void invalidateCache(const QString& path);
private:
    QString m_base;
    QString m_overlay;
    bool m_overlayUsable;
    QMap<QString, bool> m_inOverlay;
    PathManager();
};

#define P(p) PathManager::getInstance()->resolvePath(p)

#endif // PATHMANAGER_H
