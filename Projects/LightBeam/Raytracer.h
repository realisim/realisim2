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
        
        bool hasNewFrameAvailable() const;
        Broker& getBroker();
        void render();
        
    protected:
        class Message : public Core::MessageQueue::Message
        {
        public:
            explicit Message(void* ipSender = nullptr);
            
            enum Action{aNone, aStopRendering, aRender, aFrameDone};
            Action mAction;
        };
    
        Message* makeMessage(Message::Action);
        void processRenderMessage(Core::MessageQueue::Message*);
        void rayCast(ImageCells& iCells, const Math::Vector2i& iCell, const Geometry::Frustum& iFrustum);
        void render(ImageCells& iCells);
        
        Broker &mBrokerRef;
        Core::MessageQueue mRenderQueue;
        mutable Core::MessageQueue mDoneQueue;
    };
}
}