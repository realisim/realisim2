#pragma once

#include "box2d/b2_body.h"
#include "box2d/b2_world.h"
#include "Geometry/LineSegment.h"
#include "Reactor/Systems/ISystem.h"


namespace Realisim {
namespace Synthroll {

    class PhysicsSystem : public Reactor::ISystem
    {
    public:
        PhysicsSystem(Reactor::Broker* ipBroker, Reactor::Hub* ipHub);
        virtual ~PhysicsSystem();

        enum State{sUndefined, sRunning, sPaused};

        State getState() const { return mState; }
        virtual bool init() override final;
        //void initializeTerrain(const Terrain& iTerrain);
        void pause() { setState(sPaused); }
        void resume() { setState(sRunning); }
        void setTerrain(const Terrain* ipTerrain);
        virtual void update() override final;
        void removeSegment(int iIndex);
        void reset();

    protected:
        void setState(State iV);
        void updateTerrain();

        State mState;
        const Terrain* mpTerrain; //not owned, can be null

        b2World* mpWorld; //owned
        b2Body* mpPlayerBody;

        std::map<int, b2Body*> mLineSegmentToBody; // owned, but no need to delete on destructor
    };

}
}

