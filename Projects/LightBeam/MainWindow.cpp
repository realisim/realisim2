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
#include "Geometry/Loader/ObjLoader.h"
#include "Geometry/PlatonicSolid.h"


using namespace Realisim;
    using namespace Core;
    using namespace LightBeam;
    using namespace Math;
using namespace std;

const int kInitialLod = 16;

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
    Rendering::Camera &cam  = b.getCamera();
    cam.set(Vector3(1.0, 2.0, 4.0),
        Vector3(0.0, 0.0, 0.0),
        Vector3(0.0, 1.0, 0.0));
    
    // add a plane
    Geometry::Plane p(Vector3(0.0, -30, 0.0), Vector3(0.0, 1.0, 0.0));
    shared_ptr<PlaneNode> pn = make_shared<PlaneNode>();
    pn->setPlane(p);
    shared_ptr<Material> mat0 = make_shared<Material>();
    mat0->setColor(Color(1.0, 0.0, 0.0, 1.0));
    pn->setMaterial(mat0);
    
    // add a sphere
    Geometry::Sphere sphere(Vector3(6.0, 0.0, 0.0), 4);
    shared_ptr<SphereNode> sn = make_shared<SphereNode>();
    sn->setSphere(sphere);
    shared_ptr<Material> mat1 = make_shared<Material>();
    mat1->setColor( Color(0.0, 1.0, 0.0, 1.0) );
    sn->setMaterial(mat1);
    
    // add another sphere
    sphere.setCenter(Vector3(-6, 0.0, 0));
    sphere.setRadius(4);
    shared_ptr<SphereNode> sn2 = make_shared<SphereNode>();
    sn2->setSphere(sphere);
    shared_ptr<Material> mat2 = make_shared<Material>();
    mat2->setColor( Color(0.0, 0.0, 1.0, 1.0) );
    mat2->setSpecularFactor(0.0);
    mat2->setGlossFactor(1.0);
    sn2->setMaterial(mat2);
    
    // add another sphere
    sphere.setCenter(Vector3(70, 0, -50));
    sphere.setRadius(50);
    shared_ptr<SphereNode> sn3 = make_shared<SphereNode>();
    sn3->setSphere(sphere);
    shared_ptr<Material> mat3 = make_shared<Material>();
    mat3->setColor( Color(1.0, 1.0, 1.0, 1.0) );
    mat3->setSpecularFactor(0.7);
    mat3->setGlossFactor(0.8);
    sn3->setMaterial(mat3);
    
    // add a light
    Light l;
    l.setType(Light::tDirectionnal);
    l.setDirection( Vector3(0.3, 1.0, 2.0) );
    shared_ptr<LightNode> lightNode = make_shared<LightNode>();
    lightNode->setLight(l);

    //Light l;
    //l.setType(Light::tPoint);
    //l.setPosition(Vector3(2.0, 2.0, 2.0));
    //l.setAttenuationType(Light::atQuadratic);
    //shared_ptr<LightNode> lightNode = make_shared<LightNode>();
    //lightNode->setLight(l);
    
    // add a mesh
    shared_ptr<MeshNode> meshNode = make_shared<MeshNode>();
    shared_ptr<Material> mat4 = make_shared<Material>();
    mat4->setColor(Color(0.8, 0.8, 0.8, 1.0));
    mat4->setSpecularFactor(0.0);
    mat4->setGlossFactor(0.0);
    meshNode->setMaterial(mat4);

    //Geometry::PlatonicSolid ps(Geometry::PlatonicSolid::tCube);
    //Geometry::Mesh *pMesh = new Geometry::Mesh(ps.getMesh());
    //meshNode->setMeshAndTakeOwnership(pMesh);

    Geometry::ObjLoader objLoader;
    //Geometry::Mesh *pMesh = objLoader.load("D:/Models/standford models/monkey.obj");
    //Geometry::Mesh *pMesh = objLoader.load("D:/Models/standford models/happyBuddha.obj");
    //Geometry::Mesh *pMesh = objLoader.load("D:/Models/untitled.obj");
    //Geometry::Mesh *pMesh = objLoader.load("D:/Models/standford models/horse.obj");
    //Geometry::Mesh *pMesh = objLoader.load("D:/Models/standford models/bunny.obj");
    //Geometry::Mesh *pMesh = objLoader.load("D:/Models/standford models/armadillo.obj");
    //Geometry::Mesh *pMesh = objLoader.load("D:/Models/standford models/xyzrgb_dragon.obj");
    Geometry::Mesh *pMesh = objLoader.load("D:/Models/standford models/sponza-master/sponza.obj");
        
    if (!objLoader.hasErrors())
    {
        meshNode->setMeshAndTakeOwnership(pMesh);
    }

    scene.addNode(pn);
    scene.addNode(sn);
    scene.addNode(sn2);
    //scene.addNode(sn3);
    scene.addNode(lightNode);
    scene.addNode(meshNode);
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
       mouse.getDelta().normSquared() > 0 )
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
    }
    
    if(renderNeeded)
    {
        mRayTracerRef.render(kInitialLod);
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
            lod /= 2;
            if (lod > 0)
            {
                mRayTracerRef.render(lod);
            }
        }
    }
}

//-----------------------------------------------------------------------------
void MainWindow::viewChanged()
{
    mRayTracerRef.render(kInitialLod);
}

//-----------------------------------------------------------------------------
void MainWindow::updateUi()
{
    mpView->updateUi();
    update();
}

