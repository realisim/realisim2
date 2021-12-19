/**/

#include <cassert>
#include "MainWindow.h"
#include <QCoreApplication>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QOpenGLContext>
#include "Reactor/Engine.h"
#include <sstream>

//-- temp for scene creation
#include "Reactor/Systems/Renderer/Renderer.h"
#include "DataStructures/Scene/Scene.h"

using namespace Realisim;
using namespace std;

namespace
{
    const double kHighFrequencyRefreshRateInSeconds = 1/10.; //Hz
}

//-----------------------------------------------------------------------------
//--- MainWindow
//-----------------------------------------------------------------------------
MainWindow::MainWindow(Reactor::Engine *ipEngine) : QMainWindow(),
mpGlWindow(nullptr),
mEngine(*ipEngine),
mTimerId(0)
{
    resize(800, 600);

    mpCentralWidget = new QWidget(this);
    setCentralWidget(mpCentralWidget);

    QHBoxLayout* pLyt = new QHBoxLayout(mpCentralWidget);
    pLyt->setMargin(0);
    pLyt->setSpacing(5);
    {
        //--- gl context
        mpGlWindow = new GlWindow(0, 0, &mEngine);
        connect(mpGlWindow, SIGNAL(glContextHasBeenInitialized()),
            this, SLOT(handleGlContextIsInitialized()));
        connect(mpGlWindow, SIGNAL(fullScreenToggled()),
            this, SLOT(handleFullScreenToggled()));
        
        QWidget *pGlWidget = QWidget::createWindowContainer(mpGlWindow, mpCentralWidget);
        pGlWidget->setMinimumSize(10, 10);
        pGlWidget->setMaximumSize(7680, 4320); //8K
        pGlWidget->setFocusPolicy(Qt::StrongFocus);

        //--- streaming gl context is created only once mpGlWindow has been initialized.
        // this is to ensure they can share openGl context
        // see handleGlContextIsInitialized() for the creation of the streaming gl window;
        
        pLyt->addWidget(pGlWidget);
    }

    // create menus
    createMenus();


    Reactor::Broker& b = mEngine.getBrokerRef();
    b.makeBasicScene();
    
    Reactor::Hub& h = mEngine.getHubRef();
    Reactor::Renderer& renderer = h.getRendererRef();
    renderer.setScene(&b.getSceneRef());
}

//-----------------------------------------------------------------------------
void MainWindow::aboutClicked()
{
    //int major, minor, rev, build;
    //getVersion(&major, &minor, &rev, &build);

    //ostringstream oss;
    //oss << "<h1>Reactor Engine</h1><br>" <<
    //    "Copyright - 2019 Realisim<br>" <<
    //    "version: " << versionToString();
    //
    //QMessageBox::about(this, "Reactor Engine", QString::fromStdString(oss.str()));
}

//-----------------------------------------------------------------------------
void MainWindow::createMenus()
{
    QMenuBar* pMenuBar = new QMenuBar(this);
    setMenuBar(pMenuBar);

    QMenu *pFile = pMenuBar->addMenu("&File");
    pFile->addAction(QString("&Quit"), QCoreApplication::instance(), SLOT(quit()),
        QKeySequence::Quit);

     //-- help
    QMenu *pHelp = pMenuBar->addMenu("Help");
    pHelp->addAction(QString("&About"), this, SLOT(aboutClicked()));

}

//-----------------------------------------------------------------------------
Reactor::Engine& MainWindow::getEngine()
{
    return mEngine;
}

//-----------------------------------------------------------------------------
void MainWindow::handleFullScreenToggled()
{
    if (isFullScreen())
    {
        showNormal();
        menuBar()->show();
    }
    else
    {
        showFullScreen();
        menuBar()->hide();
    }
}

//-----------------------------------------------------------------------------
void MainWindow::handleGlContextIsInitialized()
{
    getEngine().setNativeWindowsGlContext(mpGlWindow->getOpenGlNativeDC(), mpGlWindow->getOpenGlNativeHandle());

    //// Create the streaming context only when the main context is created since they
    //// are going to be shared.

    ////--- streaming gl context
    //mpStreamingGlWindow = new GlWindow(mpGlWindow, mpGlWindow->getQOpenGlContext(), nullptr);
    //connect(mpStreamingGlWindow, SIGNAL(glContextHasBeenInitialized()),
    //    this, SLOT(handleGlStreamingContextIsInitialized()));
    //mpStremaingGlWidget = QWidget::createWindowContainer(mpStreamingGlWindow, mpCentralWidget);
    //mpStremaingGlWidget->setMinimumSize(1, 1);
    //mpStremaingGlWidget->setMaximumSize(2, 2);
    //mpStremaingGlWidget->setFocusPolicy(Qt::NoFocus);
    //mpStremaingGlWidget->show();

    // start the timer only when GL context are initiliazed, because the 
    // timerEvent will trigger some gl commands
    // Run as fast as possible. The renderer will sync up with the screen refresh rate
    //
    mTimerId = startTimer(0);
}

////-----------------------------------------------------------------------------
//void MainWindow::handleGlStreamingContextIsInitialized()
//{
//    mpStremaingGlWidget->hide();
//
//    getCore().setNativeWindowsGlStreamingContext(mpStreamingGlWindow->getOpenGlNativeDC(),
//        mpStreamingGlWindow->getOpenGlNativeHandle());
//}

//-----------------------------------------------------------------------------
void MainWindow::keyReleaseEvent(QKeyEvent *ipE)
{
    switch (ipE->key())
    {
    case Qt::Key_F11: handleFullScreenToggled(); break;
    default: 
        //pass to parent by not accepting the event
        ipE->ignore();
        break;
    }
}

//-----------------------------------------------------------------------------
void MainWindow::timerEvent(QTimerEvent *ipE)
{
    if (ipE->timerId() == mTimerId)
    {      
        getEngine().update();

        /*if(mTimerHighFrequencyRefreshTimer.elapsed() >= kHighFrequencyRefreshRateInSeconds)
        {
            updateUiAtHighFrequency();
            mTimerHighFrequencyRefreshTimer.start();
        }*/
        
    }
}

//-----------------------------------------------------------------------------
void MainWindow::updateUi()
{
    /*Engine::ScopedStatistics scopedStats("MainWindow::updateUi", getCore().getBroker());

    for(auto it : mPalettes)
    { 
        IG::Palette *p = it.second;
        if (p->getDockWidget()->isVisible())
        {
            p->updateUi();
        }
    }*/
}

//-----------------------------------------------------------------------------
void MainWindow::updateUiAtHighFrequency()
{
    /*Engine::ScopedStatistics scopedStats("MainWindow::updateUiAtHighFrequency", getCore().getBroker());

    for(auto it : mPalettes)
    { 
        IG::Palette *p = it.second;
        if (p->getDockWidget()->isVisible())
        {
            p->updateUiAtHighFrequency();
        }
    }*/
}

