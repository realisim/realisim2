
#include <cassert>
#include "Systems/ISystem.h"
#include "Systems/Renderer/Renderer.h"
#include "Hub.h"

using namespace Realisim;
using namespace Reactor;

//---------------------------------------------------------------------------------------------------------------------
void Hub::clear() {
    mpRenderer = nullptr;    
    mUserDefinedSystems.clear();
}

//---------------------------------------------------------------------------------------------------------------------
ISystem* Hub::getSystem(System iSystem)
{
    ISystem* r = nullptr;
    switch (iSystem)
    {
    case sRenderer: r = mpRenderer; break;
    default: assert(false);  break;
    }

    return r;
}

//---------------------------------------------------------------------------------------------------------------------
ISystem* Hub::getUserDefinedSystem(int iSystem)
{
    ISystem* r = nullptr;

    auto it = mUserDefinedSystems.find(iSystem);
    if (it != mUserDefinedSystems.end())
    {
        r = it->second;
    }

    return r;
}