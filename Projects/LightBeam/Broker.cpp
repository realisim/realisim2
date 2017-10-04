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
FrameBuffer& Broker::getFrameBuffer()
{ return mFrameBuffer; }