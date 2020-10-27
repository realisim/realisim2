/**/
#include "Broker.h"
#include "Math/CommonMath.h"
#include "MainWindow.h"
#include <QCoreApplication>
#include <QGroupBox>
#include <QHBoxLayout>
#include "View.h"
#include <regex>


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
    
    //------------------------------------
    // add some crap to the scene...
    Broker &b = getBroker();
    Scene &scene = b.getScene();
    Rendering::Camera &cam  = b.getCamera();
    cam.set(Vector3(10.0, 20.0, 30.0),
        Vector3(0.0, 0.0, 0.0),
        Vector3(0.0, 1.0, 0.0));
    
    // add a plane
    Geometry::Plane p(Vector3(0.0, -5, 0.0), Vector3(0.0, 1.0, 0.0));
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
    
    //Geometry::PlatonicSolid ps(Geometry::PlatonicSolid::tCube);
    //Geometry::Mesh *pMesh = new Geometry::Mesh(ps.getMesh());
    //meshNode->setMeshAndTakeOwnership(pMesh);

    Geometry::ObjLoader objLoader;
    Geometry::ObjLoader::Asset objAsset;
    //objAsset = objLoader.load("D:/Models/standford models/monkey.obj");
    //pMeshes = objLoader.load("D:/Models/standford models/monkey_flat.obj");
    //objAsset = objLoader.load("D:/Models/standford models/happyBuddha_smooth.obj");
    //objAsset = objLoader.load("D:/Models/Nefertiti_rotated.obj"); // very heavy...
    //objAsset = objLoader.load("D:/Models/Nefertiti.obj");

    //pMeshes = objLoader.load("D:/Models/someBoxes.obj");
    //pMeshes = objLoader.load("D:/Models/flowerPot.obj");
    //pMeshes = objLoader.load("D:/Models/debugObject2.obj");
    //Geometry::Mesh *pMesh = objLoader.load("D:/Models/standford models/horse.obj");
    //Geometry::Mesh *pMesh = objLoader.load("D:/Models/standford models/bunny.obj");
    //pMeshes = objLoader.load("D:/Models/standford models/armadillo.obj");
    //objAsset = objLoader.load("D:/Models/standford models/xyzrgb_dragon_smooth.obj");
    //objAsset = objLoader.load("D:/Models/standford models/sponza-master/sponza.obj");
    objAsset = objLoader.load("D:/Models/wallScene.obj");


    if (!objLoader.hasErrors())
    {
        // add all meshes
        // add a mesh
        for (int i = 0; i < objAsset.mMeshes.size(); ++i)
        {
            string name = objAsset.mName[i];
            regex matchRe("lightPoint.*");
            if (regex_match(name, matchRe))
            {
                Geometry::Mesh *pMeshToDiscard = objAsset.mMeshes[i];

                Light lightFromObj;
                lightFromObj.setType(Light::tPoint);
                lightFromObj.setPosition(pMeshToDiscard->getVertex(0).mVertex);
                lightFromObj.setAttenuationType(Light::atQuadratic);
                shared_ptr<LightNode> lightNodeFromObj = make_shared<LightNode>();
                lightNodeFromObj->setLight(lightFromObj);
                scene.addNode(lightNodeFromObj);

                delete pMeshToDiscard;

                printf("lightPoint %s added to scene.\n", name.c_str());
            }
            else
            {
                shared_ptr<MeshNode> meshNode = make_shared<MeshNode>();
                shared_ptr<Material> mat4 = make_shared<Material>();
                mat4->setColor(Color(0.85, 0.85, 0.85, 1.0));
                mat4->setSpecularFactor(0.0);
                mat4->setGlossFactor(0.0);
                meshNode->setMaterial(mat4);
                meshNode->setName(name);
                meshNode->setMeshAndTakeOwnership(objAsset.mMeshes[i]);
                scene.addNode(meshNode);

                printf("obj %s added to scene.\n", name.c_str());
            }
            
        }
        
    }

    //scene.addNode(pn);
    //scene.addNode(sn);
    //scene.addNode(sn2);
    //scene.addNode(sn3);
    scene.addNode(lightNode);
    //scene.addNode(meshNode);
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
            const int viewPortHeight = b.getCamera().getViewport().getHeight();
            Vector2i p(mouse.getPosition());
            p.setY(viewPortHeight - p.y());

            
            mRayTracerRef.debugRayCast(p);
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
            lod *= 0.5;
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

