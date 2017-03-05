/**/


#include "MainWindow.h"
#include <QCoreApplication>
#include <QHBoxLayout>
#include <QMessageBox>
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
}

//-----------------------------------------------------------------------------
void Viewer::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
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