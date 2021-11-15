
#pragma once

#include <array>
#include <map>

namespace Realisim
{
namespace Reactor{

    class ISystem;
    class Renderer;

    class Hub
    {
    public:
        friend class Engine;

        Hub() = default;
        Hub(const Hub&) = delete;
        Hub&operator=(const Hub&) = delete;
        ~Hub() = default;
        
        enum System {sRenderer = 0, sNumberOfSystems, sUserDefined = 10000};

        const Renderer& getRenderer() const;
        Renderer& getRendererRef() const;

        ISystem* getSystem(System);
        ISystem* getUserDefinedSystem(int iSystem);

    protected:
        void clear(); // only called by Engine.

        // not owned pointers to all core systems
        Renderer* mpRenderer;

        // owned pointer to all userDefined systems
        std::map<int, ISystem*> mUserDefinedSystems;
    };
}
}