// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "MapRendererWidget.h"
#include "GLProgressDialog.h"
#include <cmath>
#include <QDebug>
#include <QApplication>
#include "Settings.h"


#define RESOLVE_GL_FUNC(f) ok &= bool((f = (_gl##f) context()->getProcAddress(QLatin1String("gl" #f))));

#define SCALE_HEIGHTMAP 0.01
#define CELL_SIZE 8*SCALE_HEIGHTMAP
#define MAX_SHORT 65535

MapRendererWidget::MapRendererWidget(QWidget* parent) : QGLWidget(parent) {
    xRot = 0;
    yRot = 0;
    zRot = 0;
    dx = 0;
    dy = 0;
    lastZoom = 1.0;
    compileObject = false;
    blockRerender = false;
    m_vertexes = 0;
    m_normals = 0;
    m_texCoords = 0;
    m_computedNormals = false;
    m_indexes = 0;
    getGLExtensionFunctions().resolve(context());
    m_redrawStartRects = true;
    setAutoBufferSwap(false);
}

MapRendererWidget::~MapRendererWidget() {
    if (getGLExtensionFunctions().openGL15Supported()) {
        glDeleteBuffers(1, &m_VBOVertices);
        glDeleteBuffers(1, &m_VBONormals);
        glDeleteBuffers(1, &m_VBOTexCoords);
    }
    if (m_vertexes) delete m_vertexes;
    if (m_texCoords) delete m_texCoords;
    if (m_indexes) delete m_indexes;
}

void MapRendererWidget::initializeGL() {
    //GLfloat mat_ambient[] = { 0.8, 0.2, 0.2, 1.0 };
    //GLfloat mat_specular[] = { 1.0, 0.8, 0.8, 0.0 };
    //GLfloat mat_shininess[] = { 100.0 };
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);

    //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_ambient);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    //glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glBlendFunc(GL_ONE,GL_ONE);
    //glEnable(GL_ALPHA_TEST);
    //glDepthMask(GL_FALSE);
}

