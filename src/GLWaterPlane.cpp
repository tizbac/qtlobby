#include "GLWaterPlane.h"

GLWaterPlane::GLWaterPlane() {
}

void GLWaterPlane::build(RawHeightMap heightmap, float y) {
    m_heightmap = heightmap;
    vertexNumber = m_heightmap.getWidth()*(m_heightmap.getHeight()-1)*2;
    vertexes = new Vertex[vertexNumber];
    for (int i = 0; i < m_heightmap.getHeight(); i++) {
        for (int j = 0; j < m_heightmap.getWidth(); j++) {
            int offset = i*m_heightmap.getWidth()+j;
            vertexes[offset].x = i * m_scaleFactor * 8 * 1/heightmap.getRatio();
            vertexes[offset].y = y;
            vertexes[offset].z = j * m_scaleFactor * 8 * 1/heightmap.getRatio();
        }
    }
    generateTexCoords();
    generateIndexes();
}
