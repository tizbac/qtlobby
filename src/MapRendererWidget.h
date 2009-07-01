#ifndef MAPRENDERERWIDGET_H
#define MAPRENDERERWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QImage>
#include <QVector>
#include "RawHeightMap.h"

class Vertex {
public:
    float x;
    float y;
    float z;
    Vertex();
    Vertex(float x, float y, float z);
    void setXYZ(float x, float y, float z);
    void add(Vertex& v2);
    void sub(Vertex& v2);
    static Vertex getNormal(Vertex& v1, Vertex& v2);
    static Vertex getNormal(Vertex& a, Vertex& b, Vertex& c);
    void normalize();
};

class MapRendererWidget : public QGLWidget {
public:
    MapRendererWidget(QWidget* parent = 0);
    void setSource(QImage minimap, RawHeightMap heightmap);

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent * event);
private:
    void normalizeAngle(int *angle);
    GLuint makeObject();

private:
    GLuint object;
    bool compileObject;
    QPoint lastPos;
    float lastZoom;
    QImage m_minimap;
    RawHeightMap m_heightmap;
    QVector<QVector<Vertex> > vertexes;
    int xRot;
    int yRot;
    int zRot;
};

#endif // MAPRENDERERWIDGET_H
