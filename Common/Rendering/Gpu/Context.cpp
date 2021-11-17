#include "Context.h"

#include "OpenGlHeaders.h"

using namespace Realisim;
using namespace Rendering;

//-----------------------------------------------------------------------------
Context::Context() : 
    mIsValid(false),
    mDC(0),
    mGLRC(0),
    mOpengGLVersion("unknown")
{
    
}

//-----------------------------------------------------------------------------
Context::~Context()
{
    clear();
}

//-----------------------------------------------------------------------------
void Context::clear()
{
    doneCurrent();
    wglMakeCurrent(NULL, NULL);

    mOpengGLVersion = "unknown";

    mIsValid = false;
    // To verify, the context was created by Qt (QWindow with GL surface enabled)
    // and properly parented, I believe the DC and GLRC will be properly deleted
    // when the QApp will be destroyed, as the rest the Ui (widgets). So there
    // is no need to delete DC and GLRC here since they are not owned.
    //
}

//-----------------------------------------------------------------------------
void Context::doneCurrent()
{
    wglMakeCurrent(mDC, NULL);
}

//-----------------------------------------------------------------------------
HDC Context::getDC()
{
    return mDC;
}

//-----------------------------------------------------------------------------
HGLRC Context::getGLRC()
{
    return mGLRC;
}

//-----------------------------------------------------------------------------
bool Context::isValid() const
{
    return mIsValid;
}

//-----------------------------------------------------------------------------
void Context::makeCurrent()
{
    wglMakeCurrent(mDC, mGLRC);
}

//-----------------------------------------------------------------------------
void Context::wrapWGLContext(HDC iHDC, HGLRC iHGLRC)
{
    clear();
    if (iHDC != NULL && iHGLRC != NULL)
    {
        mDC = iHDC;
        mGLRC = iHGLRC;

        // get context infos
        this->makeCurrent();
        std::string openglVersion = (char*)glGetString(GL_VERSION);
        mOpengGLVersion = openglVersion.substr(0,openglVersion.find(" "));
        this->doneCurrent();

        // we need a way to validate that the context is valid...
        //
        mIsValid = true;
    }
}

std::string Context::getOpenGLVersion() const
{
    return mOpengGLVersion;
}