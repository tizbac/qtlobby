#ifndef MAPRENDERERWIDGET_H
#define MAPRENDERERWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QImage>
#include <QVector>
#include "RawHeightMap.h"


#if _MSC_VER
//fuck msvc
#define GL_ARRAY_BUFFER 0x8892
#define GL_STATIC_DRAW 0x88E4

typedef ptrdiff_t GLsizeiptrARB;
#endif

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
    void setSource(QString mapName, QImage minimap, RawHeightMap heightmap);

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
    typedef void (APIENTRY *_glGenBuffers) (GLsizei, GLuint *);
    typedef void (APIENTRY *_glBindBuffer) (GLenum, GLuint);
    typedef void (APIENTRY *_glBufferData) (GLenum, GLsizeiptrARB, const GLvoid *, GLenum);
    typedef void (APIENTRY *_glDeleteBuffers) (GLsizei, const GLuint *);

    void normalizeAngle(int *angle);
    GLuint makeObject();
    bool resolve();
    void computeNormals();
    void generateIndexes();
    void generateTexCoords();

private:
    _glGenBuffers GenBuffers;
    _glBindBuffer BindBuffer;
    _glBufferData BufferData;
    _glDeleteBuffers DeleteBuffers;
    bool m_vbo;
    bool m_computedNormals;
    QString currentMap;
    unsigned int* m_indexes;
    int m_numIndexes;
    GLuint m_texture;
    bool compileObject;
    QPoint lastPos;
    float lastZoom;
    QImage m_minimap;
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
};

#endif // MAPRENDERERWIDGET_H
