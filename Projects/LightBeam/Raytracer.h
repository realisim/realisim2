#pragma once
#include "Core/MessageQueue.h"
#include "DataStructure/ImageCells.h"
#include "Geometry/Frustum.h"

namespace Realisim
{
namespace LightBeam
{
    class Broker;

    class RayTracer
    {
    public:
        RayTracer() = delete;
        RayTracer(Broker *ipBroker);
        RayTracer(const RayTracer&) = delete;
        RayTracer& operator=(const RayTracer&) = delete;
        ~RayTracer();
        
        Broker& getBroker();
        void render();
        
    protected:
    
        void rayCast(ImageCells& iCells, const Math::Vector2i& iCell, const Geometry::Frustum& iFrustum);
        void render(ImageCells& iCells);
        
        Broker &mBrokerRef;
//        Core::MessageQueue mRenderQueue;
//        Core::MessageQueue mDoneQueue;
    };
}
}