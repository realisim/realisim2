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
FrameBuffer& Broker::getFrameBuffer()
{ return mFrameBuffer; }

//-----------------------------------------------------------------------------
Scene& Broker::getScene()
{ return mScene; }