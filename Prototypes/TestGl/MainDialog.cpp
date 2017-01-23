/**/


#include "MainDialog.h"
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
    f->glClearColor(0.2, 0.2, 0.2, 0.0);
}

//-----------------------------------------------------------------------------
void Viewer::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//-----------------------------------------------------------------------------
//--- MainDialog
//-----------------------------------------------------------------------------
MainDialog::MainDialog() : QMainWindow(),
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
            QPushButton* pBut = new QPushButton("test", pCentralWidget);
            connect(pBut, SIGNAL(clicked()), this, SLOT(testClicked()));

            pControlLyt->addWidget(pBut);
		}		

		mpViewer = new Viewer(pCentralWidget);

		pLyt->addLayout(pControlLyt, 1);
		pLyt->addWidget(mpViewer, 4);
	}
    
	updateUi();
}

//-----------------------------------------------------------------------------
void MainDialog::testClicked()
{
    QMessageBox msgBox;
    msgBox.setText("Testing slot mechanism...");
    msgBox.exec();
}

//-----------------------------------------------------------------------------
void MainDialog::updateUi()
{
	update();
	mpViewer->update();
}