

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
const ISystem* Hub::getUserDefinedSystem (int iSystem) const
{
    return const_cast<const ISystem*>(const_cast<Hub*>(this)->getUserDefinedSystemPtr(iSystem));
}

//---------------------------------------------------------------------------------------------------------------------
ISystem* Hub::getUserDefinedSystemPtr(int iSystem)
{
    ISystem* r = nullptr;

    auto it = mUserDefinedSystems.find(iSystem);
    if (it != mUserDefinedSystems.end())
    {
        r = it->second;
    }

    return r;
}