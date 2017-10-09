#pragma once
#include "DataStructure/ImageCells.h"

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
        void rayCast(ImageCells::Node*);
        void render(ImageCells::Node*);
        
        Broker &mBrokerRef;
    };
}
}