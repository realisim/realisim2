#pragma once

#include "Math/VectorI.h"
#include "Rendering/Gpu/Context.h"
#include "Rendering/Gpu/Device.h"
#include "Systems/ISystem.h"


namespace Realisim
{
namespace Reactor
{
    class Broker;
    class Hub;

    class Renderer : public ISystem {
    public:
        Renderer(Broker* ipBroker, Hub* ipHub);
        virtual ~Renderer();

        void clear();

        virtual bool init() override final;

        virtual void preUpdate() override final;
        virtual void update() override final;

        void resizeGl(int iWidth, int iHeight);
        void setScene();
        void setNativeWindowsGlContext(HDC iHDC, HGLRC iHGLRC);
        void swapBuffers();

    protected:
        bool initializeGl();
        void draw();

        Rendering::Device mDevice;
        Rendering::Context mContext;
    };

}

}