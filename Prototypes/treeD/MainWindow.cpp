/**/

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#endif

#include <cmath>
#include "MainWindow.h"
#include <QCoreApplication>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QPushButton>
#include <QString>

using namespace Realisim;
using namespace Math;
using namespace TreeD;

Camera gDefaultCamera;

Viewer::Viewer(QWidget* ipParent /*=0*/) :
QOpenGLWidget(ipParent),
mCamera(),
mCameraMode(cmRotateAround),
mMouseX(-1),
mMouseY(-1),
mMouseDeltaX(0),
mMouseDeltaY(0),
mMouseButtonPressed(false)
{
	setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    
    Projection p;
    Viewport v;
    v.set(400, 400);
    //p.setOrthoProjection(100, 1, 1000.0);
    p.setPerspectiveProjection(32, 16/9.0, 1, 1000.0);
    //p.setProjection(-50, 50, -50, 50, 1, 1000, Projection::tPerspective);
    
    gDefaultCamera.setViewport(v);
    gDefaultCamera.setProjection(p, true);
    gDefaultCamera.set(Vector3(0.0, 0.0, 300),
                Vector3(0.0, 0.0, 0.0),
                Vector3(0.0, 1.0, 0.0) );
    mCamera = gDefaultCamera;
}

Viewer::~Viewer()
{}

//-----------------------------------------------------------------------------
Camera Viewer::camera() const
{
    return mCamera;
}

//-----------------------------------------------------------------------------
void Viewer::drawCube()
{
    glDisable(GL_LIGHTING);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    const int s = 10.0 / 2;
    
    glBegin(GL_QUADS);
    
    //+z
    glColor3ub(255, 0, 0);
    glVertex3d(-s, -s, s);
    glVertex3d( s, -s, s);
    glVertex3d( s,  s, s);
    glVertex3d(-s,  s, s);
    
    //-z
    glColor3ub(255, 0, 0);
    glVertex3d(-s, -s, -s);
    glVertex3d(-s,  s, -s);
    glVertex3d( s,  s, -s);
    glVertex3d( s, -s, -s);
    
    //+x
    glColor3ub(0, 255, 0);
    glVertex3d( s, -s, -s);
    glVertex3d( s,  s, -s);
    glVertex3d( s,  s,  s);
    glVertex3d( s, -s,  s);
    
    //-x
    glColor3ub(0, 255, 0);
    glVertex3d(-s, -s, -s);
    glVertex3d(-s, -s,  s);
    glVertex3d(-s,  s,  s);
    glVertex3d(-s,  s,  -s);
    
    //+y
    glColor3ub(0, 0, 255);
    glVertex3d(-s, s, -s);
    glVertex3d(-s, s,  s);
    glVertex3d( s, s,  s);
    glVertex3d( s, s, -s);
    
    //-y
    glColor3ub(0, 0, 255);
    glVertex3d(-s, -s, -s);
    glVertex3d( s, -s, -s);
    glVertex3d( s, -s,  s);
    glVertex3d(-s, -s,  s);
    
    glEnd();
}

//-----------------------------------------------------------------------------
void Viewer::handleUserInput()
{
    Camera c = camera();
    
    //--- mouse
    const double kDegreeToRadian = M_PI/180.0;
    const double f = 0.2;
    if(mMouseButtonPressed)
    {
        switch (mCameraMode)
        {
            case cmRotateAround:
            {
                Vector3 rotateAboutPos(0.0);
                Vector3 rotateAboutAxisX = c.cameraDeltaToWorld(Vector3(1.0, 0.0, 0.0));
                
                c.rotate( -mMouseDeltaY * kDegreeToRadian * f, rotateAboutAxisX,
                         rotateAboutPos);
                c.rotate( -mMouseDeltaX * kDegreeToRadian * f, Vector3(0.0, 1.0, 0.0),
                         rotateAboutPos);
            }break;
                
            case cmFree:
            {
                Vector3 rotateAboutPos = c.position();
                Vector3 rotateAboutAxisX = c.cameraDeltaToWorld(Vector3(1.0, 0.0, 0.0));
                
                c.rotate( -mMouseDeltaY * kDegreeToRadian * f, rotateAboutAxisX,
                         rotateAboutPos);
                c.rotate( -mMouseDeltaX * kDegreeToRadian * f , Vector3(0.0, 1.0, 0.0),
                         rotateAboutPos);
            }break;
                
            default: break;
        }
    }
    mMouseDeltaX = 0;
    mMouseDeltaY = 0;
    
    //--- keyboard
    const double inc = 3.0;
    
    int dx = 0;
    int dy = 0;
    
    if( mKeyboard[Qt::Key_A] ) { dx = -inc; }
    if( mKeyboard[Qt::Key_D] ) { dx = inc; }
    if( mKeyboard[Qt::Key_S] ) { dy = -inc; }
    if( mKeyboard[Qt::Key_W] ) { dy = inc; }
    
    switch (mCameraMode)
    {
        case cmRotateAround:
        {
            Vector3 z = c.cameraDeltaToWorld(Vector3(0, 0, 1));
            
            c.translate(-z * dy);
        } break;
            
        case cmFree:
        {
            Vector3 z = c.cameraDeltaToWorld(Vector3(0, 0, 1));
            Vector3 x = c.cameraDeltaToWorld(Vector3(1, 0, 0));
            
            c.translate(-z * dy + x * dx);
        } break;
            
        default:
            break;
    }
    setCamera(c);
}