void MapRendererWidget::resizeGL(int w, int h) {
    glViewport(0, 0, (GLint)w, (GLint)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();

    const float angle=30;
    gluPerspective(angle,double(w)/(double(h),1,1000);

    //if (w <= h)
    //    glOrtho (dx+lastZoom*-100, dx+lastZoom*100,/*left,right*/
    //             dy+lastZoom*-100*(GLfloat)h/(GLfloat)w, dy+lastZoom*100*(GLfloat)h/(GLfloat)w,/*top,bottom*/
    //             -2000.0, 2000.0);/*near,far*/
    //else
    //    glOrtho (dx+lastZoom*-100*(GLfloat)w/(GLfloat)h, dx+lastZoom*100*(GLfloat)w/(GLfloat)h,/*left,right*/
    //             dy+lastZoom*-100, dy+lastZoom*100,/*top,bottom*/
    //             -2000.0, 2000.0);/*near,far*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MapRendererWidget::paintGL() {
    if (!m_heightmap.getWidth() || blockRerender) return;
    m_time.start();
    //GLfloat light_position[] = { 5, 5, MAX_HEIGHT*1.3, 0.0 };
    //Weird, but this line doesn't work for my i915 integrated graphics, tho next 2 work fine
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    if (compileObject) {
        makeObject();
        compileObject = false;
    }
    glRotatef(-90, 0, 0, 1);
    glTranslatef(0,0,-(lastZoom*100));
    glTranslatef(dy,-dx,0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);

    glTranslatef(-m_heightmap.getHeight()*CELL_SIZE/2., -m_heightmap.getWidth()*CELL_SIZE/2., 0);
    //glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    //Water rendering
    glBegin(GL_QUADS);
    glColor4f(0, 0, 1, 0.7);
    glVertex3f(0,0,0);
    glVertex3f(0, m_heightmap.getWidth()*CELL_SIZE,0);
    glVertex3f(m_heightmap.getHeight() * CELL_SIZE, m_heightmap.getWidth()*CELL_SIZE,0);
    glVertex3f(m_heightmap.getHeight() * CELL_SIZE, 0,0);
    glEnd();

    glColor4f(1, 1, 1, 1);
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    //glEnableClientState( GL_NORMAL_ARRAY );

    if (getGLExtensionFunctions().openGL15Supported()) {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
        glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );

        //glBindBuffer(GL_ARRAY_BUFFER, m_VBONormals);
        //glNormalPointer(GL_FLOAT, 0, (char *) NULL );

        glBindBuffer( GL_ARRAY_BUFFER, m_VBOTexCoords );
        glTexCoordPointer( 2, GL_FLOAT, 0, (char *) NULL );
    } else {
        glVertexPointer( 3, GL_FLOAT, 0, m_vertexes );
        //glNormalPointer(GL_FLOAT, 0, m_normals );
        glTexCoordPointer( 2, GL_FLOAT, 0, m_texCoords );
    }

    if(m_redrawStartRects) drawStartRecs();
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glDrawElements(GL_TRIANGLE_STRIP, m_numIndexes, GL_UNSIGNED_INT, m_indexes);

    glDisableClientState( GL_VERTEX_ARRAY );
    //glDisableClientState( GL_NORMAL_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    swapBuffers();
    int msecs = m_time.elapsed();
    if(msecs)
        emit updateDebugInfo(m_debugInfo.arg(1000/msecs));
    else
        emit updateDebugInfo(m_debugInfo.arg(tr("Inf")));
}

void MapRendererWidget::generateTexCoords() {
    if (m_texCoords) delete m_texCoords;
    m_texCoords = new TexCoord[m_vertexNumber];
    for (int i = 0; i < m_heightmap.getHeight(); i++) {
        for (int j = 0; j < m_heightmap.getWidth(); j++) {
            m_texCoords[i*m_heightmap.getWidth()+j].u = (float)j/m_heightmap.getWidth();
            m_texCoords[i*m_heightmap.getWidth()+j].v = m_heightmap.getHeight() - (float)i/m_heightmap.getHeight();
        }
    }
}

void MapRendererWidget::generateIndexes() {
    m_numIndexes = (m_heightmap.getWidth() * 2) * (m_heightmap.getHeight() - 1) + (m_heightmap.getHeight() - 2);

    if (m_indexes) delete m_indexes;
    m_indexes = new unsigned int[m_numIndexes];

    int index = 0;
    for ( int z = 0; z < m_heightmap.getHeight() - 1; z++ ) {
        // Even rows move left to right, odd rows move right to left.
        if ( z % 2 == 0 ) {
            // Even row
            int x;
            for ( x = 0; x < m_heightmap.getWidth(); x++ ) {
                m_indexes[index++] = x + (z * m_heightmap.getWidth());
                m_indexes[index++] = x + (z * m_heightmap.getWidth()) + m_heightmap.getWidth();
            }
            // Insert degenerate vertex if this isn't the last row
            if ( z != m_heightmap.getHeight() - 2) {
                m_indexes[index++] = --x + (z * m_heightmap.getWidth());
            }
        } else {
            // Odd row
            int x;
            for ( x = m_heightmap.getWidth() - 1; x >= 0; x-- ) {
                m_indexes[index++] = x + (z * m_heightmap.getWidth());
                m_indexes[index++] = x + (z * m_heightmap.getWidth()) + m_heightmap.getWidth();
            }
            // Insert degenerate vertex if this isn't the last row
            if ( z != m_heightmap.getHeight() - 2) {
                m_indexes[index++] = ++x + (z * m_heightmap.getWidth());
            }
        }
    }
}

void MapRendererWidget::makeObject() {
    blockRerender = true;
    /*GLProgressDialog* progress = new GLProgressDialog(this);
    progress->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    progress->setMax(m_heightmap.getHeight());
    progress->updateProgress(0);
    progress->show();
    QApplication::processEvents();*/
    if (getGLExtensionFunctions().openGL15Supported()) {
        glGenBuffers( 1, &m_VBOVertices );
        glBindBuffer( GL_ARRAY_BUFFER, m_VBOVertices );
        glBufferData( GL_ARRAY_BUFFER, m_vertexNumber*sizeof(Vertex), m_vertexes, GL_STATIC_DRAW );
    }
    //computeNormals();
    //if(getGLExtensionFunctions().openGL15Supported()) {
    //GenBuffers( 1, &m_VBONormals );
    //BindBuffer( GL_ARRAY_BUFFER, m_VBONormals );
    //BufferData( GL_ARRAY_BUFFER, m_vertexNumber*sizeof(Vertex), m_normals, GL_STATIC_DRAW );
    //}
    generateTexCoords();
    if (getGLExtensionFunctions().openGL15Supported()) {
        glGenBuffers( 1, &m_VBOTexCoords );
        glBindBuffer( GL_ARRAY_BUFFER, m_VBOTexCoords );
        glBufferData( GL_ARRAY_BUFFER, m_vertexNumber*sizeof(TexCoord), m_texCoords, GL_STATIC_DRAW );
    }
    m_texture = bindTexture(QPixmap::fromImage(m_minimap), GL_TEXTURE_2D);
    generateIndexes();
    //QApplication::processEvents();
    //progress->hide();
    //delete progress;
    m_heightmap.free();
    if (getGLExtensionFunctions().openGL15Supported()) {
        delete m_vertexes;
        m_vertexes = 0;
        delete m_texCoords;
        m_texCoords = 0;
    }
    blockRerender = false;
}


#define V(i,j) m_vertexes[(i)*m_heightmap.getWidth()+(j)]
#define N(i,j) m_normals[(i)*m_heightmap.getWidth()+(j)]

void MapRendererWidget::computeNormals() {
    if (m_normals) delete m_normals;
    Vertex* m_normals = new Vertex[m_vertexNumber];
    memset(m_normals, 0, m_vertexNumber*sizeof(Vertex));
    for (int i = 0; i < m_heightmap.getHeight(); i++) {
        for (int j = 0; j < m_heightmap.getWidth(); j++) {
            Vertex left, right, top, bottom, current;
            current = V(i,j);
            if (i > 0) left = V(i-1,j);
            if (j > 0) top = V(i,j-1);
            if (i < m_heightmap.getHeight() - 1) right = V(i+1,j);
            if (j < m_heightmap.getWidth() - 1) bottom = V(i,j+1);
            N(i,j).add(Vertex::getNormal(current, left, top));
            N(i,j).add(Vertex::getNormal(current, left, bottom));
            N(i,j).add(Vertex::getNormal(current, right, top));
            N(i,j).add(Vertex::getNormal(current, right, bottom));
            N(i,j).normalize();
        }
    }
}

void MapRendererWidget::setSource(QString mapName, QImage minimap, QImage metalmap, RawHeightMap heightmap) {
    if (currentMap == mapName) return;
    currentMap = mapName;
    m_minimap = minimap;
    m_metalmap = metalmap;
    m_heightmap.free();
    heightmap.downscale(Settings::Instance()->value("MapViewing/downscaleHeightmap").toInt());
    m_heightmap = heightmap;
    if (getGLExtensionFunctions().openGL15Supported()) {
        glDeleteBuffers(1, &m_VBOVertices);
        glDeleteBuffers(1, &m_VBONormals);
        glDeleteBuffers(1, &m_VBOTexCoords);
        m_debugInfo = "VBO: <b>" + tr("Enabled");
    } else
        m_debugInfo = "VBO: <b>" + tr("Disabled");
    m_vertexNumber = heightmap.getWidth()*(heightmap.getHeight()-1)*2;
    m_debugInfo += "</b> FPS: <b>%1</b> " + tr("Number of primitives") + ": <b>" + QString::number(heightmap.getWidth()*heightmap.getHeight()*2)+"</b>";
    if (m_vertexes) delete m_vertexes;
    m_vertexes = new Vertex[m_vertexNumber];
    float range = m_heightmap.getMaxHeight() - m_heightmap.getMinHeight();
    for (int i = 0; i < heightmap.getHeight(); i++) {
        for (int j = 0; j < heightmap.getWidth(); j++) {
            unsigned short int value = heightmap.getData()[i*heightmap.getWidth()+j];
            int offset = i*heightmap.getWidth()+j;
            m_vertexes[offset].x = i * CELL_SIZE;
            m_vertexes[offset].y = j * CELL_SIZE;
            //m_vertexes[offset].z = value/(float)MAX_SHORT*MAX_HEIGHT*m_heightmap.getRatio();
            m_vertexes[offset].z = (m_heightmap.getMinHeight()+value*range/MAX_SHORT)*m_heightmap.getRatio()*SCALE_HEIGHTMAP;
        }
    }
    compileObject = true;
    lastZoom *= m_heightmap.getRatio();
    switch(Settings::Instance()->value("MapViewing/startPos/startRect/brushNumber").toInt()) {
    case 1:
        m_brushStyle = Qt::BDiagPattern;
        break;
    case 2:
        m_brushStyle = Qt::FDiagPattern;
        break;
    case 3:
        m_brushStyle = Qt::DiagCrossPattern;
        break;
    case 0:
    default:
        m_brushStyle = Qt::SolidPattern;
        break;
    }
    m_borderWidth = Settings::Instance()->value("MapViewing/startPos/startRect/borderWidth").toInt();
    m_alpha = Settings::Instance()->value("MapViewing/startPos/startRect/alpha").toInt();
    if (hasFocus()) updateGL();
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
    float newZoom = lastZoom - event->delta() * 0.0005*m_heightmap.getRatio();
    if (newZoom > 1*m_heightmap.getRatio() || newZoom < 0.1*m_heightmap.getRatio()) return;
    lastZoom = newZoom;
    resizeGL(width(), height());
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
        setYRotation(yRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        //setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    } else if (event->buttons() & Qt::MidButton) {
        this->dx += -dx * 0.1 * lastZoom;
        this->dy += dy * 0.1 * lastZoom;
//        resizeGL(width(), height());
        updateGL();
    }
    lastPos = event->pos();
}


void MapRendererWidget::drawStartRecs() {
    if (m_minimap.isNull()) return;
    m_withRects = m_minimap;
    QPainter p(&m_withRects);
    if(Settings::Instance()->value("MapViewing/metalmapSuperposition").toBool())
        p.drawImage(m_withRects.rect(), m_metalmap);
    if (m_drawStartPositions && !startRects.isEmpty()) {
        for (QMap<int, QRect>::const_iterator i = startRects.begin(); i != startRects.end(); i++) {
            QRect scaled = i.value();
            scaled.setWidth(scaled.width()/201.*m_withRects.width());
            scaled.setHeight(scaled.height()/201.*m_withRects.height());
            scaled.moveTo(scaled.x()/201.*m_withRects.width(), scaled.y()/201.*m_withRects.height());
            QColor red(Qt::red);
            QColor redFill(Qt::red);
            redFill.setAlpha(m_alpha);
            QColor green(Qt::green);
            QColor greenFill(Qt::green);
            greenFill.setAlpha(m_alpha);
            if (i.key() == myAlly) {
                p.setBrush(QBrush(greenFill, m_brushStyle));
                p.setPen(QPen(green, m_borderWidth));
            } else {
                p.setBrush(QBrush(redFill, m_brushStyle));
                p.setPen(QPen(red, m_borderWidth));
            }
            p.drawRect(scaled);
        }
    }
    p.end();
    deleteTexture(m_texture);
    m_texture = bindTexture(QPixmap::fromImage(m_withRects), GL_TEXTURE_2D);
    m_redrawStartRects = false;
}

void MapRendererWidget::addStartRect(int ally, QRect r) {
    startRects[ally] = r;
    m_redrawStartRects = true;
}

void MapRendererWidget::setMyAllyTeam(int n) {
    n--;
    if (myAlly == n) return;
    myAlly = n;
    m_redrawStartRects = true;
}

void MapRendererWidget::removeStartRect(int ally) {
    startRects.remove(ally);
    m_redrawStartRects = true;
}


// Vertex

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

