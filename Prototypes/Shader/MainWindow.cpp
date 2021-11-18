/**/
// openglHeaders must be inluded firat
#include "Rendering/Gpu/OpenGlHeaders.h"

#include "MainWindow.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QOpenGLContext>
#include <sstream>

using namespace Realisim;
using namespace Math;
using namespace Rendering;
using namespace std;

//----------------------------------------------
Viewer3d::Viewer3d(QWidget* ipParent, MainWindow* ipMw) :
	mpMainWindow(ipMw),
	Viewer(ipParent)
{
	setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);    
}

Viewer3d::~Viewer3d()
{}

//-----------------------------------------------------------------------------
void Viewer3d::initializeGL()
{
	Viewer::initializeGL();

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glEnable(GL_FRAMEBUFFER_SRGB);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

	mpMainWindow->updateUi();
}

//-----------------------------------------------------------------------------
void Viewer3d::paintGL()
{
	Viewer::paintGL();
}

//-----------------------------------------------------------------------------
//--- MainWindow
//-----------------------------------------------------------------------------
MainWindow::MainWindow() : QMainWindow(),
mpViewer(0),
mTimerId(0),
mShaderSourceInitialized(false)
{
	resize(800, 600);

	QWidget *pCentralWidget = new QWidget(this);
	setCentralWidget(pCentralWidget);

	QVBoxLayout* pLyt = new QVBoxLayout(pCentralWidget);
	pLyt->setMargin(5);
	{
		mpViewer = new Viewer3d(pCentralWidget, this);

		pLyt->addWidget(mpViewer);
	}
   

    mTimerId = startTimer(0);
    mTimerToUpdateGl.start();
	updateUi();
}

//-----------------------------------------------------------------------------
void MainWindow::timerEvent(QTimerEvent *ipE)
{
    if(ipE->timerId() == mTimerId)
    {
        mpViewer->handleUserInput();
        
        const double e = mTimerToUpdateGl.elapsed();
        mUpdateStats.add(e);
        if(e + mUpdateStats.average() >= 1/60.0 )
        {
            mpViewer->update();
            mTimerToUpdateGl.start();
            mUpdateStats.clear();
        }
    }
}

//-----------------------------------------------------------------------------
void MainWindow::updateUi()
{
}