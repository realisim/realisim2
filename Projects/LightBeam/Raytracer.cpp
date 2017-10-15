
#include "Broker.h"
#include <cassert>
#include "Core/Timer.h"
#include "Geometry/Intersections.h"
#include "Math/Vector.h"
#include "Math/VectorI.h"
#include "RayTracer.h"

using namespace Realisim;
    using namespace Core;
    using namespace Geometry;
    using namespace LightBeam;
    using namespace Math;
    using namespace Rendering;
using namespace std;

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
bool RayTracer::hasNewFrameAvailable() const
{
    bool r = !mReplyQueue.isEmpty();
    
    //the queue as only one element by construction...
    //
    mReplyQueue.processNextMessage();
    mReplyQueue.clear();
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
        int numberOfCells = vw * vh / (double)m->mDivideBy;
    
        ImageCells cells;
        Rectangle coverage(Vector2i(0, 0),
                           vw, vh);
        const double cellHeight = sqrt(numberOfCells*vh/vw);
        const double cellWidth = numberOfCells/cellHeight;
        cells.setSize(Vector2i(cellWidth, cellHeight));
        cells.setCoverage(coverage);
        
        render(cells);
        Core::Image im = reconstructImage(cells);
        
        // post the computed cells
        Reply *done = new Reply();
        done->mImage = im;
        mReplyQueue.post(done);
        
        if(mMessageQueue.isEmpty() && m->mDivideBy > 1)
        {
            Message *m2 = new Message();
            m2->mSize = m->mSize;
            m2->mDivideBy = m->mDivideBy / 2;
            mMessageQueue.post(m2);
        }
        
    }
//printf("processMessage %d %f(s)\n", m->mDivideBy, _t.elapsed());
}

//-----------------------------------------------------------------------------
void RayTracer::rayCast(ImageCells& iCells,
    const Vector2i& iCell,
    const Frustum& iFrustum)
{
    Broker &b = getBroker();
    Camera &cRef = b.getCamera();
    
    // make ray for given pixel
    const Rectangle& r = iCells.getCellCoverage(iCell);
    const Vector2 pixelPos = r.getCenter();
    
    const Vector3 camPos = cRef.getPosition();
    Line ray(camPos,
             cRef.pixelToWorld(pixelPos,
                               camPos + cRef.getDirection().normalize()));
    
    Vector3 LightDir(1.0, 1.0, 1.0);
    LightDir.normalize();
    
    //--- default color
    Color c(0.0, 0.0, 0.0, 1.0);
    
    //--- intersects with scene.
    Plane plane(Vector3(0.0, -12, 0.0), Vector3(1.0, 1.0, 0.0));
    Sphere sphere(Vector3(6.0, 0.0, 0.0), 16);
    Sphere sphere1(Vector3(15.0, 8.0, -30.0), 22);
    
    Vector3 p, n;
    IntersectionType pType = intersect(ray, plane, &p, &n);
    if(pType != itNone &&
       iFrustum.contains(p))
    {
        double v = 0.2;
        const double nDotL = n*LightDir;
        if(nDotL > 0) { v += 1.0 * nDotL; }
        v = min(max(0.0, v), 1.0);
        c.set(v, 0.0, 0.0, 1.0);
    }
    
    //sphere 0
    {
        vector<Vector3> points, normals;
        pType = intersect(ray, sphere, &points, &normals);
        p = points[0];
        n = normals[0];
        if(pType != itNone &&
           iFrustum.contains(p))
        {
            double v = 0.2;
            const double nDotL = n*LightDir;
            if(nDotL > 0) { v += 1.0 * nDotL; }
            v = min(max(0.0, v), 1.0);
            c.set(0, v, 0.0, 1.0);
        }
    }
    
    iCells.setCellValue(iCell, c, (p - camPos).norm());
}

//-----------------------------------------------------------------------------
Core::Image RayTracer::reconstructImage(const ImageCells& iCells)
{
    Core::Timer _t;
    
    // init final image
    Core::Image im;
    Geometry::Rectangle coverage = iCells.getCoverage();
    im.set(coverage.getWidth(), coverage.getHeight(), iifRgbaUint8);
    
    // reconstruct image from cells
    for(int cellY = 0; cellY < iCells.getHeightInCells(); ++cellY)
        for(int cellX = 0; cellX < iCells.getWidthInCells(); ++cellX)
        {
            const Vector2i cellIndex(cellX, cellY);
            Rectangle r = iCells.getCellCoverage(cellIndex);
            const Vector2 bl = r.getBottomLeft();
            const Vector2 tr = r.getTopRight();
            
            Color c;
            for(int y = bl.y(); y < tr.y(); ++y)
                for(int x = bl.x(); x < tr.x(); ++x)
                {
                    c = iCells.getCellColor(cellIndex);
                    im.setPixelColor(Vector2i(x,y), c);
                }
        }
    
    //printf("reconstructImage %f(s)\n", _t.elapsed());
    return im;
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
    Broker &b = getBroker();
    Camera &cRef = b.getCamera();
    const Frustum frustum = cRef.getFrustum();
    
    const int w = iCells.getWidthInCells();
    const int h = iCells.getHeightInCells();
    
//#pragma omp parallel for //not tested
    for(int y = 0; y < h; ++y)
    {
        for(int x = 0; x < w; ++x)
        {
            const Vector2i cell(x, y);
            
            rayCast(iCells, cell, frustum);
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

