#pragma once
#include "Core/MessageQueue.h"
#include "DataStructure/ImageCells.h"
#include "Geometry/Frustum.h"
#include "Math/Vector.h"

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
            Math::Vector2i mSize;
            int mDivideBy;
        };
        
        class Reply : public Core::MessageQueue::Message
        {
        public:
            explicit Reply(void* ipSender = nullptr);
            ImageCells mCells;
        };
    
        void processReply(Core::MessageQueue::Message*);
        void processMessage(Core::MessageQueue::Message*);
        void rayCast(ImageCells& iCells, const Math::Vector2i& iCell, const Geometry::Frustum& iFrustum);
        void render(ImageCells& iCells);
        
        Broker &mBrokerRef;
        Core::MessageQueue mMessageQueue;
        mutable Core::MessageQueue mReplyQueue;
    };
}
}