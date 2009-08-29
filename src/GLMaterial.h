#ifndef GLMATERIAL_H
#define GLMATERIAL_H

#include <QtOpenGL>

class GLMaterial {
public:
    GLMaterial();
    void setAmbient(GLfloat r,GLfloat g,GLfloat b,GLfloat a);
    void setDiffuse(GLfloat r,GLfloat g,GLfloat b,GLfloat a);
    void setSpecular(GLfloat r,GLfloat g,GLfloat b,GLfloat a);
    void setShininess(GLfloat s);
    void apply();
private:
    GLfloat m_ambient[4];
    GLfloat m_diffuse[4];
    GLfloat m_specular[4];
    GLfloat m_shininess;
};

inline void GLMaterial::setAmbient(GLfloat r,GLfloat g,GLfloat b,GLfloat a) {
    m_ambient[0] = r;
    m_ambient[1] = g;
    m_ambient[2] = b;
    m_ambient[3] = a;
}

inline void GLMaterial::setDiffuse(GLfloat r,GLfloat g,GLfloat b,GLfloat a) {
    m_diffuse[0] = r;
    m_diffuse[1] = g;
    m_diffuse[2] = b;
    m_diffuse[3] = a;
}

inline void GLMaterial::setSpecular(GLfloat r,GLfloat g,GLfloat b,GLfloat a) {
    m_specular[0] = r;
    m_specular[1] = g;
    m_specular[2] = b;
    m_specular[3] = a;
}

inline void GLMaterial::setShininess(GLfloat s) {
    m_shininess = s;
}


#endif // GLMATERIAL_H
