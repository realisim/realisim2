
#pragma once

#include "Math/Vector.h"

namespace Realisim
{
namespace Rendering
{
    // This class represent a very basic viewport.
    //
    // It presents a width and a height, origin is alway (0, 0) and there is
    // no assumption on the location of the origin (bottomLeft(openGL) vs topLeft(QT) ).
    //
    // The orientaion can be horizontal, meaning the viewport is wider than tall.
    // Vertical, means it is taller than wide. It is to note, that the viewport
    // will report being horizontal when width and height are equal.
    //
    // The ratio function returns the ratio of width over height. The goal is to
    // present the viewport ration as in 16/9 for example.
    //
    class Viewport
    {
    public:
        Viewport();
        Viewport(int iWidth, int iHeight);
        Viewport(const Viewport&) = default;
        Viewport& operator=(const Viewport&) = default;
        ~Viewport() = default;
            
        enum Orientation { oHorizontal, oVertical };
            
        Orientation orientation() const;
        int height() const;
        double ratio() const;
        void set(int, int);
        Math::Vector2 size() const;
        int width() const;
            
    protected:
        Orientation mOrientation;
        int mWidth;
        int mHeight;
    };
}
}
