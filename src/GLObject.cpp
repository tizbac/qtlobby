#include "GLObject.h"
#include "glextensions.h"

GLObject::GLObject() {
    m_BeginMode = GL_TRIANGLE_STRIP;
    vertexes = 0;
    normals = 0;
    texCoords = 0;
    indexes = 0;
}

GLObject::~GLObject() {
    free();
}

void GLObject::draw() {
    if (getGLExtensionFunctions().openGL15Supported()) {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
        glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );

        glBindBuffer(GL_ARRAY_BUFFER, m_VBONormals);
        glNormalPointer(GL_FLOAT, 0, (char *) NULL );

        glBindBuffer( GL_ARRAY_BUFFER, m_VBOTexCoords );
        glTexCoordPointer( 2, GL_FLOAT, 0, (char *) NULL );
    } else {
        glVertexPointer( 3, GL_FLOAT, 0, vertexes );
        glNormalPointer(GL_FLOAT, 0, normals );
        glTexCoordPointer( 2, GL_FLOAT, 0, texCoords );
    }
    glDrawElements(m_BeginMode, numIndexes, GL_UNSIGNED_INT, indexes);
}

void GLObject::compile() {
    if (getGLExtensionFunctions().openGL15Supported()) {
        glGenBuffers( 1, &m_VBOVertices );
        glBindBuffer( GL_ARRAY_BUFFER, m_VBOVertices );
        glBufferData( GL_ARRAY_BUFFER, vertexNumber*sizeof(Vertex), vertexes, GL_STATIC_DRAW );

        glGenBuffers( 1, &m_VBONormals );
        glBindBuffer( GL_ARRAY_BUFFER, m_VBONormals );
        glBufferData( GL_ARRAY_BUFFER, vertexNumber*sizeof(Vertex), normals, GL_STATIC_DRAW );

        glGenBuffers( 1, &m_VBOTexCoords );
        glBindBuffer( GL_ARRAY_BUFFER, m_VBOTexCoords );
        glBufferData( GL_ARRAY_BUFFER, vertexNumber*sizeof(TexCoord), texCoords, GL_STATIC_DRAW );
    }
}

void GLObject::free() {
    if (getGLExtensionFunctions().openGL15Supported()) {
        glDeleteBuffers(1, &m_VBOVertices);
        glDeleteBuffers(1, &m_VBONormals);
        glDeleteBuffers(1, &m_VBOTexCoords);
    }
    if(vertexes) delete vertexes;
    vertexes = 0;
    vertexNumber = 0;
    if(normals) delete normals;
    normals = 0;
    if(texCoords) delete texCoords;
    texCoords = 0;
    if(indexes) delete indexes;
    indexes = 0;
    numIndexes = 0;
}

void GLObject::freeUnusedData() {
    if (getGLExtensionFunctions().openGL15Supported()) {
        if(vertexes) delete vertexes;
        vertexes = 0;
        if(normals) delete normals;
        normals = 0;
        if(texCoords) delete texCoords;
        texCoords = 0;
    }
}


/*****************************************************************
*************************** Vertex *******************************
******************************************************************/

Vertex::Vertex() {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vertex::Vertex(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vertex::setXYZ(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vertex Vertex::getNormal(Vertex& v1, Vertex& v2) {
    Vertex n(v1.y*v2.z - v2.y*v1.z, v1.z*v2.x - v2.z*v1.x, v1.x*v2.y - v2.x*v1.y);
    n.normalize();
    return n;

}

Vertex Vertex::getNormal(Vertex& a, Vertex& b, Vertex& c) {
    Vertex n;
    Vertex v1, v2;
    v1.setXYZ(b.x - a.x, b.y - a.y, b.z - a.z);
    v2.setXYZ(c.x - a.x, c.y - a.y, c.z - a.z);
    n.setXYZ(v1.y*v2.z - v2.y*v1.z, v1.z*v2.x - v2.z*v1.x, v1.x*v2.y - v2.x*v1.y);
    n.normalize();
    return n;
}

void Vertex::normalize() {
    float Mn = sqrtf(x*x + y*y + z*z);
    x /= Mn;
    y /= Mn;
    z /= Mn;
}

void Vertex::add(Vertex v2) {
    x += v2.x;
    y += v2.y;
    z += v2.z;
}

void Vertex::sub(Vertex v2) {
    x -= v2.x;
    y -= v2.y;
    z -= v2.z;
}


