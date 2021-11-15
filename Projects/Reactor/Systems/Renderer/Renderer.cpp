
#include "Broker.h"
#include "Hub.h"
#include "Renderer.h"

using namespace Realisim;
using namespace Reactor;

//---------------------------------------------------------------------------------------------------------------------
Renderer::Renderer(Broker* ipBroker, Hub* ipHub) : ISystem(ipBroker, ipHub)
{}

//---------------------------------------------------------------------------------------------------------------------
Renderer::~Renderer()
{
    clear();
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::clear()
{
    
}
