// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "MapRendererWidget.h"
#include <cmath>
#include <QDebug>
#include <QApplication>
#include "Settings.h"


int perm[256]= {151,160,137,91,90,15,
                131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
                190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
                88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
                77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
                102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
                135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
                5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
                223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
                129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
                251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
                49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
                138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180};

int grad3[16][3] = {{0,1,1},{0,1,-1},{0,-1,1},{0,-1,-1},
                    {1,0,1},{1,0,-1},{-1,0,1},{-1,0,-1},
                    {1,1,0},{1,-1,0},{-1,1,0},{-1,-1,0}, // 12 cube edges
                    {1,0,-1},{-1,0,-1},{0,-1,1},{0,1,1}}; // 4 more to make 16

MapRendererWidget::MapRendererWidget(QWidget* parent) : QGLWidget(parent) {
    xRot = 0;
    yRot = 0;
    zRot = 0;
    dx = 0;
    dy = 0;
    dz = 0;
    lastZoom = 1.0;
    m_compileObjects = false;
    blockRerender = false;
    getGLExtensionFunctions().resolve(context());
    m_glslSupported = getGLExtensionFunctions().glslSupported();
    m_glslSupported = m_glslSupported && Settings::Instance()->value("MapViewing/useShaders").toBool();
    m_redrawStartRects = true;
    setAutoBufferSwap(false);
    m_perspective = Settings::Instance()->value("MapViewing/perspectiveProjectionType").toBool();
    if(m_glslSupported) {
        makeCurrent();
        if(m_waterShaderSet.loadShaders(":/src/shaders/water.glsl")) {
            m_waterTimeLoc = m_waterShaderSet.getUniformLocation("time");
            m_waterpermTextureLoc = m_waterShaderSet.getUniformLocation("permTexture");
            m_lightSourceLoc = m_waterShaderSet.getUniformLocation("lightSource");
            m_reflectionTextureLoc = m_waterShaderSet.getUniformLocation("reflectionTexture");
            glActiveTexture(GL_TEXTURE1);
            initPermTexture();
        } else {
            qDebug() << "Water shader failed to load";
            qDebug() << m_waterShaderSet.getErrorMessage();
        }
        if(m_landShaderSet.loadShaders(":/src/shaders/land.glsl")) {
            m_landTextureLoc = m_landShaderSet.getUniformLocation("tex");
            m_landLightSourceLoc = m_landShaderSet.getUniformLocation("lightSource");
        } else {
            qDebug() << "Land shader failed to load";
            qDebug() << m_landShaderSet.getErrorMessage();
        }
    }
    fbo = 0;
}

MapRendererWidget::~MapRendererWidget() {
    m_heightmap.free();
    m_waterPlane.free();
}

void MapRendererWidget::initializeGL() {
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);

    //Land material
    m_landMaterial.setAmbient(1.0, 1.0, 1.0, 1.0);
    m_landMaterial.setDiffuse(1.0, 1.0, 1.0, 1.0);
    m_landMaterial.setSpecular(1.0, 0.8, 0.8, 1.0);
    m_landMaterial.setShininess(80.0);

    //Water material
    m_waterMaterial.setAmbient(0.0, 0.0, 0.2, 0.3);
    m_waterMaterial.setDiffuse(0.0, 0.0, 0.8, 0.3);
    m_waterMaterial.setSpecular(0.9, 0.9, 0.9, 0.9);
    m_waterMaterial.setShininess(128.0);

    compileSimpleWaterPlane();

    glShadeModel (GL_SMOOTH);

    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glBlendFunc(GL_ONE,GL_ONE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_ALPHA_TEST);
    //glDepthMask(GL_FALSE);

    //    glLightfv(GL_LIGHT0, GL_POSITION,LightPosition);
    glEnable(GL_LIGHT0);
    GLfloat ambientLight[]={0.1,0.1,0.1,1.0};    	             // set ambient light parameters
    glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);

    GLfloat diffuseLight[]={0.8,0.8,0.8,1.0};    	             // set diffuse light parameters
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);

    GLfloat specularLight[]={0.5,0.5,0.5,1.0};  	               // set specular light parameters
    glLightfv(GL_LIGHT0,GL_SPECULAR,specularLight);

    /*glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 15.0f);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);*/

    //glColorMaterial ( GL_FRONT_AND_BACK, GL_DIFFUSE ) ;

    //glEnable(GL_COLOR_MATERIAL);
    fbo = new QGLFramebufferObject(1024, 1024, QGLFramebufferObject::Depth, GL_TEXTURE_2D, GL_RGBA16);

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
}

void MapRendererWidget::hideEvent(QHideEvent* /*event*/){
    m_timer.stop();
}

void MapRendererWidget::showEvent(QShowEvent * /*event*/) {
    m_timer.start();
}

