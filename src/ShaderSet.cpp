#include "ShaderSet.h"
#include <QFile>

ShaderSet::ShaderSet() {
}

bool ShaderSet::loadShaders(QString filename) {
    QFile f(filename);
    if(!f.exists()) {
        m_error = tr("Requested shader file does not exists") + ": " + filename;
        return false;
    }
    if(!f.open(QIODevice::ReadOnly)) {
        m_error = tr("Unable to open requested shader file") + ": " + filename;
        return false;
    }
    QString line;
    QString vertex;
    QString fragment;
    QString* target = 0;
    while(!(line = f.readLine()).isEmpty()) {
        if(line == "[VertexShader]\n") {
            target = &vertex;
            continue;
        } else if(line == "[FragmentShader]\n") {
            target = &fragment;
            continue;
        } else if(line == "[Parameters]\n") {
            break;
        }
        if(target)
            target->append(line);
    }
    f.close();
    const char* shaderStrings[1];
    char str[512];
    // Create the vertex shader.
    m_vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    QByteArray data = vertex.toAscii();
    shaderStrings[0] = data.constData();
    glShaderSourceARB(m_vertexShader, 1, shaderStrings, NULL );
    glCompileShaderARB(m_vertexShader);
    GLint compiled;
    glGetObjectParameterivARB( m_vertexShader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled );
    if(compiled  == GL_FALSE) {
        glGetInfoLogARB(m_vertexShader, sizeof(str), NULL, str);
        m_error = tr("Error compiling vertex shader:\n");
        m_error += str;
        return false;
    }
    // Create the fragment shader.
    m_fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
    data = fragment.toAscii();
    shaderStrings[0] = data.constData();
    glShaderSourceARB(m_fragmentShader, 1, shaderStrings, NULL );
    glCompileShaderARB(m_fragmentShader);
    glGetObjectParameterivARB( m_fragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled );
    if(compiled  == GL_FALSE) {
        glGetInfoLogARB(m_fragmentShader, sizeof(str), NULL, str);
        m_error = tr("Error compiling fragment shader:\n");
        m_error += str;
        return false;
    }
    // Create a program object and attach the two compiled shaders.
    m_program = glCreateProgramObjectARB();
    glAttachObjectARB( m_program, m_vertexShader );
    glAttachObjectARB( m_program, m_fragmentShader );

    // Link the program object and print out the info log.
    GLint linked;
    glLinkProgramARB( m_program );
    glGetObjectParameterivARB( m_program, GL_OBJECT_LINK_STATUS_ARB, &linked );

    if( linked == GL_FALSE ) {
        glGetInfoLogARB( m_program, sizeof(str), NULL, str );
        m_error = tr("Error linking program:\n");
        m_error += str;
        return false;
    }
    return true;
}
