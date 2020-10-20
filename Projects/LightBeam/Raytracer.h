#pragma once
#include "Core/Image.h"
#include "Core/MessageQueue.h"
#include "DataStructure/ImageCells.h"
#include "Geometry/Frustum.h"
#include "Math/Vector.h"
#include "SimpleIntegrator.h"
#include <vector>

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
        int getLevelOfDetail() const { return mDesiredLevelOfDetail; }
        void render(int iLevelOfDetail);
        const Core::Image& getImage() const;
        
    protected:
        class RaytraceRequest : public Core::MessageQueue::Message
        {
        public:
            explicit RaytraceRequest(void* ipSender = nullptr);

            Geometry::Rectangle mCoverage;
            int mTileSizeInPixel;
            int mId;
        };
        
        class RaytraceReply : public Core::MessageQueue::Message
        {
        public:
            explicit RaytraceReply(void* ipSender = nullptr);
            //Core::Image mImage;
            Geometry::Rectangle mCoverage;
            ImageCells mRenderedCells;
            int mId;
        };
    
        int fillPixels(Core::Image *opImage, const ImageCells& iCells, const Math::Vector2i& iCellIndex, const Geometry::Rectangle& iCellCoverage);
        int fillPixel(Core::Image *opImage, const ImageCells& iCells, const Math::Vector2i& iCellIndex, const Geometry::Rectangle& iCellCoverage);
        void mergeImage(Core::Image *opImage, ImageCells&);
        void processReply(Core::MessageQueue::Message*);
        void processMessage(Core::MessageQueue::Message*);
        void rayCast(ImageCells* iCells, const Math::Vector2i& iCell);
        void rayCast(int iDepth, const Geometry::Line& iRay, const Scene& iScene, const Rendering::Camera& iCamera, Core::Color *opColor, double *opDistanceToCamera);
        void render(ImageCells* iCells);
        
        Broker &mBrokerRef;
        Core::MessageQueue mMessageQueue;
        mutable Core::MessageQueue mReplyQueue;
        int mDesiredLevelOfDetail;
        Core::Image mFinalImage;

        SimpleIntegrator mIntegrator;
    };
}
}