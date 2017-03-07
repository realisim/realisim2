/**/

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#endif

#include <cmath>
#include "MainWindow.h"
#include <QCoreApplication>
#include <QHBoxLayout>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QPushButton>

using namespace Realisim;
using namespace Math;
using namespace TreeD;

Viewer::Viewer(QWidget* ipParent /*=0*/) :
QOpenGLWidget(ipParent),
mCamera()
{
	setFocusPolicy(Qt::StrongFocus);
    //setMouseTracking(true);
    
    Projection p;
    Viewport v;
    v.set(400, 400);
    //p.setOrthoProjection(100, 1, 1000.0);
    p.setPerspectiveProjection(65, 16/9.0, 1, 1000.0);
    //p.setProjection(-50, 50, -50, 50, 1, 1000, Projection::tPerspective);
    mCamera.setViewport(v);
    mCamera.setProjection(p, true);
    mCamera.set(Vector3(0.0, 0.0, 100),
                Vector3(0.0, 0.0, 0.0),
                Vector3(0.0, 1.0, 0.0) );
}

Viewer::~Viewer()
{}

//-----------------------------------------------------------------------------
Camera Viewer::camera() const
{
    return mCamera;
}

//-----------------------------------------------------------------------------
void Viewer::drawCube()
{
    glDisable(GL_LIGHTING);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    const int s = 10.0 / 2;
    
    glBegin(GL_QUADS);
    
    //+z
    glColor3ub(255, 0, 0);
    glVertex3d(-s, -s, s);
    glVertex3d( s, -s, s);
    glVertex3d( s,  s, s);
    glVertex3d(-s,  s, s);
    
    //-z
    glColor3ub(255, 0, 0);
    glVertex3d(-s, -s, -s);
    glVertex3d(-s,  s, -s);
    glVertex3d( s,  s, -s);
    glVertex3d( s, -s, -s);
    
    //+x
    glColor3ub(0, 255, 0);
    glVertex3d( s, -s, -s);
    glVertex3d( s,  s, -s);
    glVertex3d( s,  s,  s);
    glVertex3d( s, -s,  s);
    
    //-x
    glColor3ub(0, 255, 0);
    glVertex3d(-s, -s, -s);
    glVertex3d(-s, -s,  s);
    glVertex3d(-s,  s,  s);
    glVertex3d(-s,  s,  -s);
    
    //+y
    glColor3ub(0, 0, 255);
    glVertex3d(-s, s, -s);
    glVertex3d(-s, s,  s);
    glVertex3d( s, s,  s);
    glVertex3d( s, s, -s);
    
    //-y
    glColor3ub(0, 0, 255);
    glVertex3d(-s, -s, -s);
    glVertex3d( s, -s, -s);
    glVertex3d( s, -s,  s);
    glVertex3d(-s, -s,  s);
    
    glEnd();
}

//-----------------------------------------------------------------------------
void Viewer::initializeGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
    
    f->glEnable(GL_FRAMEBUFFER_SRGB);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

//-----------------------------------------------------------------------------
void Viewer::mouseMoveEvent(QMouseEvent* ipE)
{
    Camera c = camera();
    const double kDegreeToRadian = M_PI/180.0;
    c.rotate( 2 * kDegreeToRadian, Vector3(1.0, 1.0, 1.0));
    setCamera(c);
}

//-----------------------------------------------------------------------------
void Viewer::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadMatrixd(mCamera.viewMatrix().getDataPointer());
    
    for(int i = -2; i <= 2; ++i)
        for(int j = -2; j <= 2; ++j)
        {
            glPushMatrix();
            
            glTranslated( i * 30, j * 30, 0.0 );
            drawCube();
            
            glPopMatrix();
        }
    
    
}

//-----------------------------------------------------------------------------
void Viewer::resizeGL(int iW, int iH)
{
    Viewport v;
    v.set(iW, iH);
    mCamera.setViewport(v);
    
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(mCamera.projectionMatrix().getDataPointer() );
    glMatrixMode(GL_MODELVIEW);
}

//-----------------------------------------------------------------------------
void Viewer::setCamera(Camera& iC)
{
    mCamera = iC;
    update();
}

//-----------------------------------------------------------------------------
//--- MainWindow
//-----------------------------------------------------------------------------
MainWindow::MainWindow() : QMainWindow(),
mpViewer(0)
{
	resize(800, 600);

	QWidget *pCentralWidget = new QWidget(this);
	setCentralWidget(pCentralWidget);

	QHBoxLayout* pLyt = new QHBoxLayout(pCentralWidget);
	pLyt->setMargin(5);
	{
		QVBoxLayout *pControlLyt = new QVBoxLayout();
		{
		}		

		mpViewer = new Viewer(pCentralWidget);

		pLyt->addLayout(pControlLyt, 1);
		pLyt->addWidget(mpViewer, 4);
	}
    
	updateUi();
}

//-----------------------------------------------------------------------------
void MainWindow::updateUi()
{
	update();
	mpViewer->update();
}