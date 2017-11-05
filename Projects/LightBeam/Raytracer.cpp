
#include "Broker.h"
#include <cassert>
#include "Core/Timer.h"
#include "Geometry/Intersections.h"
#include "IntersectionResult.h"
#include "Math/Vector.h"
#include "Math/VectorI.h"
#include "RayTracer.h"
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
    const int kMultisamplingFactor = 4;
}

//-----------------------------------------------------------------------------
RayTracer::RayTracer(Broker *ipBroker) :
    mBrokerRef(*ipBroker)
{
    using placeholders::_1;
    function<void(MessageQueue::Message*)> f =
        bind(&RayTracer::processMessage, this, _1);
    mMessageQueue.setProcessingFunction(f);
    mMessageQueue.setMaximumSize(1);
    mMessageQueue.startInThread();
    
    f = bind(&RayTracer::processReply, this, _1);
    mReplyQueue.setProcessingFunction(f);
    mReplyQueue.setMaximumSize(1);
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
    const int increment = round(1.0 / iCellCoverage.getWidth());
    
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
bool RayTracer::hasNewFrameAvailable() const
{
    bool r = !mReplyQueue.isEmpty();
    
    //the queue as only one element by construction...
    //
    mReplyQueue.processNextMessage();
    return r;
}

//-----------------------------------------------------------------------------
Broker& RayTracer::getBroker()
{ return mBrokerRef; }

//-----------------------------------------------------------------------------
void RayTracer::processReply(Core::MessageQueue::Message* ipM)
{
    Reply *m = dynamic_cast<Reply*>(ipM);
    if(m)
    {
        Broker &b = getBroker();
        Core::Image &finalIm = b.getFinalImage();
        finalIm = m->mImage;
    }
}

//-----------------------------------------------------------------------------
void RayTracer::processMessage(MessageQueue::Message* ipM)
{
Core::Timer _t;
    Message *m = dynamic_cast<Message*>(ipM);
    if(m)
    {
        const double vw = m->mSize.x();
        const double vh = m->mSize.y();
        int numberOfCells = (int)(vw * vh / (double)m->mDivideBy);
    
        ImageCells cells;
        Rectangle coverage(Vector2i(0, 0),
                           vw, vh);
        const int cellHeight = (int)sqrt(numberOfCells*vh/vw);
        const int cellWidth = (int)numberOfCells/cellHeight;
        cells.setSize(Vector2i(cellWidth, cellHeight));
        cells.setCoverage(coverage);
        
        render(cells);
        Core::Image im = reconstructImage(cells);
        
        // post the computed cells
        Reply *done = new Reply();
        done->mImage = im;
        mReplyQueue.post(done);
        
        if(mMessageQueue.isEmpty() && m->mDivideBy > 1/(2*kMultisamplingFactor))
        {
            Message *m2 = new Message();
            m2->mSize = m->mSize;
            
            // divide by 4 each time to effectively
            // grow the image by 2 in each dimension
            //
            m2->mDivideBy = 0.25 * m->mDivideBy;
            mMessageQueue.post(m2);
        }
        
    }
//printf("processMessage %d %f(s)\n", m->mDivideBy, _t.elapsed());
}

//-----------------------------------------------------------------------------
void RayTracer::rayCast(ImageCells& iCells,
    const Vector2i& iCell)
{
    Broker &b = getBroker();
    Camera &camera = b.getCamera();
    const Scene &scene = b.getScene();
    
    // make ray for given pixel
    const Rectangle& r = iCells.getCellCoverage(iCell);
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
    
    iCells.setCellValue(iCell, color, distanceToCamera);
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
    
    if(vt.isOccluded())
    { f = 0.0; }
    
    const double specularFactor = ir.mpMaterial->getSpecularFactor();
    const double oneMinusSpec = 1.0 - specularFactor;
    
    Color diffuse = ir.mpMaterial->getColor();
    diffuse.set( diffuse.getRed() * f * oneMinusSpec,
          diffuse.getGreen() * f * oneMinusSpec,
          diffuse.getBlue() * f * oneMinusSpec,
          diffuse.getAlpha() );
    
    *opColor += diffuse;

    if( iDepth < kMaxRecursionDepth &&
       ir.mpMaterial->getSpecularFactor() > 0.0)
    {
        // reflect the ray and call raycast again.
        Vector3 reflectedDirection = reflect(iRay.getDirection(), ir.mNormal);
        
        const Vector3 intersectionPoint = iRay.getOrigin() +
            ir.mD * iRay.getDirection();
        
        //
        Line reflectedRay;
        reflectedRay.setOrigin(intersectionPoint + reflectedDirection);
        reflectedRay.setDirection(reflectedDirection);
        
        rayCast(++iDepth, reflectedRay, iScene, iCamera, opColor, opDistanceToCamera);
    }
    
    *opDistanceToCamera = ir.mD;
}

//-----------------------------------------------------------------------------
Core::Image RayTracer::reconstructImage(const ImageCells& iCells)
{
    Core::Timer _t;
    
    // init final image
    Core::Image im;
    Geometry::Rectangle coverage = iCells.getCoverage();
    im.set((int)coverage.getWidth(), (int)coverage.getHeight(), iifRgbaUint8);
    
    // reconstruct image from cells
    int cellIncrement = 1;
    for(int cellY = 0; cellY < iCells.getHeightInCells(); cellY += cellIncrement)
        for(int cellX = 0; cellX < iCells.getWidthInCells(); cellX += cellIncrement)
        {
            const Vector2i cellIndex(cellX, cellY);
            const Rectangle cellCoverage = iCells.getCellCoverage(cellIndex);
            
            if(cellCoverage.getWidth() >= 1) // more than 1 cell per pixels, supersampling
            { cellIncrement = fillPixels(&im, iCells, cellIndex, cellCoverage); }
            else
            { cellIncrement = fillPixel(&im, iCells, cellIndex, cellCoverage); }
        }
    
    //printf("reconstructImage %f(s)\n", _t.elapsed());
    return im;
}

//-----------------------------------------------------------------------------
// returns the reflected ray.
//
// iIncident and iNormal must be normalized
//
// http://paulbourke.net/geometry/reflected/
//
Vector3 RayTracer::reflect(const Math::Vector3& iIncident,
    const Math::Vector3 &iNormal)
{
    return (2.0*iNormal*(-iIncident * iNormal) + iIncident).normalize();
}


//-----------------------------------------------------------------------------
void RayTracer::render()
{
    Message *m = new Message();
    Image &im = getBroker().getFinalImage();
    
    m->mDivideBy = 256;
    m->mSize = im.getSizeInPixels();
    
    // send request to compute.
    mMessageQueue.post(m);
}

//-----------------------------------------------------------------------------
void RayTracer::render(ImageCells& iCells)
{   
    const int w = iCells.getWidthInCells();
    const int h = iCells.getHeightInCells();
    
//#pragma omp parallel for //not tested
    for(int y = 0; y < h; ++y)
    {
        for(int x = 0; x < w; ++x)
        {
            const Vector2i cell(x, y);
            
            rayCast(iCells, cell);
        }
        
        // exits the rendering if a message is in the queue...
        // using an atomic int would be better here... no
        // mutex acquiring needed...
        //
        if(!mMessageQueue.isEmpty())
        {return;}
    }
}

//-----------------------------------------------------------------------------
//--- RayTracer::RenderMessage
//-----------------------------------------------------------------------------
RayTracer::Message::Message(void *ipSender) :
    Core::MessageQueue::Message(ipSender),
    mSize(1, 1),
    mDivideBy(1)
{}

//-----------------------------------------------------------------------------
RayTracer::Reply::Reply(void *ipSender) :
    Core::MessageQueue::Message(ipSender),
    mImage()
{}

