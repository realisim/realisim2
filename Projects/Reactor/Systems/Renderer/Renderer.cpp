#include "Rendering/Gpu/OpenGlHeaders.h"

#include "Broker.h"
#include "Hub.h"
#include "Renderer.h"
#include "Rendering/Camera.h"
#include "Rendering/Viewport.h"


using namespace Realisim;
    using namespace Math;
    using namespace Reactor;
    using namespace Rendering;

//---------------------------------------------------------------------------------------------------------------------
Renderer::Renderer(Broker* ipBroker, Hub* ipHub) : ISystem(ipBroker, ipHub)
{}

//---------------------------------------------------------------------------------------------------------------------
Renderer::~Renderer()
{
    clear();
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::clear()
{
    
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::draw()
{
    const Camera& cam = getBroker().getMainCamera();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(cam.getProjectionMatrix().getDataPointer());
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(cam.getViewMatrix().getDataPointer());


    glDisable(GL_LIGHTING);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    const double s = 1.0 / 2;

    glBegin(GL_QUADS);

    //+z
    glColor3ub(255, 0, 0);
    glVertex3d(-s, -s, s);
    glVertex3d(s, -s, s);
    glVertex3d(s, s, s);
    glVertex3d(-s, s, s);

    //-z
    glColor3ub(255, 0, 0);
    glVertex3d(-s, -s, -s);
    glVertex3d(-s, s, -s);
    glVertex3d(s, s, -s);
    glVertex3d(s, -s, -s);

    //+x
    glColor3ub(0, 255, 0);
    glVertex3d(s, -s, -s);
    glVertex3d(s, s, -s);
    glVertex3d(s, s, s);
    glVertex3d(s, -s, s);

    //-x
    glColor3ub(0, 255, 0);
    glVertex3d(-s, -s, -s);
    glVertex3d(-s, -s, s);
    glVertex3d(-s, s, s);
    glVertex3d(-s, s, -s);

    //+y
    glColor3ub(0, 0, 255);
    glVertex3d(-s, s, -s);
    glVertex3d(-s, s, s);
    glVertex3d(s, s, s);
    glVertex3d(s, s, -s);

    //-y
    glColor3ub(0, 0, 255);
    glVertex3d(-s, -s, -s);
    glVertex3d(s, -s, -s);
    glVertex3d(s, -s, s);
    glVertex3d(-s, -s, s);

    glEnd();
}

//---------------------------------------------------------------------------------------------------------------------
bool Renderer::init()
{
    bool r = true;

    // place camera
    Camera& cam = getBrokerRef().getMainCameraRef();
    cam.set(Vector3(10, 10, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

    return r;
}

//---------------------------------------------------------------------------------------------------------------------
bool Renderer::initializeGl()
{
    bool r = true;
    mContext.makeCurrent();

    // init GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        printf("Glew initialization failed... Context must be invalid.\n");
        r = false;
    }

    // set glClearColor
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_FRAMEBUFFER_SRGB);

    // basic opengl states.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);

    // enable vsync
    const int swapInterval = 1; // mConfig.isVSyncEnabled() ? 1 : 0;
    wglSwapIntervalEXT(swapInterval);

    mContext.doneCurrent();

    return r;
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::preUpdate() {
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::update(){
    mContext.makeCurrent();
    
    // draw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw();

    mContext.doneCurrent();
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::resizeGl(int iWidth, int iHeight)
{
    mContext.makeCurrent();

    Camera& cam = getBrokerRef().getMainCameraRef();

    Rendering::Viewport v(iWidth, iHeight);

    // reset camera viewport
    cam.setViewport(v);
    glViewport(0, 0, iWidth, iHeight);

    mContext.doneCurrent();
}


//---------------------------------------------------------------------------------------------------------------------
void Renderer::setScene()
{}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::setNativeWindowsGlContext(HDC iHDC, HGLRC iHGLRC)
{
    mDevice.createGlContextFromWindowsHandles(iHDC, iHGLRC);
    mContext = Rendering::Context();
    if (mDevice.getNumberOfContexts() > 0)
    {
        mContext = mDevice.getContext(mDevice.getNumberOfContexts() - 1);
    }

    initializeGl();
    const Camera& cam = getBroker().getMainCamera();
    resizeGl(cam.getViewport().getWidth(), cam.getViewport().getHeight());
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::swapBuffers() {
    mContext.makeCurrent();

    SwapBuffers(mContext.getDC());
    //glFinish();

    mContext.doneCurrent();
}