
#include <cmath>
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Rendering/Gpu/OpenGlHeaders.h"
#include <QOpenGLContext>
#include "Core/FileInfo.h"
#include "Core/Path.h"
#include "Viewer.h"

using namespace Realisim;
    using namespace Math;
    using namespace Rendering;
    using namespace Core;
using namespace std;

//----------------------------------------------
Viewer::Viewer(QWidget* ipParent /*=0*/) :
QOpenGLWidget(ipParent),
mCamera(),
mWorldUpVector(0.0, 1.0, 0.0),
mCameraMovementIncrement(1.0),
mCameraMode(cmFree),
mMousePos(-1, -1),
mMousePosWhenPressed(-1, -1),
mMouseLeftPressed(false),
mMouseRightPressed(false)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
}

Viewer::~Viewer()
{}

//-----------------------------------------------------------------------------
Camera Viewer::getCamera() const
{
    return mCamera;
}

//-----------------------------------------------------------------------------
double Viewer::getCameraMovementIncrement() const
{
	return mCameraMovementIncrement;
}

//-----------------------------------------------------------------------------
Vector3 Viewer::getWorldUpVector() const
{
	return mWorldUpVector;
}

//-----------------------------------------------------------------------------
void Viewer::handleUserInput()
{
    Camera c = getCamera();
    
    //--- mouse
    const double kDegreeToRadian = M_PI/180.0;
    const double f = 0.15;
    if(mMouseLeftPressed)
    {
        // move camera
        switch (mCameraMode)
        {
            case cmRotateAround:
            {
                Vector3 rotateAboutPos(0.0);
                Vector3 rotateAboutAxisX = c.cameraDeltaToWorld(Vector3(1.0, 0.0, 0.0));
                
                c.rotate( -mMouseDelta.y() * kDegreeToRadian * f, rotateAboutAxisX,
                         rotateAboutPos);
                c.rotate( -mMouseDelta.x() * kDegreeToRadian * f, getWorldUpVector(),
                         rotateAboutPos);
            }break;
                
            case cmFree:
            {
                Vector3 rotateAboutPos = c.getPosition();
                Vector3 rotateAboutAxisX = c.cameraDeltaToWorld(Vector3(1.0, 0.0, 0.0));
                
                c.rotate( -mMouseDelta.y() * kDegreeToRadian * f, rotateAboutAxisX,
                         rotateAboutPos);
                c.rotate( -mMouseDelta.x() * kDegreeToRadian * f , getWorldUpVector(),
                         rotateAboutPos);
            }break;
                
            default: break;
        }
    }
    
    mMouseDelta.set(0, 0);
    
    //--- keyboard
    if (mKeyboard[Qt::Key_Plus])
    {
        mCameraMovementIncrement *= 1.1;
    }
    if (mKeyboard[Qt::Key_Minus])
    {
        mCameraMovementIncrement *= (1.0/1.1);
    }

    double inc = getCameraMovementIncrement();
    
    float dx = 0.0;
    float dy = 0.0;
    float dz = 0.0;
    
    if( mKeyboard[Qt::Key_A] ) { dx = -inc; }
    if( mKeyboard[Qt::Key_D] ) { dx = inc; }
    if( mKeyboard[Qt::Key_S] ) { dy = -inc; }
    if( mKeyboard[Qt::Key_W] ) { dy = inc; }
    if( mKeyboard[Qt::Key_Q] ) { dz = -inc; }
    if( mKeyboard[Qt::Key_E] ) { dz = inc; }
    
    switch (mCameraMode)
    {
        case cmRotateAround:
        {
            Vector3 z = c.cameraDeltaToWorld(Vector3(0, 0, 1));
            
            c.translate(-z * dy);
        } break;
            
        case cmFree:
        {
            const Vector3 x = c.cameraDeltaToWorld(Vector3(1, 0, 0));
            const Vector3 y = c.cameraDeltaToWorld(Vector3(0, 1, 0));
            const Vector3 z = c.cameraDeltaToWorld(Vector3(0, 0, 1));
            
            c.translate(x * dx + (-z * dy) + (y * dz) );
        } break;
            
        default:
            break;
    }
    setCamera(c);
}

