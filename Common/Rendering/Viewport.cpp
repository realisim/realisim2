#include "Rendering/Viewport.h"

using namespace Realisim;
using namespace Math;
using namespace Rendering;


Viewport::Viewport() :
mOrientation(oHorizontal),
mWidth(1),
mHeight(1)
{}

//--------------------------------------------------------------------------
Viewport::Viewport(int iWidth, int iHeight)
{
    // calling set to recompute orientation...
    //
    set(iWidth, iHeight);
}

//--------------------------------------------------------------------------
Viewport::Orientation Viewport::orientation() const
{ return mOrientation; }

//--------------------------------------------------------------------------
int Viewport::height() const
{ return mHeight; }

//--------------------------------------------------------------------------
double Viewport::ratio() const
{
    return width() / (double)height();
}

//--------------------------------------------------------------------------
void Viewport::set(int iW, int iH)
{
    if(width() != iW || height() != iH)
    {
        mWidth = iW;
        mHeight = iH;

        mOrientation = mHeight > mWidth ? oVertical : oHorizontal; 
    }
}

//--------------------------------------------------------------------------
Vector2 Viewport::size() const
{ return Vector2(width(), height()); }

//--------------------------------------------------------------------------
int Viewport::width() const
{
    return mWidth;
}