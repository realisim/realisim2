
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
    // The orientaion can be paysage, meaning the viewport is wider than tall.
    // Portrait, means it is taller than wide. It is to note, that the viewport
    // will report being paysage when width and height are equal.
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
            
        enum Orientation { oPaysage, oPortrait };
            
        int getHeight() const;
		Orientation getOrientation() const;
        double getRatio() const;        
        Math::Vector2 getSize() const;
        int getWidth() const;
		bool operator==(const Viewport& iP);
		bool operator!=(const Viewport& iP);
		void set(int, int);
            
    protected:
        Orientation mOrientation;
        int mWidth;
        int mHeight;
    };
}
}
