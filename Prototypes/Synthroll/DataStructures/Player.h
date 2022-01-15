#pragma once

#include "Reactor/DataStructures/Scene/ModelNode.h"

namespace Realisim {
namespace Synthroll {


    class Player {
    public:
        Player();
        virtual ~Player();

        float getRadius() const { return mRadius; }
        const Reactor::ModelNode& getModelNode() const { return *mpModel; }
        Reactor::ModelNode& getModelNodeRef() { return *mpModel; }
        void setModelNode(Reactor::ModelNode* ipModel) { mpModel = ipModel; }
        void setRadius(float iV) { mRadius = iV; }

    protected:
        float mRadius;
        Reactor::ModelNode* mpModel; // should never be null... NOT OWNED

        
        //float mMaxSpeed;
        //float mWeight;
    };

}
}

