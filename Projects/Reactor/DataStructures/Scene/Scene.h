#pragma once

#include "3d/Scene/SceneNode.h"

namespace Realisim
{
namespace Reactor
{
    class Scene
    {
    public:
        Scene();
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;
        ~Scene();

        void clear();
        const ThreeD::SceneNode& getRoot() const { return *mpRoot; }
        ThreeD::SceneNode& getRootRef() { return *mpRoot; }

    protected:
        ThreeD::SceneNode* mpRoot; // never null

    };

}
}

