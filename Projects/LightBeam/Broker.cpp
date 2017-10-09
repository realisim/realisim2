#include "Broker.h"

using namespace Realisim;
    using namespace LightBeam;

//-----------------------------------------------------------------------------
Broker::Broker()
{}

//-----------------------------------------------------------------------------
Broker::~Broker()
{}

//-----------------------------------------------------------------------------
Rendering::Camera& Broker::getCamera()
{ return mCamera; }

//-----------------------------------------------------------------------------
ImageCells& Broker::getImageCells()
{ return mImageCells; }

//-----------------------------------------------------------------------------
FrameBuffer& Broker::getFrameBuffer()
{ return mFrameBuffer; }

//-----------------------------------------------------------------------------
Scene& Broker::getScene()
{ return mScene; }