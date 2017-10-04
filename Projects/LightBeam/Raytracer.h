#pragma once


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

        void render();
        
        Broker& getBroker();
    protected:
        Broker &mBrokerRef;
    };
}
}