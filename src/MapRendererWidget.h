// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef MAPRENDERERWIDGET_H
#define MAPRENDERERWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QImage>
#include <QVector>
#include "RawHeightMap.h"
#include "glextensions.h"

class Vertex {
public:
    float x;
    float y;
    float z;
    Vertex();
    Vertex(float x, float y, float z);
    void setXYZ(float x, float y, float z);
    void add(Vertex v2);
    void sub(Vertex v2);
    static Vertex getNormal(Vertex& v1, Vertex& v2);
    static Vertex getNormal(Vertex& a, Vertex& b, Vertex& c);
    void normalize();
};

struct TexCoord {
    float u;
    float v;
};

class MapRendererWidget : public QGLWidget {
public:
    MapRendererWidget(QWidget* parent = 0);
    ~MapRendererWidget();
    void setSource(QString mapName, QImage minimap, QImage metalmap, RawHeightMap heightmap);
    void setDrawStartPositions(bool b);

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void addStartRect(int ally, QRect r);
    void setMyAllyTeam(int n);
    void removeStartRect(int ally);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent * event);
private:
    void normalizeAngle(int *angle);
    void makeObject();
    void computeNormals();
    void generateIndexes();
    void generateTexCoords();
    void drawStartRecs();
private:
    bool m_computedNormals;
    QString currentMap;
    unsigned int* m_indexes;
    int m_numIndexes;
    GLuint m_texture;
    bool compileObject;
    QPoint lastPos;
    float lastZoom;
    QImage m_minimap;
    QImage m_metalmap;
    QImage m_withRects;
    RawHeightMap m_heightmap;
    QVector<QVector<Vertex> > vertexes;
    unsigned int m_vertexNumber;
    Vertex* m_vertexes;
    Vertex* m_normals;
    TexCoord* m_texCoords;
    unsigned int m_VBOVertices;
    unsigned int m_VBOTexCoords;
    unsigned int m_VBONormals;
    bool blockRerender;
    int xRot;
    int yRot;
    int zRot;
    float dx;
    float dy;
    QMap<int, QRect> startRects;
    int myAlly;
    bool m_redrawStartRects;
    bool m_drawStartPositions;
    Qt::BrushStyle m_brushStyle;
    int m_borderWidth;
    int m_alpha;
};

inline void MapRendererWidget::setDrawStartPositions(bool b) {
    m_drawStartPositions = b;
}

#endif // MAPRENDERERWIDGET_H
