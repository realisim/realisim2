#pragma once

#include "Systems/ISystem.h"

namespace Realisim
{
namespace Reactor
{
    class Broker;
    class Hub;

    class Renderer : public ISystem {
    public:
        Renderer(Broker* ipBroker, Hub* ipHub);
        virtual ~Renderer();

        void clear();

    protected:
        
    };

}

}