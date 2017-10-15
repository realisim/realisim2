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
Interface::Keyboard& Broker::getKeyboard()
{return mKeyboard; }

//-----------------------------------------------------------------------------
Interface::Mouse& Broker::getMouse()
{ return mMouse; }

//-----------------------------------------------------------------------------
Scene& Broker::getScene()
{ return mScene; }