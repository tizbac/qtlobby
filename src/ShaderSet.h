#ifndef SHADER_H
#define SHADER_H

#include <QObject>
#include <QString>
#include "glextensions.h"


class ShaderSet : QObject {
public:
    ShaderSet();
    bool loadShaders(QString filename);
    GLint getUniformLocation(QString str);
    QString getErrorMessage();
    void use();
    void stop();
private:
    GLhandleARB m_vertexShader;
    GLhandleARB m_fragmentShader;
    GLhandleARB m_program;
    QString m_error;
};

inline GLint ShaderSet::getUniformLocation(QString str) {
    QByteArray data = str.toAscii();
    return glGetUniformLocationARB(m_program, data.constData());
}

inline QString ShaderSet::getErrorMessage() {
    return m_error;
}

inline void ShaderSet::use() {
    glUseProgramObjectARB(m_program);
}

inline void ShaderSet::stop() {
    glUseProgramObjectARB(0);
}

#endif // SHADER_H
