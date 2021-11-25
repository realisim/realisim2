/**/
// openglHeaders must be inluded firat
#include "Rendering/Gpu/OpenGlHeaders.h"

#include "3d/Loader/ObjLoader.h"
#include "Core/FileInfo.h"
#include "Core/Path.h"
#include "Geometry/Mesh.h"
#include "MainWindow.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QOpenGLContext>
#include "Rendering/Gpu/VertexArrayObjectMaker.h"
#include <sstream>

using namespace Realisim;
using namespace Core;
using namespace Math;
using namespace Rendering;
using namespace ThreeD;
using namespace std;

//----------------------------------------------
Viewer3d::Viewer3d(QWidget* ipParent, MainWindow* ipMw) :
	mpMainWindow(ipMw),
	Viewer(ipParent)
{
	setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    mTimer.start();
}

Viewer3d::~Viewer3d()
{
    for (auto pModelVao : mModelVaoPtrs) {
        delete pModelVao;
        pModelVao = nullptr;
    }
    mModelVaoPtrs.clear();
}

#include "Geometry/RectangularPrism.h"
//-----------------------------------------------------------------------------
void Viewer3d::initializeGL()
{
	Viewer::initializeGL();

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glEnable(GL_FRAMEBUFFER_SRGB);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    //--- init Object
    FileInfo appPath(Path::getApplicationFilePath());
    string assetsPath = Path::join(appPath.getAbsolutePath(), "../assets");

    //ObjLoader loader;
    //ObjLoader::Asset asset = loader.load(assetsPath + "/monkey_smooth.obj");
    ////ObjLoader::Asset asset = loader.load("D:/Models/standford models/monkey.obj");
    ////ObjLoader::Asset asset = loader.load("D:/Models/standford models/horse.obj");
    ////ObjLoader::Asset asset = loader.load("D:/Models/standford models/xyzrgb_dragon_smooth.obj");
    ////ObjLoader::Asset asset = loader.load("D:/Models/standford models/Sponza-master/sponza.obj");
    ////ObjLoader::Asset asset = loader.load("D:/Models/Nefertiti.obj");
    ////ObjLoader::Asset asset = loader.load("D:/Models/Dragon_1_rotated.obj");
    ////ObjLoader::Asset asset = loader.load("D:/Models/bressant_statue.obj");
    //
    //for (const auto assetMesh : asset.mMeshes)
    //{
    //    std::vector<Geometry::Mesh> cutMeshes;
    //    Geometry::Mesh::cutIntoSmallerMeshes(*assetMesh, 65535, &cutMeshes);

    //    for (const auto& m : cutMeshes) {
    //        mModelVaoPtrs.push_back(makeVao(m));
    //    }
    //}
    

    Geometry::RectangularPrism prism;
    prism.set(Vector3(-0.5), 1.0, 1.0, 1.0);
    mModelVaoPtrs.push_back(makeVao(prism.makeMesh()));
    

	mpMainWindow->updateUi();
}

//-----------------------------------------------------------------------------
void Viewer3d::loadShaders()
{
    Viewer::loadShaders();
}

//-----------------------------------------------------------------------------
void Viewer3d::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Matrix4 rotMatrix, scaleMatrix, modelMatrix;
    scaleMatrix.setAsScaling(Vector3(50));
    rotMatrix.setAsRotation( degreesToRadians( fmod(mTimer.elapsed()*6, 360.0) ), Vector3(0, 1, 0));
    modelMatrix = rotMatrix * scaleMatrix;
    //Matrix4 modelMatrix;

    glUseProgram(mShadedMaterialShader.getProgramId());

    mShadedMaterialShader.setUniform("uProjectionMatrix", mCamera.getProjectionMatrix());
    mShadedMaterialShader.setUniform("uViewMatrix", mCamera.getViewMatrix());
    mShadedMaterialShader.setUniform("uModelMatrix", modelMatrix);
    mShadedMaterialShader.setUniform("uApplyLighting", true);
    mShadedMaterialShader.setUniform("uLightPosition", Math::Vector3(-1, 1, 1));

    //draw axis
    for (auto vao : mModelVaoPtrs) {
        vao->draw();
    }

    glUseProgram(0);
}

//-----------------------------------------------------------------------------
//--- MainWindow
//-----------------------------------------------------------------------------
MainWindow::MainWindow() : QMainWindow(),
mpViewer(0),
mTimerId(0)
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