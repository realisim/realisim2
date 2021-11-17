#pragma once

#include "Context.h"
#include <vector>
#include <string>

namespace Realisim
{
namespace Rendering
{
    class Device
    {
    public:
        Device();
        Device(const Device&) = delete;
        Device& operator=(const Device&) = delete;
        ~Device();

        void clear();
        Context getContext(int);
        int getNumberOfContexts() const;
        std::string getDeviceName() const;
        std::string getDriverVersion() const;
        std::string getManufacturer() const;
        Context createGlContextFromWindowsHandles(HDC iHDC, HGLRC iHGLRC);

    protected:
        std::vector<Context> mContexts;

        std::string mManufacturer;
        std::string mDeviceName;
        std::string mDriver;
    };
}
}