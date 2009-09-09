#include "PathManager.h"

#include <QFile>
#include <QDir>
#include <QDebug>

PathManager::PathManager() {
    m_overlay = QString::null;
    m_overlayUsable = false;
}

void PathManager::setResourceRoot(const QString& path) {
    m_base = path + QDir::separator();
}

void PathManager::setOverlayPath(const QString& path) {
    QDir d(path);
    if(d.exists()) {
        m_overlay = path + QDir::separator();
        m_overlayUsable = true;
    } else
        m_overlayUsable = false;
}

QString PathManager::resolvePath(const QString& path) {
    if(m_overlayUsable) {
        if(!m_inOverlay.contains(path))
            m_inOverlay[path] = QFile::exists(m_overlay+path);
        if(m_inOverlay[path])
            return m_overlay + path;
    }
    return m_base + path;
}

QString PathManager::resolveOverlayPath(const QString& path) {
    return m_overlay + path;
}

QString PathManager::resolveOverlayPath() {
    return m_overlay;
}

bool PathManager::isOverlayValid() {
    return m_overlayUsable;
}

void PathManager::invalidateCache(const QString& path) {
    m_inOverlay[path] = QFile::exists(m_overlay+path);
}

