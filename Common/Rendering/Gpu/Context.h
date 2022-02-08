#pragma once

#include <Windows.h>
#include <string>

namespace Realisim
{
namespace Rendering
{
    class Context
    {
    public:
        Context();
        Context(const Context&) = default;
        Context& operator=(const Context&) = default;
        ~Context();

        //createVao();
        void clear();
        void doneCurrent();
        HDC getDC();
        HGLRC getGLRC();
        std::string getOpenGLVersion() const;
        bool isInitialized() const { return mIsInitialized; }
        bool isValid() const;
        void makeCurrent();
        void setAsInitialized(bool iV) { mIsInitialized = iV; }
        void wrapWGLContext(HDC iHDC, HGLRC iHGLRC);

    protected:        

        bool mIsInitialized;
        bool mIsValid;
        HDC mDC; //not owned
        HGLRC mGLRC; //not owned

        std::string mOpengGLVersion;
    };
}
}