void Viewer::loadShader()
{
    mShadedMaterialShader.clear();

    //--- shaders
    FileInfo appPath(Path::getApplicationFilePath());
    string assetsPath = Path::join(appPath.getAbsolutePath(), "../assets");

    mShadedMaterialShader.setName("shadedMaterial");
    mShadedMaterialShader.addSourceFromFile(stVertex, Path::join(assetsPath, "main.vert"));
    mShadedMaterialShader.addSourceFromFile(stFragment, Path::join(assetsPath, "shadedMaterial.frag"));
    mShadedMaterialShader.compile();
    mShadedMaterialShader.link();

    if (mShadedMaterialShader.hasErrors())
    {
        cout << mShadedMaterialShader.getAndClearLastErrors();
    }
}

//-----------------------------------------------------------------------------
void Viewer::initializeGL()
{
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glEnable(GL_FRAMEBUFFER_SRGB); 
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

#ifdef WIN32
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		printf("Glew initialization failed...\n");
		exit(1);
	}
#endif // WIN32

    loadShader();    
}

//-----------------------------------------------------------------------------
void Viewer::keyPressEvent(QKeyEvent* ipE)
{
    mKeyboard[ipE->key()] = true;

    mKeyboard[ipE->modifiers()] = true;
}

//-----------------------------------------------------------------------------
void Viewer::keyReleaseEvent(QKeyEvent* ipE)
{
    mKeyboard[ipE->key()] = false;
    mKeyboard[ipE->modifiers()] = false;

    if ((ipE->modifiers()==Qt::ControlModifier) && (ipE->key()==Qt::Key_R))
    {
        std::cout << "Reload shaders" << std::endl;
        loadShader();
    }
}

//-----------------------------------------------------------------------------
void Viewer::mouseMoveEvent(QMouseEvent* ipE)
{
    const Math::Vector2i currentPos(ipE->x(), ipE->y());
    if (mMousePos.x() != -1)
    {
        mMouseDelta = currentPos - mMousePos;
    }
    mMousePos = currentPos;
}

//-----------------------------------------------------------------------------
void Viewer::mousePressEvent(QMouseEvent* ipE)
{
    mMouseLeftPressed = ipE->buttons() & Qt::LeftButton;
    mMouseRightPressed = ipE->buttons() & Qt::RightButton;

    mMousePosWhenPressed.set(ipE->x(), ipE->y());
}

//-----------------------------------------------------------------------------
void Viewer::mouseReleaseEvent(QMouseEvent* ipE)
{
    if(ipE->button() == Qt::LeftButton){ mMouseLeftPressed = false; }
    if(ipE->button() == Qt::RightButton){ mMouseRightPressed = false; }
}

//-----------------------------------------------------------------------------
void Viewer::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Matrix4 modelMatrix(Vector3(0.0, 0.0, 0.0));
    //modelMatrix.setAsScaling(Vector3(100, 100, 100));

    glUseProgram(mShadedMaterialShader.getProgramId());

    mShadedMaterialShader.setUniform("uProjectionMatrix", mCamera.getProjectionMatrix());
    mShadedMaterialShader.setUniform("uViewMatrix", mCamera.getViewMatrix());
    mShadedMaterialShader.setUniform("uModelMatrix", modelMatrix);
    mShadedMaterialShader.setUniform("uApplyLighting", true);
    mShadedMaterialShader.setUniform("uLightPosition", Math::Vector3(-1, 1, 1) );
    
    //draw axis
    mShadedMaterialShader.setUniform("uApplyLighting", false);
    mPainter.drawAxis();

    mShadedMaterialShader.setUniform("uApplyLighting", true);
    mPainter.drawCube();

    glUseProgram(0);
}

//-----------------------------------------------------------------------------
void Viewer::resizeGL(int iW, int iH)
{
    Viewport v(iW, iH);
    
    mCamera.setViewport(v);
}

//-----------------------------------------------------------------------------
void Viewer::setCamera(Camera& iC)
{
    mCamera = iC;
    update();
}

//-----------------------------------------------------------------------------
void Viewer::setCameraMovementIncrement(double iInc)
{
	mCameraMovementIncrement = iInc;
}

//-----------------------------------------------------------------------------
void Viewer::setWorldUpVector(const Vector3& iUp)
{
	mWorldUpVector = iUp;
	update();
}