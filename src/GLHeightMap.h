#ifndef GLHEIGHTMAP_H
#define GLHEIGHTMAP_H

#include "GLObject.h"
#include "RawHeightMap.h"

class GLHeightMap : public GLObject {
public:
    GLHeightMap();
    void build(RawHeightMap heightmap);
    int getWidth();
    int getHeight();
    void setScaleFactor(float factor);
protected:
    RawHeightMap m_heightmap;
    float m_scaleFactor;

    void computeNormals();
    void generateIndexes();
    void generateTexCoords();
};

inline void GLHeightMap::setScaleFactor(float factor) {
    m_scaleFactor = factor;
}

inline int GLHeightMap::getWidth() {
    return m_heightmap.getWidth() * m_scaleFactor * 8 * 1/m_heightmap.getRatio();
}

inline int GLHeightMap::getHeight() {
    return m_heightmap.getHeight() * m_scaleFactor * 8 * 1/m_heightmap.getRatio();
}

#endif // GLHEIGHTMAP_H
