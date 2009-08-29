#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <QtOpenGL>

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


class GLObject {
public:
    GLObject();
    ~GLObject();
    void draw();
    void setBeginMode(GLint mode);
    void compile();
    void free();
    void freeUnusedData();
protected:
    unsigned int* indexes;
    int numIndexes;
    bool computedNormals;
    bool compileObject;
    unsigned int vertexNumber;
    Vertex* vertexes;
    Vertex* normals;
    TexCoord* texCoords;
private:
    unsigned int m_VBOVertices;
    unsigned int m_VBOTexCoords;
    unsigned int m_VBONormals;
    GLint m_BeginMode;
};

inline void GLObject::setBeginMode(GLint mode) {
    m_BeginMode = mode;
}

#define MAX_SHORT 65535

#endif // GLOBJECT_H
