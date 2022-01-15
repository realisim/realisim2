#pragma once

#include "DataStructures/Player.h"
#include "DataStructures/Terrain.h"
#include "Geometry/LineSegment.h"
#include "Geometry/Mesh.h"
#include "Reactor/Systems/ISystem.h"


namespace Realisim {
namespace Synthroll {

    class GameSystem : public Reactor::ISystem
    {
    public:
        GameSystem(Reactor::Broker* ipBroker, Reactor::Hub* ipHub);
        virtual ~GameSystem();

        enum Mode{mNone, mMainMenu, mPlaying, mTerrainEdition};

        Mode getMode() const { return mMode; }
        const Player& getPlayer() const { return *mpPlayer0; }
        Player& getPlayerRef() { return *mpPlayer0; }
        const Terrain& getTerrain() const { return mTerrain; }
        Terrain& getTerrainRef() { return mTerrain; }

        virtual bool init() override final;
        virtual bool postInit() override final;
        void setMode(Mode iV);
        virtual void update() override final;
        void updateCamera();

    protected:
        Geometry::Mesh makeMeshFromTerrainSegment(const Geometry::LineSegment&);
        void updateSceneGraph(ThreeD::SceneNode* ipNode);
        void updateTerrain();

        Mode mMode;
        
        Player* mpPlayer0; // owned, never null
        Terrain mTerrain;


    };

}
}

