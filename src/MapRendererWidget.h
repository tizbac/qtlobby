#ifndef MAPRENDERERWIDGET_H
#define MAPRENDERERWIDGET_H

#include <QGLWidget>
#include <QImage>
#include <QVector>
#include "RawHeightMap.h"

struct Vertex {
    float x;
    float y;
    float z;
};

class MapRendererWidget : public QGLWidget {
public:
    MapRendererWidget(QWidget* parent = 0);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void setSource(QImage minimap, RawHeightMap heightmap);
private:
    QImage m_minimap;
    RawHeightMap m_heightmap;
    QVector<QVector<Vertex> > vertexes;
};

#endif // MAPRENDERERWIDGET_H
