/**/

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#endif

#include "3d/Camera.h"
#include "MainWindow.h"
#include <QCoreApplication>
#include <QHBoxLayout>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QPushButton>


Viewer::Viewer(QWidget* ipParent /*=0*/) : QOpenGLWidget(ipParent)
{
	setFocusPolicy(Qt::StrongFocus);
}

Viewer::~Viewer()
{}

//-----------------------------------------------------------------------------
void Viewer::initializeGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
    
    f->glEnable(GL_FRAMEBUFFER_SRGB);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

//-----------------------------------------------------------------------------
void Viewer::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    Realisim::TreeD::Camera c;
    Realisim::TreeD::Projection p;
    Realisim::TreeD::Viewport v;
    v.set(400, 400);
    p.setOrthoProjection(100, 1, 1000.0);
    c.setViewport(v);
    c.setProjection(p, true);
    
    glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    glLoadMatrixd(c.getProjectionMatrix().getDataPointer() );
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(c.getViewMatrix().getDataPointer());

    
    glDisable(GL_LIGHTING);
    glTranslated(0, 0, 0);
    glColor3ub(255, 0, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);
        glVertex3d(-10, -10, 10);
        glVertex3d( 10, -10, 10);
        glVertex3d( 10,  10, 10);
        glVertex3d(-10,  10, 10);
    glEnd();
}

//-----------------------------------------------------------------------------
void Viewer::resizeGL(int iW, int iH)
{
    
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