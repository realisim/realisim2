
#include "Broker.h"
#include <cassert>
#include "Core/Timer.h"
#include "Geometry/Intersections.h"
#include "IntersectionResult.h"
#include "Math/Vector.h"
#include "Math/VectorI.h"
#include "RayTracer.h"
#include "Utilities.h"
#include "VisibilityTester.h"

using namespace Realisim;
    using namespace Core;
    using namespace Geometry;
    using namespace LightBeam;
    using namespace Math;
    using namespace Rendering;
using namespace std;

namespace  {
    const int kMaxRecursionDepth = 5;
    const int kTileSize = 64;
    int kNumThreads = 1;
}

//-----------------------------------------------------------------------------
RayTracer::RayTracer(Broker *ipBroker) :
    mBrokerRef(*ipBroker)
{
    // set max number of threads
    kNumThreads = std::thread::hardware_concurrency();

    using placeholders::_1;
    function<void(MessageQueue::Message*)> f =
        bind(&RayTracer::processMessage, this, _1);
    mMessageQueue.setProcessingFunction(f);
    mMessageQueue.setMaximumSize(-1);
    mMessageQueue.setNumberOfThreads(kNumThreads);
    mMessageQueue.setBehavior(MessageQueue::bFifo);
    mMessageQueue.startInThread();
    
    f = bind(&RayTracer::processReply, this, _1);
    mReplyQueue.setProcessingFunction(f);
    mMessageQueue.setBehavior(MessageQueue::bFifo);
    mReplyQueue.setMaximumSize(-1);
}

//-----------------------------------------------------------------------------
RayTracer::~RayTracer()
{
    mMessageQueue.stopThread();
}

//-----------------------------------------------------------------------------
int RayTracer::fillPixels(Core::Image *opImage,
                           const ImageCells& iCells,
                           const Math::Vector2i& iCellIndex,
                           const Geometry::Rectangle& iCellCoverage)
{
    const Vector2 bl = iCellCoverage.getBottomLeft();
    const Vector2 tr = iCellCoverage.getTopRight();
    
    Color c;
    for(int y = (int)bl.y(); y < (int)tr.y(); ++y)
    {
        for(int x = (int)bl.x(); x < (int)tr.x(); ++x)
        {
            c = iCells.getCellColor(iCellIndex);
            opImage->setPixelColor(Vector2i(x,y), c);
        }
    }
    
    return 1;
}

//-----------------------------------------------------------------------------
int RayTracer::fillPixel(Core::Image *opImage,
                          const ImageCells& iCells,
                          const Math::Vector2i& iCellIndex,
                          const Geometry::Rectangle& iCellCoverage)
{
    const int increment = (int)round(1.0 / iCellCoverage.getWidth());
    
//printf("fillPixel: increment = %d\n", increment);
    
    Color c;
    for(int y = 0; y < increment; ++y)
        for(int x = 0; x < increment; ++x)
        {
            c += iCells.getCellColor(iCellIndex + Vector2i(x, y));
        }
    
    c /= (double)(increment*increment);
    opImage->setPixelColor(iCellIndex/increment, c);

    return increment;
}

//-----------------------------------------------------------------------------
const Core::Image& RayTracer::getImage() const
{
    return mFinalImage;
}

//-----------------------------------------------------------------------------
bool RayTracer::hasNewFrameAvailable() const
{
    bool r = !mReplyQueue.isEmpty();
    
    //
    mReplyQueue.processMessages();
    return r;
}

//-----------------------------------------------------------------------------
Broker& RayTracer::getBroker()
{ return mBrokerRef; }

//-----------------------------------------------------------------------------
void RayTracer::mergeImage(Core::Image *opImage, ImageCells& iCells)
{
    //Core::Timer _t;

    Geometry::Rectangle coverage = iCells.getCoverage();

    // reconstruct image from cells and merge into finalImage
    int cellIncrement = 1;
    for (int cellY = 0; cellY < iCells.getHeightInCells(); cellY += cellIncrement)
        for (int cellX = 0; cellX < iCells.getWidthInCells(); cellX += cellIncrement)
        {
            const Vector2i cellIndex(cellX, cellY);
            const Rectangle cellCoverage = iCells.getCellCoverage(cellIndex);

            if (cellCoverage.getWidth() >= 1)  // cell bigger than 1 px, undersampling
            {
                cellIncrement = fillPixels(opImage, iCells, cellIndex, cellCoverage);
            }
            else // more than 1 cell per pixels, supersampling
            {
                cellIncrement = fillPixel(opImage, iCells, cellIndex, cellCoverage);
            }
        }

    //printf("reconstructImage %f(s)\n", _t.elapsed());
}

//-----------------------------------------------------------------------------
// reply is handled in the MAIN thread
//
void RayTracer::processReply(Core::MessageQueue::Message* ipM)
{
    RaytraceReply *m = dynamic_cast<RaytraceReply*>(ipM);
    if(m)
    {
        // update final image
        //
        mergeImage(&mFinalImage, m->mRenderedCells);
        //printf("processReply id: %d, level: %d %f(s)\n", m->mId, mDesiredLevelOfDetail, _t.elapsed());
    }
}

//-----------------------------------------------------------------------------
void RayTracer::processMessage(MessageQueue::Message* ipM)
{
    RaytraceRequest *m = dynamic_cast<RaytraceRequest*>(ipM);
    if(m)
    {
        ImageCells cells;
        cells.setCoverage(m->mCoverage);
        cells.setSize(m->mTileSizeInPixel / mDesiredLevelOfDetail);

        render(&cells);

        // post the computed cells
        RaytraceReply *done = new RaytraceReply();
        done->mCoverage = m->mCoverage;
        done->mRenderedCells = cells;
        done->mId = m->mId;
        mReplyQueue.post(done);
        
    }
}

