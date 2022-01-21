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
        void forward(int iIncrement);
        void pause() { setState(sPaused); }
        void resume();
        void rewind(int iIncrement);
        void setTerrain(const Terrain* ipTerrain);
        virtual void update() override final;
        void removeSegment(int iIndex);
        void reset();

    protected:
        struct ReplayRecord {
            b2Vec2 mPosition;
            b2Vec2 mLinearVelocity;
            float mAngularVelocity;
            float mAngle;
        };

        void setState(State iV);
        void updateTerrain();

        State mState;
        const Terrain* mpTerrain; //not owned, can be null

        b2World* mpWorld; //owned
        b2Body* mpPlayerBody;

        std::map<int, b2Body*> mLineSegmentToBody; // owned, but no need to delete on destructor

        std::vector<ReplayRecord> mPlayerReplayRecords;
        int mCurrentRecordIndex;
        int mLastRecordIndex;
    };

}
}

