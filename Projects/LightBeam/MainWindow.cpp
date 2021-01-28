/**/
#include "Broker.h"
#include "Math/CommonMath.h"
#include "MainWindow.h"
#include <QCoreApplication>
#include <QGroupBox>
#include <QHBoxLayout>
#include "View.h"


using namespace Realisim;
    using namespace Core;
    using namespace LightBeam;
    using namespace Math;
    using namespace ThreeD;
using namespace std;

const int kInitialLod = 8;

//-----------------------------------------------------------------------------
//--- MainWindow
//-----------------------------------------------------------------------------
MainWindow::MainWindow(Broker *ipBroker, RayTracer *ipRayTracer) : QMainWindow(),
    mBrokerRef(*ipBroker),
    mRayTracerRef(*ipRayTracer)
{    
    resize(400, 300);

    QWidget *pCentralWidget = new QWidget(this);
    setCentralWidget(pCentralWidget);

    QHBoxLayout* pLyt = new QHBoxLayout(pCentralWidget);
    pLyt->setMargin(0);
    {
        mpView = new View(pCentralWidget, ipBroker);
        connect(mpView, SIGNAL(viewChanged()), this, SLOT(viewChanged()));

        pLyt->addWidget(mpView);
    }
    
    mpView->initialize();
    
    //getBroker().getSceneRef().createDummyScene();
    getBroker().getSceneRef().importObj("D:/Models/SponzaWithLightPoints.obj");
    //getBroker().getSceneRef().importObj("D:/Models/standford models/monkey.obj");
    //getBroker().getSceneRef().importObj("D:/Models/texturedCube/cube.obj");
    //getBroker().getSceneRef().importObj("D:/Models/turboSquid/turbosonicobj/turbosonic.obj");
    
    mUpdateTimerId = startTimer(30);
}

//-----------------------------------------------------------------------------
Broker& MainWindow::getBroker()
{ return mBrokerRef; }

//-----------------------------------------------------------------------------
void MainWindow::handleUserInput()
{
    using namespace Interface;
    bool renderNeeded = false;
    
    Broker &b = getBroker();
    Rendering::Camera &c = b.getSceneRef().getCameraRef();
    
    //--- Mouse
    Mouse &mouse = b.getMouse();
    if(mouse.getState() == Mouse::sDragging)
    {
        const double f = degreesToRadians(1.0);
        const Vector2i d = mouse.getAndClearDelta();
        
        
        c.rotate(f * -d.x(),
                 Vector3(0.0, 1.0, 0.0),
                 c.getPosition());
        
        c.rotate(f * -d.y(),
                 c.getLateralVector(),
                 c.getPosition() );
        
        if (d.normSquared() > 0)
        {
            renderNeeded = true;
        }
    }
    
    //--- keyboard
    {
        Keyboard &k = b.getKeyboard();
        Vector3 displacement;
        double f = 1.0;

        if(k.isKeyPressed(Key_Shift))
        { f *= 0.1; }

        if (k.isKeyPressed(Key_Control))
        { f *= 10.0; }

        if(k.isKeyPressed(Key_W))
        { displacement += c.getDirection() * f; }
        
        if(k.isKeyPressed(Key_A))
        { displacement -= c.getLateralVector() * f; }
        
        if(k.isKeyPressed(Key_S))
        { displacement -= c.getDirection() * f; }
        
        if(k.isKeyPressed(Key_D))
        { displacement += c.getLateralVector() * f; }
        
        if(k.isKeyPressed(Key_Q))
        { displacement += c.getUp() * f;}
        
        if(k.isKeyPressed(Key_E))
        { displacement -= c.getUp() * f; }
        
        if(!displacement.isEqual(Vector3(), 1e-5))
        {
            c.set(c.getPosition() + displacement,
                  c.getFocal() + displacement,
                  c.getUp() );
            
            renderNeeded = true;
        }


        //--- crappy debugging keys
        if (k.isKeyPressed(Key_R))
        {
            // Qt is Y down, while we are Y up...
            // make the conversion here
            const int viewPortHeight = c.getViewport().getHeight();
            Vector2i p(mouse.getPosition());
            p.setY(viewPortHeight - p.y());

            
            mRayTracerRef.debugRayCast(p);
        }
    }
    
    if(renderNeeded)
    {
        mRayTracerRef.render(kInitialLod, true);
    }
           
}

//-----------------------------------------------------------------------------
void MainWindow::timerEvent(QTimerEvent *ipE)
{
    if(ipE->timerId() == mUpdateTimerId)
    {
        handleUserInput();
    
        if(mRayTracerRef.hasNewFrameAvailable())
        {
            Image &fi = getBroker().getFinalImage();
            const Image &rendererImage = mRayTracerRef.getImage();
            fi.setData(rendererImage.getWidth(), rendererImage.getHeight(),
                rendererImage.getInternalFormat(),
                rendererImage.getImageData().constData());
            mpView->updateUi();
        }
        else
        {
            
            int lod = mRayTracerRef.getLevelOfDetail();
            lod *= 0.5;
            if (lod > 0)
            {
                mRayTracerRef.render(lod, false);
            }
        }
    }
}

//-----------------------------------------------------------------------------
void MainWindow::viewChanged()
{
    mRayTracerRef.render(kInitialLod, true);
}

//-----------------------------------------------------------------------------
void MainWindow::updateUi()
{
    mpView->updateUi();
    update();
}