//-----------------------------------------------------------------------------
void Viewer::initializeGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
    
    f->glEnable(GL_FRAMEBUFFER_SRGB);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

//-----------------------------------------------------------------------------
void Viewer::keyPressEvent(QKeyEvent* ipE)
{
    mKeyboard[ipE->key()] = true;
}

//-----------------------------------------------------------------------------
void Viewer::keyReleaseEvent(QKeyEvent* ipE)
{
    mKeyboard[ipE->key()] = false;
}

//-----------------------------------------------------------------------------
void Viewer::mouseMoveEvent(QMouseEvent* ipE)
{
    mMouseButtonPressed = ipE->buttons() & Qt::LeftButton;
    
    if(mMouseX != -1)
    {
        mMouseDeltaX = ipE->x() - mMouseX;
        mMouseDeltaY = ipE->y() - mMouseY;
    }
    
    mMouseX = ipE->x();
    mMouseY = ipE->y();
}

//-----------------------------------------------------------------------------
void Viewer::mousePressEvent(QMouseEvent* ipE)
{
    Vector2 pixel(ipE->x(), ipE->y());
    
//    Vector3 screenToWorld = camera().screenToWorld(pixel, Vector3(0.0));
//    Vector2 worldToScreen = camera().worldToScreen(Vector3(-30, 0, 0) );
//    
//    printf("screenToWorld: %s\n", screenToWorld.toString().c_str());
//    printf("worldToScreen: %s\n", worldToScreen.toString().c_str());
}

//-----------------------------------------------------------------------------
void Viewer::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(mCamera.projectionMatrix().getDataPointer() );
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(mCamera.viewMatrix().getDataPointer());
    
    for(int i = -2; i <= 2; ++i)
        for(int j = -2; j <= 2; ++j)
        {
            glPushMatrix();
            
            glTranslated( i * 30, j * 30, 0.0 );
            drawCube();
            
            glPopMatrix();
        }
}

//-----------------------------------------------------------------------------
void Viewer::resizeGL(int iW, int iH)
{
    Viewport v;
    v.set(iW, iH);
    mCamera.setViewport(v);
}

