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
Core::Image& Broker::getFinalImage()
{ return mFinalImage; }

//-----------------------------------------------------------------------------
Interface::Keyboard& Broker::getKeyboard()
{return mKeyboard; }

//-----------------------------------------------------------------------------
Interface::Mouse& Broker::getMouse()
{ return mMouse; }

//-----------------------------------------------------------------------------
const Scene& Broker::getScene() const
{
    return mScene;
}

//-----------------------------------------------------------------------------
Scene& Broker::getSceneRef()
{ return mScene; }