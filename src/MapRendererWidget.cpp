#include "MapRendererWidget.h"
#include <cmath>

#define CELL_SIZE 0.1
#define MAX_HEIGHT 15
#define MAX_SHORT 65535

MapRendererWidget::MapRendererWidget(QWidget* parent) : QGLWidget(parent) {
    xRot = 0;
    yRot = 0;
    zRot = 0;
    lastZoom = 1.0;
    compileObject = false;
}

void MapRendererWidget::initializeGL() {
    GLfloat mat_ambient[] = { 0.8, 0.2, 0.2, 1.0 };
    GLfloat mat_specular[] = { 1.0, 0.8, 0.8, 0.0 };
    GLfloat mat_shininess[] = { 30.0 };
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void MapRendererWidget::resizeGL(int w, int h) {
    glViewport(0, 0, (GLint)w, (GLint)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho (-100, 100, -100*(GLfloat)h/(GLfloat)w,
                 100*(GLfloat)h/(GLfloat)w, -200.0, 200.0);
    else
        glOrtho (-100*(GLfloat)w/(GLfloat)h,
                 100*(GLfloat)w/(GLfloat)h, -100, 100, -200.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    /*float mapRadius = sqrt(pow(m_heightmap.getWidth()*CELL_SIZE, 2)+pow(m_heightmap.getWidth()*CELL_SIZE, 2));
    float scale = 1;
    if(mapRadius > 100) {
        scale = 100 / mapRadius;
    } else {
        scale = mapRadius / 100;
    }
    glScalef(scale, scale, scale);
    //glTranslated(-m_heightmap.getWidth()*CELL_SIZE/2, scale*m_heightmap.getHeight()*CELL_SIZE/2, 0);*/
}

void MapRendererWidget::paintGL() {
    if(!m_heightmap.getWidth()) return;
    GLfloat light_position[] = { 5, 5, MAX_HEIGHT*1.3, 0.0 };
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(-m_heightmap.getWidth()*CELL_SIZE/2., m_heightmap.getHeight()*CELL_SIZE/2., 0);
    glRotatef(-90, 0, 0, 1);
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
    glScaled(lastZoom, lastZoom, lastZoom);
    if(compileObject) {
        object = makeObject();
        compileObject = false;
    }
    glCallList(object);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

GLuint MapRendererWidget::makeObject() {
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    for(int i = 1; i < m_heightmap.getHeight(); i++) {
        for(int j = 1; j < m_heightmap.getWidth(); j++) {
            Vertex v00 = vertexes[i-1][j-1];
            Vertex v01 = vertexes[i][j-1];
            Vertex v10 = vertexes[i-1][j];
            Vertex v11 = vertexes[i][j];
            Vertex n = Vertex::getNormal(v00, v01, v11);

            glNormal3f(n.x, n.y, n.z);
            glVertex3f(v00.x, v00.y, v00.z);
            glVertex3f(v01.x, v01.y, v01.z);
            glVertex3f(v11.x, v11.y, v11.z);

            glVertex3f(v00.x, v00.y, v00.z);
            glVertex3f(v10.x, v10.y, v10.z);
            glVertex3f(v11.x, v11.y, v11.z);
        }
    }
    glEnd();
    glEndList();
    return list;
}

void MapRendererWidget::setSource(QImage minimap, RawHeightMap heightmap) {
    m_minimap = minimap;
    m_heightmap.free();
    m_heightmap = heightmap;
    vertexes.clear();
    for(int i = 0; i < heightmap.getHeight(); i++) {
        QVector<Vertex> line;
        for(int j = 0; j < heightmap.getWidth(); j++) {
            unsigned short int value = heightmap.getData()[i*heightmap.getWidth()+j];
            Vertex v(i * CELL_SIZE, j * CELL_SIZE, value/(float)MAX_SHORT*MAX_HEIGHT);
            line << v;
        }
        vertexes << line;
    }
    compileObject = true;
    updateGL();
}


void MapRendererWidget::setXRotation(int angle) {
     normalizeAngle(&angle);
     if (angle != xRot) {
         xRot = angle;
         updateGL();
     }
 }

void MapRendererWidget::setYRotation(int angle) {
     normalizeAngle(&angle);
     if (angle != yRot) {
         yRot = angle;
         updateGL();
     }
}

 void MapRendererWidget::setZRotation(int angle) {
     normalizeAngle(&angle);
     if (angle != zRot) {
         zRot = angle;
         updateGL();
     }
}

void MapRendererWidget::normalizeAngle(int *angle) {
     while (*angle < 0)
         *angle += 360 * 16;
     while (*angle > 360 * 16)
         *angle -= 360 * 16;
}

void MapRendererWidget::wheelEvent ( QWheelEvent * event ) {
    if(lastZoom + event->delta() * 0.002 < 0.5 || lastZoom + event->delta() * 0.002 > 2) return;
    lastZoom += event->delta() * 0.002;
    updateGL();
}

void MapRendererWidget::mousePressEvent(QMouseEvent *event) {
     lastPos = event->pos();
}

void MapRendererWidget::mouseMoveEvent(QMouseEvent *event) {
     int dx = event->x() - lastPos.x();
     int dy = event->y() - lastPos.y();

     if (event->buttons() & Qt::LeftButton) {
         //setXRotation(xRot + 8 * dy);
         setYRotation(yRot + 8 * dx);
     } else if (event->buttons() & Qt::RightButton) {
         //setXRotation(xRot + 8 * dy);
         setZRotation(zRot + 8 * dx);
     }
     lastPos = event->pos();
}



// Vertex

Vertex::Vertex() {}

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
    float Mn = sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
    x /= Mn;
    y /= Mn;
    z /= Mn;
}

void Vertex::add(Vertex& v2) {
    x += v2.x;
    y += v2.y;
    z += v2.z;
}

void Vertex::sub(Vertex& v2) {
    x -= v2.x;
    y -= v2.y;
    z -= v2.z;
}