//-----------------------------------------------------------------------------
void Viewer::setCamera(Camera& iC)
{
    mCamera = iC;
    update();
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

	QHBoxLayout* pLyt = new QHBoxLayout(pCentralWidget);
	pLyt->setMargin(5);
	{
		QVBoxLayout *pControlLyt = new QVBoxLayout();
		{
            QGroupBox* pCameraProjGrp = new QGroupBox("Camera Projection", pCentralWidget);
            {
                QHBoxLayout *camProjLyt = new QHBoxLayout(pCameraProjGrp);
                {
                    mpOrthoProj = new QRadioButton("Ortho", pCameraProjGrp);
                    connect(mpOrthoProj, SIGNAL(clicked()),
                            this, SLOT(cameraProjOrthoClicked()));
                    
                    mpPerspectiveProj = new QRadioButton("Perspective", pCameraProjGrp);
                    connect(mpPerspectiveProj, SIGNAL(clicked()),
                            this, SLOT(cameraProjPerspectiveClicked()));
                }
                camProjLyt->addWidget(mpOrthoProj);
                camProjLyt->addWidget(mpPerspectiveProj);
            }
            
            QGroupBox* pCameraModeGrp = new QGroupBox("Camera Mode", pCentralWidget);
            {
                QHBoxLayout *camModeLyt = new QHBoxLayout(pCameraModeGrp);
                {
                    mpCameraRotateAbout = new QRadioButton("Rotate about", pCameraModeGrp);
                    connect(mpCameraRotateAbout, SIGNAL(clicked()),
                            this, SLOT(cameraRotateAboutClicked()));
                    
                    mpCameraFree = new QRadioButton("Free", pCameraModeGrp);
                    connect(mpCameraFree, SIGNAL(clicked()),
                            this, SLOT(cameraFreeClicked()));
                }
                camModeLyt->addWidget(mpCameraRotateAbout);
                camModeLyt->addWidget(mpCameraFree);
            }
            
            QHBoxLayout *pZoomLayout = new QHBoxLayout();
            {
                QLabel *pl = new QLabel("zoom: ", pCentralWidget);
                mpZoomFactor = new QSlider(pCentralWidget);
                mpZoomFactor->setOrientation(Qt::Horizontal);
                mpZoomFactor->setMinimum(1);
                mpZoomFactor->setMaximum(100);
                mpZoomFactor->setValue(10);
                connect(mpZoomFactor, SIGNAL(valueChanged(int)),
                        this, SLOT(zoomFactorChanged(int)));
                
                mpZoomFactorLabel = new QLabel("1.0", pCentralWidget);
                
                pZoomLayout->addWidget(pl);
                pZoomLayout->addWidget(mpZoomFactor);
                pZoomLayout->addWidget(mpZoomFactorLabel);
            }
            
            pControlLyt->addWidget(pCameraProjGrp);
            pControlLyt->addWidget(pCameraModeGrp);
            pControlLyt->addLayout(pZoomLayout);
            pControlLyt->addStretch(1);
		}
        

		mpViewer = new Viewer(pCentralWidget);

		pLyt->addLayout(pControlLyt, 1);
		pLyt->addWidget(mpViewer, 4);
	}
    
    mTimerId = startTimer(0);
	updateUi();
}

//-----------------------------------------------------------------------------
void MainWindow::cameraProjOrthoClicked()
{
    Camera c = mpViewer->camera();
    Projection p = c.projection();
    p.setProjection(-100, 100,
                    -100, 100,
                    p.near(), p.far(),
                    Projection::tOrthogonal);
    c.setProjection(p, true);
    mpViewer->setCamera(c);
    
    updateUi();
}

//-----------------------------------------------------------------------------
void MainWindow::cameraProjPerspectiveClicked()
{
    Camera c = mpViewer->camera();
    Projection p = gDefaultCamera.projection();
    c.setProjection(p, true);
    mpViewer->setCamera(c);

    updateUi();
}

//-----------------------------------------------------------------------------
void MainWindow::cameraRotateAboutClicked()
{
    mpViewer->setCamera(gDefaultCamera);
    
    mpViewer->mCameraMode = Viewer::cmRotateAround;
    updateUi();
}

//-----------------------------------------------------------------------------
void MainWindow::cameraFreeClicked()
{
    mpViewer->mCameraMode = Viewer::cmFree;
    updateUi();
}

//-----------------------------------------------------------------------------
void MainWindow::timerEvent(QTimerEvent *ipE)
{
    if(ipE->timerId() == mTimerId)
    {
        mpViewer->handleUserInput();
        mpViewer->update();
    }
}

//-----------------------------------------------------------------------------
void MainWindow::updateUi()
{
    switch (mpViewer->camera().projection().type()) {
        case Projection::tOrthogonal:
            mpOrthoProj->setChecked(true);
            break;
        
        case Projection::tPerspective:
            mpPerspectiveProj->setChecked(true);
            break;
            
        default:
            break;
    }
    
    switch (mpViewer->mCameraMode)
    {
        case Viewer::cmRotateAround:
            mpCameraRotateAbout->setChecked(true); break;
        
        case Viewer::cmFree:
            mpCameraFree->setChecked(true); break;
            
        default:
            break;
    }
    
    //--- zoom
    const double z = mpViewer->camera().zoomFactor();
    mpZoomFactor->setValue( z * 10 );
    mpZoomFactorLabel->setText( QString::number(mpViewer->camera().zoomFactor(), 'f', 1) );
	update();
	mpViewer->update();
}

//-----------------------------------------------------------------------------
void MainWindow::zoomFactorChanged(int iV)
{
    const double maxZoom = 10.0;
    const double t = iV / 100.0;
    Camera c = mpViewer->camera();
    c.setZoomFactor(t*maxZoom);
    mpViewer->setCamera(c);
    updateUi();
}
