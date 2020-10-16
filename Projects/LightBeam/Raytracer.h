#pragma once
#include "Core/Image.h"
#include "Core/MessageQueue.h"
#include "DataStructure/ImageCells.h"
#include "Geometry/Frustum.h"
#include "Math/Vector.h"
#include "SimpleIntegrator.h"

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
        class RaytraceRequest : public Core::MessageQueue::Message
        {
        public:
            explicit RaytraceRequest(void* ipSender = nullptr);
            Math::Vector2i mSize;
            double mDivideBy;
        };
        
        class RaytraceReply : public Core::MessageQueue::Message
        {
        public:
            explicit RaytraceReply(void* ipSender = nullptr);
            Core::Image mImage;
        };
    
        int fillPixels(Core::Image *opImage, const ImageCells& iCells, const Math::Vector2i& iCellIndex, const Geometry::Rectangle& iCellCoverage);
        int fillPixel(Core::Image *opImage, const ImageCells& iCells, const Math::Vector2i& iCellIndex, const Geometry::Rectangle& iCellCoverage);
        void processReply(Core::MessageQueue::Message*);
        void processMessage(Core::MessageQueue::Message*);
        void rayCast(ImageCells& iCells, const Math::Vector2i& iCell);
        void rayCast(int iDepth, const Geometry::Line& iRay, const Scene& iScene, const Rendering::Camera& iCamera, Core::Color *opColor, double *opDistanceToCamera);
        Core::Image reconstructImage(const ImageCells&);
        void render(ImageCells& iCells);
        
        Broker &mBrokerRef;
        Core::MessageQueue mMessageQueue;
        mutable Core::MessageQueue mReplyQueue;
        
        SimpleIntegrator mIntegrator;
    };
}
}