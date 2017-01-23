/**/


#include "MainDialog.h"
#include <QCoreApplication>
#include <QHBoxLayout>


Viewer::Viewer(QWidget* ipParent /*=0*/) : QGLWidget(ipParent)
{
	setFocusPolicy(Qt::StrongFocus);
}

Viewer::~Viewer()
{}

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
		}		

		mpViewer = new Viewer(pCentralWidget);

		pLyt->addLayout(pControlLyt, 1);
		pLyt->addWidget(mpViewer, 4);
	}
    
	updateUi();
}

//-----------------------------------------------------------------------------
void MainDialog::updateUi()
{
	update();
	mpViewer->update();
}