//-----------------------------------------------------------------------------
void RayTracer::rayCast(ImageCells* iCells,
    const Vector2i& iCell)
{
    Broker &b = getBroker();
    Camera &camera = b.getCamera();
    const Scene &scene = b.getScene();
    
    // make ray for given pixel
    const Rectangle& r = iCells->getCellCoverage(iCell);
    const Vector2 pixelPos = r.getCenter();
    
    const Vector3 camPos = camera.getPosition();
    Line ray(camPos,
             camera.pixelToWorld(pixelPos,
                               camPos + camera.getDirection()));
    
    Color color;
    double distanceToCamera;
    rayCast(0, ray, scene, camera, &color, &distanceToCamera);
    
    //clamp color
    color.set( min(color.getRed(), 1.0),
              min(color.getGreen(), 1.0),
              min(color.getBlue(), 1.0),
              min(color.getAlpha(), 1.0) );
    
    iCells->setCellValue(iCell, color, distanceToCamera);
}

//-----------------------------------------------------------------------------
void RayTracer::rayCast( int iDepth,
                        const Line& iRay,
                    const Scene& iScene,
                    const Camera& iCamera,
                    Color *opColor,
                    double *opDistanceToCamera)
{
    //--- intersects with scene.
    IntersectionResult ir;
    VisibilityTester vt;
    
    double f = mIntegrator.computeLi(iRay, iScene, iCamera, &ir, &vt);
    
    if(vt.isOccluded() || f < 0.0)
    { f = 0.0; }
    
    const double specularFactor = ir.mpMaterial->getSpecularFactor();
    const double oneMinusSpec = 1.0 - specularFactor;
    
    Color ambient(0.0, 0.0, 0.0, 0.0);
    Color diffuse = ir.mpMaterial->getColor();
    diffuse.set( diffuse.getRed() * f * oneMinusSpec,
          diffuse.getGreen() * f * oneMinusSpec,
          diffuse.getBlue() * f * oneMinusSpec,
          diffuse.getAlpha() );
    
    *opColor += diffuse + ambient;

    if( iDepth < kMaxRecursionDepth &&
       ir.mpMaterial->getSpecularFactor() > 0.0)
    {
        // reflect the ray and call raycast again.
        Vector3 reflectedDirection = reflect(iRay.getDirection(), ir.mNormal);
        
        const Vector3 intersectionPoint = iRay.getOrigin() +
            ir.mD * iRay.getDirection();
        
        //
        Line reflectedRay;
        reflectedRay.setOrigin(intersectionPoint + 1e-5*reflectedDirection);
        reflectedRay.setDirection(reflectedDirection);
        
        rayCast(++iDepth, reflectedRay, iScene, iCamera, opColor, opDistanceToCamera);
    }
    
    *opDistanceToCamera = ir.mD;
}

//-----------------------------------------------------------------------------
void RayTracer::render(int iLevelOfDetail)
{
    //mMessageQueue.clear();
    mDesiredLevelOfDetail = iLevelOfDetail;

    Image &im = getBroker().getFinalImage();
    if(mFinalImage.getSizeInPixels() != im.getSizeInPixels())
    {
        mFinalImage.set(im.getWidth(), im.getHeight(), iifRgbaUint8);
    }

    const double vw = im.getSizeInPixels().x();
    const double vh = im.getSizeInPixels().y();
    int numberOfCellsX = (int)ceil(vw / (double)kTileSize);
    int numberOfCellsY = (int)ceil(vh / (double)kTileSize);

    // prepare all the rendering tiles
    //
    for (int j = 0; j < numberOfCellsY; ++j)
    {
        for (int i = 0; i < numberOfCellsX; ++i)
        {
            RaytraceRequest *m = new RaytraceRequest();

            Rectangle coverage(Vector2i(i * kTileSize, j * kTileSize),
                kTileSize, kTileSize);

            m->mCoverage = coverage;
            m->mTileSizeInPixel = kTileSize;
            m->mId = i + j*numberOfCellsX;
            // send request to compute.
            mMessageQueue.post(m);
        }
    }


    //----- DEBUG CODE' RENDER ONLY ONE TILE
    //const double vw = im.getSizeInPixels().x();
    //int numberOfCellsX = (int)ceil(vw / (double)kTileSize);
    //RaytraceRequest *m = new RaytraceRequest();

    //Rectangle coverage(Vector2i(3 * kTileSize, 2 * kTileSize),
    //    kTileSize, kTileSize);

    //m->mCoverage = coverage;
    //m->mTileSizeInPixel = kTileSize;
    //m->mId = 3 + 3*numberOfCellsX;
    //// send request to compute.
    //mMessageQueue.post(m);
}

//-----------------------------------------------------------------------------
void RayTracer::render(ImageCells* iCells)
{   
    const int w = iCells->getWidthInCells();
    const int h = iCells->getHeightInCells();
    
    //#pragma omp parallel for //not tested
    for(int y = 0; y < h; ++y)
    {
        // exit the rendering loop if there is a new message
        // in the queue...
        for(int x = 0; x < w; ++x)
        {
            const Vector2i cell(x, y);
            
            rayCast(iCells, cell);
        }
    }
}

//-----------------------------------------------------------------------------
//--- RayTracer::RenderMessage
//-----------------------------------------------------------------------------
RayTracer::RaytraceRequest::RaytraceRequest(void *ipSender) :
    Core::MessageQueue::Message(ipSender),
    mCoverage(),
    mTileSizeInPixel(1),
    mId(0)
{}

//-----------------------------------------------------------------------------
RayTracer::RaytraceReply::RaytraceReply(void *ipSender) :
    Core::MessageQueue::Message(ipSender),
    mCoverage(),
    mRenderedCells(),
    mId(0)
{}

