
#include "Broker.h"
#include <cassert>
#include "Core/Timer.h"
#include "Core/Unused.h"
#include "Geometry/Intersections.h"
#include "DataStructure/IntersectionResult.h"
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
    using namespace ThreeD;
using namespace std;

namespace  {
    const int kMaxRecursionDepth = 5;
    const int kTileSize = 16;
    int kNumThreads = 1;
}

//-----------------------------------------------------------------------------
RayTracer::RayTracer(Broker *ipBroker) :
    mBrokerRef(*ipBroker)
{
    // set max number of threads
#ifdef NDEBUG
    kNumThreads = std::thread::hardware_concurrency();
#else
    kNumThreads = 1;
#endif // NDEBUG

    

    using placeholders::_1;
    function<void(MessageQueue::Message*)> f =
        bind(&RayTracer::processMessage, this, _1);
    mMessageQueue.setOneByOneProcessingFunction(f);
    mMessageQueue.setMaximumSize(-1);
    mMessageQueue.setNumberOfThreads(kNumThreads);
    mMessageQueue.setBehavior(MessageQueue::bFifo);
    mMessageQueue.startInThread();
    
    function<void( const std::vector<MessageQueue::Message*>& )> f2 =
        bind(&RayTracer::processReplies, this, _1);
    mReplyQueue.setAllAtOnceProcessingFunction(f2);
    mReplyQueue.setBehavior(MessageQueue::bFifo);
    mReplyQueue.setMaximumSize(-1);
}

//-----------------------------------------------------------------------------
RayTracer::~RayTracer()
{
    mMessageQueue.stopThread();
}

//-----------------------------------------------------------------------------
void RayTracer::debugRayCast(const Vector2i &iPixelPosition)
{
    Broker &b = getBroker();
    const Scene &scene = b.getScene();
    const Camera &camera = scene.getCamera();
    

    // make ray for given pixel
    const Vector2 pixelPos = Vector2(iPixelPosition) + Vector2(0.5);

    const Vector3 camPos = camera.getPosition();
    Line ray(camPos,
        camera.pixelToWorld(pixelPos,
            camPos + camera.getDirection()));

    Color color;
    double distanceToCamera;
    rayCast(0, ray, scene, camera, &color, &distanceToCamera);

    printf("--- Debug Ray ---\n"
        "Pixel pos: %.1f, %.1f\n"
        "Color: %.2f, %.2f, %.2f, %.2f\n"
        "Distance to camera (m): %f\n",
        pixelPos.x(), pixelPos.y(),
        color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha(),
        distanceToCamera);
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
    //int numMessages = mReplyQueue.getNumberOfMessages();
    //Timer _t;
    mReplyQueue.processMessagesAllAtOnce();
    //double e = _t.elapsed();
    //printf("number of replies: %d, handled in %.4f sec.\n", numMessages, (float)e );
    
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
void RayTracer::processReplies( const vector<Core::MessageQueue::Message*>& ipMessages)
{
    RaytraceReply *m = nullptr;
    const int numMessages = (int)ipMessages.size();
    for (int i = 0; i < numMessages; ++i)
    {
        m = dynamic_cast<RaytraceReply*>(ipMessages[i]);
        if (m)
        {
            // update final image
            //
            mergeImage(&mFinalImage, m->mRenderedCells);
        }
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
        cells.setSize(m->mTileSizeInPixel);

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

    const Scene &scene = b.getScene();
    const Camera &camera = scene.getCamera();
    
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
    
    Color li = mIntegrator.computeLi(iRay, iScene, iCamera, &ir, &vt);
    li.setAlpha(1.0);

    *opColor += li;

    // reflect ray if material is specular
    if( iDepth < kMaxRecursionDepth &&
        ir.mpMaterialNode->getMaterial().hasReflectanceModel(Material::rmPerfectSpecular) )
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
void RayTracer::render(int iLevelOfDetail, bool iClearIterativeRendering)
{

    printf("lod: %d\n", iLevelOfDetail);

    if(iClearIterativeRendering)
        mMessageQueue.clear();

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
            m->mTileSizeInPixel = kTileSize / mDesiredLevelOfDetail;
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
    
    for(int y = 0; y < h; ++y)
    {
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

