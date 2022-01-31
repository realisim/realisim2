#pragma once

#include "Reactor/Systems/ISystem.h"


namespace Realisim {
namespace Synthroll {

    class TerrainEditionSystem : public Reactor::ISystem
    {
    public:
        TerrainEditionSystem(Reactor::Broker* ipBroker, Reactor::Hub* ipHub);
        virtual ~TerrainEditionSystem();

        virtual bool init() override final;
        virtual void update() override final;

    protected:
        enum State {sIdle, sAddPoints};

        void addNewSpline();
        void addSegmentToSlice();
        void endSegment();

        void handleKeyboard();
        void handleMouse();
        //void handleGamepad();

        Math::Vector3 mousePosToWorld(int iX, int iY);
        void removeSegment(int iSplineIndex, int iSegmentId);
        void setState(State iS);

        State mState;
        int mCurrentSplineIndex;
        int mCurrentSegmentIndex;
    };

}
}