void MapRendererWidget::resizeGL(int w, int h) {
    glViewport(0, 0, (GLint)w, (GLint)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();

    if(m_perspective) {
        const float angle=30;
        gluPerspective(angle,double(w)/double(h),1,1000);
    } else {
        if (w <= h)
            glOrtho (dx+lastZoom*-100, dx+lastZoom*100,/*left,right*/
                     dy+lastZoom*-100*(GLfloat)h/(GLfloat)w, dy+lastZoom*100*(GLfloat)h/(GLfloat)w,/*top,bottom*/
                     -2000.0, 2000.0);/*near,far*/
        else
            glOrtho (dx+lastZoom*-100*(GLfloat)w/(GLfloat)h, dx+lastZoom*100*(GLfloat)w/(GLfloat)h,/*left,right*/
                     dy+lastZoom*-100, dy+lastZoom*100,/*top,bottom*/
                     -2000.0, 2000.0);/*near,far*/
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    m_timer.start(25);
    m_lightTime.start();
}

void MapRendererWidget::paintGL() {
    if (!m_heightmap.getWidth() || blockRerender) return;

    m_time.start();
    int curMSecs = m_lightTime.elapsed();

    //    glEnable(GL_LIGHT0);

    glLoadIdentity();

    if (m_compileObjects) {
        makeObject();
        m_compileObjects = false;
    }

    if(m_perspective) {
        glTranslatef(0,-(lastZoom*100),0);
    }

    glTranslatef(dx, dy, dz);
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
    glTranslatef(-m_heightmap.getWidth()/2, 0, -m_heightmap.getHeight()/2);

    if(!m_glslSupported)
        glEnable(GL_LIGHTING);

    GLfloat LightPosition[]= {10, 20, 10, 0 };
    //GLfloat LightDirection[]= {m_heightmap.getHeight()/2, 0, m_heightmap.getWidth()/2, 0 };
    //glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, LightDirection);


    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );

    if(m_redrawStartRects) drawStartRecs();

    //Rendering water reflection into a framebuffer
    if(m_glslSupported && fbo->bind()) {
        glClearColor(0.0, 0.0, 0.0, 0.0); //transparent pixels will be ingnored while constructing reflection in shader
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
        //Mirroring along xz plane
        glScalef(1.0, -1.0, 1.0);
        glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
        glFrontFace( GL_CW );
        if(m_glslSupported) {
            m_landShaderSet.use();
            glUniform1iARB(m_landTextureLoc, 0);
            glUniform1iARB(m_lightSourceLoc, 0);
        }
        //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        //glCallList(m_simpleWaterPlane);
        //glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);*/
        m_landMaterial.apply();
        m_lowResHeightmap.draw();
        glDepthFunc(GL_GREATER);
        glCallList(m_simpleWaterPlane);
        glDepthFunc(GL_LESS);
        if(m_glslSupported)
            m_landShaderSet.stop();
        glPopMatrix();
        glFrontFace( GL_CCW );

        fbo->release();
//        fbo->toImage().save("/home/lupus/fbo.png");
    }
    glClearColor (0.0, 0.0, 0.4, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

    //land rendering
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    if(m_glslSupported) {
        m_landShaderSet.use();
        glUniform1iARB(m_landTextureLoc, 0);
        glUniform1iARB(m_lightSourceLoc, 0);
    }
    m_landMaterial.apply();
    m_heightmap.draw();
    if(m_glslSupported)
        m_landShaderSet.stop();

    glDisableClientState( GL_NORMAL_ARRAY );

    if(!m_glslSupported)
        glDisable(GL_LIGHTING);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_permTexture);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, fbo->texture());

    //Water rendering
    glEnable(GL_BLEND);
    if(m_glslSupported) {
        m_waterShaderSet.use();
        glUniform1fARB(m_waterTimeLoc, curMSecs/100.0);
        glUniform1iARB(m_waterpermTextureLoc, 1);
        glUniform1iARB(m_reflectionTextureLoc, 2);
        glUniform1iARB(m_lightSourceLoc, 0);
    }
    m_waterMaterial.apply();

    glColor4f(0.0, 0.0, 1.0, 0.3);
    glNormal3f(0, 1.0, 0);
    m_waterPlane.draw();
    if(m_glslSupported)
        m_waterShaderSet.stop();
    glDisable(GL_BLEND);

    glDisableClientState( GL_NORMAL_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );

    //glCallList(m_simpleWaterPlane);

    swapBuffers();
    int msecs = m_time.elapsed();
    if(msecs)
        emit updateDebugInfo(m_debugInfo.arg(1000/msecs));
    else
        emit updateDebugInfo(m_debugInfo.arg(tr("Inf")));
}

void MapRendererWidget::makeObject() {
    blockRerender = true;

    m_heightmap.compile();
    m_heightmap.freeUnusedData();

    m_lowResHeightmap.compile();
    m_lowResHeightmap.freeUnusedData();

    m_waterPlane.compile();
    m_waterPlane.freeUnusedData();

    blockRerender = false;
}


