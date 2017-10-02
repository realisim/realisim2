#include "Rendering/Viewport.h"

using namespace Realisim;
using namespace Math;
using namespace Rendering;


Viewport::Viewport() :
mOrientation(oPaysage),
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
int Viewport::getHeight() const
{
	return mHeight;
}

//--------------------------------------------------------------------------
Viewport::Orientation Viewport::getOrientation() const
{ return mOrientation; }

//--------------------------------------------------------------------------
double Viewport::getRatio() const
{
    return getWidth() / (double)getHeight();
}

//--------------------------------------------------------------------------
Vector2 Viewport::getSize() const
{
	return Vector2(getWidth(), getHeight());
}

//--------------------------------------------------------------------------
int Viewport::getWidth() const
{
	return mWidth;
}

//--------------------------------------------------------------------------
bool Viewport::operator==(const Viewport& iV)
{
	return mOrientation == iV.mOrientation &&
		mWidth == iV.mWidth &&
		mHeight == iV.mHeight;
}

//--------------------------------------------------------------------------
bool Viewport::operator!=(const Viewport& iV)
{
	return !operator==(iV);
}

//--------------------------------------------------------------------------
void Viewport::set(int iW, int iH)
{
    if(getWidth() != iW || getHeight() != iH)
    {
        mWidth = iW;
        mHeight = iH;

        mOrientation = mHeight > mWidth ? oPortrait : oPaysage; 
    }
}