/**/
#include "Broker.h"
#include "Math/CommonMath.h"
#include "MainWindow.h"
#include <QCoreApplication>
#include <QGroupBox>
#include <QHBoxLayout>
#include "View.h"


// temporary until we can read scene from files...
#include "DataStructure/Scene/GeometryNodes.h"
#include "DataStructure/Scene/LightNode.h"


using namespace Realisim;
    using namespace Core;
    using namespace LightBeam;
    using namespace Math;
using namespace std;

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
    pLyt->setMargin(5);
    {
        mpView = new View(pCentralWidget, ipBroker);
        connect(mpView, SIGNAL(viewChanged()), this, SLOT(viewChanged()));

        pLyt->addWidget(mpView);
    }
    
    mpView->initialize();
    
    //------------------------------------
    // add some crap to the scene...
    Broker &b = getBroker();
    Scene &scene = b.getScene();
    
    // add a plane
    Geometry::Plane p(Vector3(0.0, -30, 0.0), Vector3(0.0, 1.0, 0.0));
    shared_ptr<PlaneNode> pn = make_shared<PlaneNode>();
    pn->setPlane(p);
    shared_ptr<Material> mat0 = make_shared<Material>();
    mat0->setColor(Color(1.0, 0.0, 0.0, 1.0));
    pn->setMaterial(mat0);
    
    // add a sphere
    Geometry::Sphere sphere(Vector3(6.0, 0.0, 0.0), 16);
    shared_ptr<SphereNode> sn = make_shared<SphereNode>();
    sn->setSphere(sphere);
    shared_ptr<Material> mat1 = make_shared<Material>();
    mat1->setColor( Color(0.0, 1.0, 0.0, 1.0) );
    sn->setMaterial(mat1);
    
    // add another sphere
    sphere.setCenter(Vector3(6, 0.0, -100));
    sphere.setRadius(35);
    shared_ptr<SphereNode> sn2 = make_shared<SphereNode>();
    sn2->setSphere(sphere);
    shared_ptr<Material> mat2 = make_shared<Material>();
    mat2->setColor( Color(0.0, 0.0, 1.0, 1.0) );
    mat2->setSpecularFactor(0.0);
    mat2->setGlossFactor(1.0);
    sn2->setMaterial(mat2);
    
    // add another sphere
    sphere.setCenter(Vector3(100, 0, -50));
    sphere.setRadius(50);
    shared_ptr<SphereNode> sn3 = make_shared<SphereNode>();
    sn3->setSphere(sphere);
    shared_ptr<Material> mat3 = make_shared<Material>();
    mat3->setColor( Color(1.0, 1.0, 1.0, 1.0) );
    mat3->setSpecularFactor(0.7);
    mat3->setGlossFactor(1.0);
    sn3->setMaterial(mat3);
    
    // add a light
    Light l;
    l.setType(Light::tDirectionnal);
    shared_ptr<LightNode> lightNode = make_shared<LightNode>();
    lightNode->setLight(l);
    
    scene.addNode(pn);
    scene.addNode(sn);
    scene.addNode(sn2);
    scene.addNode(sn3);
    scene.addNode(lightNode);
    //------------------------------------
    
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
    Rendering::Camera &c = b.getCamera();
    
    //--- Mouse
    Mouse &mouse = b.getMouse();
    if(mouse.isButtonPressed(Mouse::bLeft) &&
       mouse.getDelta().normSquare() > 0 )
    {
        const double f = degreesToRadians(1.0);
        const Vector2i d = mouse.getAndClearDelta();
        
        c.rotate(f * -d.x(),
                 Vector3(0.0, 1.0, 0.0),
                 c.getPosition());
        
        c.rotate(f * -d.y(),
                 c.getLateralVector(),
                 c.getPosition() );
        
        renderNeeded = true;
    }
    
    //--- keyboard
    {
        Keyboard &k = b.getKeyboard();
        Vector3 displacement;
        const double f = 5.0;
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
    }
    
    if(renderNeeded)
    {
        mRayTracerRef.render();
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
            mpView->updateUi();
        }
    }
}

//-----------------------------------------------------------------------------
void MainWindow::viewChanged()
{
    mRayTracerRef.render();
}

//-----------------------------------------------------------------------------
void MainWindow::updateUi()
{
    mpView->updateUi();
    update();
}