void MapRendererWidget::setSource(QString mapName, QImage minimap, QImage metalmap, RawHeightMap heightmap) {
    if (currentMap == mapName) return;
    int primNumber = 0;
    currentMap = mapName;
    m_minimap = minimap;
    m_metalmap = metalmap;
    heightmap.downscale(Settings::Instance()->value("MapViewing/downscaleHeightmap").toInt());
    m_ratio = heightmap.getRatio();
    primNumber += heightmap.getWidth()*heightmap.getHeight()*2;
    m_heightmap.build(heightmap);
    heightmap.downscale(2);
    m_lowResHeightmap.build(heightmap);
    heightmap.downscale(1);
    primNumber += heightmap.getWidth()*heightmap.getHeight()*2;
    m_waterPlane.build(heightmap, 0);
    heightmap.free();
    if (getGLExtensionFunctions().openGL15Supported())
        m_debugInfo = "VBO: <b>" + tr("Enabled");
    else
        m_debugInfo = "VBO: <b>" + tr("Disabled");
    if (getGLExtensionFunctions().fboSupported())
        m_debugInfo += "</b> FBO: <b>" + tr("Enabled");
    else
        m_debugInfo += "</b> FBO: <b>" + tr("Disabled");
    if (getGLExtensionFunctions().glslSupported())
        m_debugInfo += "</b> GLSL: <b>" + tr("Enabled");
    else
        m_debugInfo += "</b> GLSL: <b>" + tr("Disabled");
    m_debugInfo += "</b> FPS: <b>%1</b> " + tr("Number of primitives") + ": <b>" + QString::number(primNumber)+"</b>";
    m_compileObjects = true;
    lastZoom *= m_ratio;
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


void MapRendererWidget::setRotation(int xAngle, int yAngle, int zAngle) {
    int update=0;
    normalizeAngle(&xAngle);
    normalizeAngle(&yAngle);
    normalizeAngle(&zAngle);
    if (xAngle != xRot) {
        xRot = xAngle;
        update=1;
    }
    if (yAngle != yRot) {
        yRot = yAngle;
        update=1;
    }
    if (zAngle != zRot) {
        zRot = zAngle;
        update=1;
    }
    if (update==1) updateGL();
}

void MapRendererWidget::normalizeAngle(int *angle) {
    while (*angle < 0)
        *angle += 360 * 16;
    while (*angle > 360 * 16)
        *angle -= 360 * 16;
}

void MapRendererWidget::wheelEvent ( QWheelEvent * event ) {
    float newZoom = lastZoom - event->delta() * 0.001*m_ratio;
    if (newZoom > 4*m_ratio || newZoom < 0.05*m_ratio) return;
    lastZoom = newZoom;
    if(!m_perspective) {
        resizeGL(width(), height());
    }
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
        setRotation(xRot+ 8 * dy, yRot + 8 * dx, zRot);
    } else if (event->buttons() & Qt::MidButton || event->buttons() & Qt::RightButton) {
        this->dx += dx * 0.1 * lastZoom;
        this->dy += -dy * 0.1 * lastZoom;
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
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
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


void MapRendererWidget::initPermTexture() {
    char *pixels;
    int i,j;

    glGenTextures(1, &m_permTexture); // Generate a unique texture ID
    glBindTexture(GL_TEXTURE_2D, m_permTexture); // Bind the texture to texture unit N

    pixels = (char*)malloc( 256*256*4 );
    for(i = 0; i<256; i++)
        for(j = 0; j<256; j++) {
        int offset = (i*256+j)*4;
        char value = perm[(j+perm[i]) & 0xFF];
        pixels[offset] = grad3[value & 0x0F][0] * 64 + 64;   // Gradient x
        pixels[offset+1] = grad3[value & 0x0F][1] * 64 + 64; // Gradient y
        pixels[offset+2] = grad3[value & 0x0F][2] * 64 + 64; // Gradient z
        pixels[offset+3] = value;                     // Permuted index
    }

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
}

void MapRendererWidget::compileSimpleWaterPlane() {
    m_simpleWaterPlane = glGenLists(1);
    glNewList(m_simpleWaterPlane, GL_COMPILE);
    glBegin(GL_TRIANGLE_STRIP);
    const float overkill = 1.5;
    glNormal3f(0.0, 1.0, 0.0);
    glTexCoord2f(0, 0); glVertex3f(-overkill*m_heightmap.getWidth(), 0, -overkill*m_heightmap.getHeight());
    glTexCoord2f(1, 0); glVertex3f(overkill*m_heightmap.getWidth(), 0, -overkill*m_heightmap.getHeight());
    glTexCoord2f(0, 1); glVertex3f(-overkill*m_heightmap.getWidth(), 0, overkill*m_heightmap.getHeight());
    glTexCoord2f(1, 1); glVertex3f(overkill*m_heightmap.getWidth(), 0, overkill*m_heightmap.getHeight());
    glEnd();
    glEndList();
}
