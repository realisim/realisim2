#include "3d/Viewport.h"

using namespace Realisim;
using namespace Math;
using namespace TreeD;


Viewport::Viewport() :
mOrientation(oHorizontal),
mWidth(1),
mHeight(1)
{}

//--------------------------------------------------------------------------
Viewport::Orientation Viewport::orientation() const
{ return mOrientation; }

//--------------------------------------------------------------------------
int Viewport::height() const
{ return mHeight; }

//--------------------------------------------------------------------------
int Viewport::width() const
{ return mWidth; }

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
