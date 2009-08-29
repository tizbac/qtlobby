#include "GLWaterPlane.h"

GLWaterPlane::GLWaterPlane() {
}

void GLWaterPlane::build(RawHeightMap heightmap, float y, int downscale) {
    if(downscale <= 0) downscale = 1;
    m_heightmap = RawHeightMap(heightmap.getWidth()/downscale, heightmap.getHeight()/downscale, 0, 0, 0);
    vertexNumber = m_heightmap.getWidth()*(m_heightmap.getHeight()-1)*2;
    vertexes = new Vertex[vertexNumber];
    for (int i = 0; i < m_heightmap.getHeight(); i++) {
        for (int j = 0; j < m_heightmap.getWidth(); j++) {
            int offset = i*m_heightmap.getWidth()+j;
            vertexes[offset].x = i * m_scaleFactor * 8 * downscale;
            vertexes[offset].y = y;
            vertexes[offset].z = j * m_scaleFactor * 8 * downscale;
        }
    }
    generateIndexes();
}


