#ifndef GLMATERIAL_H
#define GLMATERIAL_H

#include <QtOpenGL>

class GLMaterial {
public:
    GLMaterial();
    void setAmbient(GLfloat r,GLfloat g,GLfloat b,GLfloat a);
    const GLfloat* getAmbient() const;
    void setDiffuse(GLfloat r,GLfloat g,GLfloat b,GLfloat a);
    const GLfloat* getDiffuese() const;
    void setSpecular(GLfloat r,GLfloat g,GLfloat b,GLfloat a);
    const GLfloat* getSpecular() const;
    void setShininess(GLfloat s);
    GLfloat getShininess() const;
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

inline const GLfloat* GLMaterial::getAmbient() const {
    return m_ambient;
}

inline const GLfloat* GLMaterial::getDiffuese() const {
    return m_diffuse;
}


inline const GLfloat* GLMaterial::getSpecular() const {
    return m_specular;
}


inline GLfloat GLMaterial::getShininess() const {
    return m_shininess;
}


#endif // GLMATERIAL_H
