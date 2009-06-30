#include "MapRendererWidget.h"

#define CELL_SIZE 5;
#define MAX_HEIGHT 15;
#define MAX_SHORT 65535

MapRendererWidget::MapRendererWidget(QWidget* parent) : QGLWidget(parent) {
}

void MapRendererWidget::initializeGL() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
}

void MapRendererWidget::resizeGL(int w, int h) {
    glViewport(0, 0, (GLint)w, (GLint)h);
}

void MapRendererWidget::paintGL() {
    if(!m_heightmap.getWidth()) return;
    glBegin(GL_TRIANGLES);
    for(int i = 1; i < m_heightmap.getHeight(); i++) {
        for(int j = 1; j < m_heightmap.getWidth(); j++) {
            Vertex v00 = vertexes[i-1][j-1];
            Vertex v01 = vertexes[i][j-1];
            Vertex v10 = vertexes[i-1][j];
            Vertex v11 = vertexes[i][j];

            glVertex3f(v00.x, v00.y, v00.z);
            glVertex3f(v01.x, v01.y, v01.z);
            glVertex3f(v11.x, v11.y, v11.z);

            glVertex3f(v00.x, v00.y, v00.z);
            glVertex3f(v10.x, v10.y, v10.z);
            glVertex3f(v11.x, v11.y, v11.z);
        }
    }
    glEnd();
}

void MapRendererWidget::setSource(QImage minimap, RawHeightMap heightmap) {
    m_minimap = minimap;
    m_heightmap.free();
    m_heightmap = heightmap;
    vertexes.clear();
    for(int i = 0; i < heightmap.getHeight(); i++) {
        QVector<Vertex> line;
        for(int j = 0; j < heightmap.getWidth(); j++) {
            unsigned short int value = heightmap.getData()[i*heightmap.getWidth()+j];
            Vertex v;
            v.x = i * CELL_SIZE;
            v.y = j * CELL_SIZE;
            v.z = value/MAX_SHORT*MAX_HEIGHT;
            line << v;
        }
        vertexes << line;
    }
}
