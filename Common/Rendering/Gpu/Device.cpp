
#include "Device.h"

#include "OpenGlHeaders.h"

using namespace Realisim;
using namespace Rendering;

//-----------------------------------------------------------------------------
Device::Device() :
    mContexts(),
    mManufacturer("unknown"),
    mDeviceName("unknown"),
    mDriver("unknown")
{}

//-----------------------------------------------------------------------------
Device::~Device()
{
    clear();
}

//-----------------------------------------------------------------------------
void Device::clear()
{
    mContexts.clear();
    mManufacturer = "unknown";
    mDeviceName = "unknown";
    mDriver = "unknown";
}

//-----------------------------------------------------------------------------
Context Device::createGlContextFromWindowsHandles(HDC iHDC, HGLRC iHGLRC)
{
    Context r;
    r.wrapWGLContext(iHDC, iHGLRC);

    if (r.isValid())
    {
        mContexts.push_back(r);

        r.makeCurrent();
        // ask infos about the device
        mManufacturer = (char*)glGetString(GL_VENDOR);
        mDeviceName = (char*)glGetString(GL_RENDERER);

        std::string openglVersion = (char*)glGetString(GL_VERSION);
        mDriver = openglVersion.substr(openglVersion.rfind(" "));
        r.doneCurrent();
    }
    return r;
}

//-----------------------------------------------------------------------------
Context Device::getContext(int iIndex)
{
    if(iIndex >= 0 && iIndex < getNumberOfContexts())
    {
        return mContexts[iIndex];
    }
    return Context();
}

//-----------------------------------------------------------------------------
int Device::getNumberOfContexts() const
{
    return (int)mContexts.size();
}

std::string Device::getDeviceName() const
{
    return mDeviceName;
}
std::string Device::getDriverVersion() const
{
    return mDriver;
}
std::string Device::getManufacturer() const
{
    return mManufacturer;
}