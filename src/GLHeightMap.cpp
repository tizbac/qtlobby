#include "GLHeightMap.h"

GLHeightMap::GLHeightMap() {
    m_scaleFactor = 0.01;
}


#define V(i,j) vertexes[(i)*m_heightmap.getWidth()+(j)]
#define N(i,j) normals[(i)*m_heightmap.getWidth()+(j)]

void GLHeightMap::computeNormals() {
    normals = new Vertex[vertexNumber];
    memset(normals, 0, vertexNumber*sizeof(Vertex));
    for (int i = 0; i < m_heightmap.getHeight(); i++) {
        for (int j = 0; j < m_heightmap.getWidth(); j++) {
            Vertex left, right, top, bottom, current;
            current = V(i,j);
            if (i > 0) left = V(i-1,j);
            if (j > 0) top = V(i,j-1);
            if (i < m_heightmap.getHeight() - 1) right = V(i+1,j);
            if (j < m_heightmap.getWidth() - 1) bottom = V(i,j+1);
            N(i,j).add(Vertex::getNormal(current, top, left));
            N(i,j).add(Vertex::getNormal(current, left, bottom));
            N(i,j).add(Vertex::getNormal(current, bottom, right));
            N(i,j).add(Vertex::getNormal(current, right, top));
            N(i,j).normalize();
        }
    }
}

void GLHeightMap::generateIndexes() {
    numIndexes = (m_heightmap.getWidth() * 2) * (m_heightmap.getHeight() - 1) + (m_heightmap.getHeight() - 2);

    indexes = new unsigned int[numIndexes];

    int index = 0;
    for ( int z = 0; z < m_heightmap.getHeight() - 1; z++ ) {
        // Even rows move left to right, odd rows move right to left.
        if ( z % 2 == 0 ) {
            // Even row
            int x;
            for ( x = 0; x < m_heightmap.getWidth(); x++ ) {
                indexes[index++] = x + (z * m_heightmap.getWidth()) + m_heightmap.getWidth();
                indexes[index++] = x + (z * m_heightmap.getWidth());
            }
            // Insert degenerate vertex if this isn't the last row
            if ( z != m_heightmap.getHeight() - 2) {
                indexes[index++] = --x + (z * m_heightmap.getWidth());
            }
        } else {
            // Odd row
            int x;
            for ( x = m_heightmap.getWidth() - 1; x >= 0; x-- ) {
                indexes[index++] = x + (z * m_heightmap.getWidth()) + m_heightmap.getWidth();
                indexes[index++] = x + (z * m_heightmap.getWidth());
            }
            // Insert degenerate vertex if this isn't the last row
            if ( z != m_heightmap.getHeight() - 2) {
                indexes[index++] = ++x + (z * m_heightmap.getWidth());
            }
        }
    }
}

void GLHeightMap::generateTexCoords() {
    texCoords = new TexCoord[vertexNumber];
    for (int i = 0; i < m_heightmap.getHeight(); i++) {
        for (int j = 0; j < m_heightmap.getWidth(); j++) {
            texCoords[i*m_heightmap.getWidth()+j].u = (float)j/m_heightmap.getWidth();
            texCoords[i*m_heightmap.getWidth()+j].v = m_heightmap.getHeight() - (float)i/m_heightmap.getHeight();
        }
    }
}

void GLHeightMap::build(RawHeightMap heightmap) {
    m_heightmap = heightmap;
    vertexNumber = heightmap.getWidth()*(heightmap.getHeight()-1)*2;
    vertexes = new Vertex[vertexNumber];
    float range = m_heightmap.getMaxHeight() - m_heightmap.getMinHeight();
    for (int i = 0; i < heightmap.getHeight(); i++) {
        for (int j = 0; j < heightmap.getWidth(); j++) {
            unsigned short int value = heightmap.getData()[i*heightmap.getWidth()+j];
            int offset = i*heightmap.getWidth()+j;
            vertexes[offset].x = i * m_scaleFactor * 8;
            vertexes[offset].y = (m_heightmap.getMinHeight()+value*range/MAX_SHORT)*m_heightmap.getRatio()*m_scaleFactor;
            vertexes[offset].z = j * m_scaleFactor * 8;
        }
    }
    computeNormals();
    generateIndexes();
    generateTexCoords();
}
