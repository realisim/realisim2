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
Core::Image& Broker::getFinalImage()
{ return mFinalImage; }

//-----------------------------------------------------------------------------
ImageCells& Broker::getImageCells()
{ return mImageCells; }

//-----------------------------------------------------------------------------
Scene& Broker::getScene()
{ return mScene; }