
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
        ImageCells &ic = b.getImageCells();
        ic = m->mCells;
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
        
        // post the computed cells
        Reply *done = new Reply();
        done->mCells = cells;
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
    
    //--- default color
    Color c(0.0, 0.0, 0.0, 1.0);
    
    //--- intersects with scene.
    Plane plane(Vector3(0.0, -12, 0.0), Vector3(1.0, 1.0, 0.0));
    Sphere sphere(Vector3(6.0, 0.0, 0.0), 16);
    
    Vector3 p, n;
    IntersectionType pType = intersect(ray, plane, &p, &n);
    if(pType != itNone &&
       iFrustum.contains(p))
    {
        c.set(1.0, 0.0, 0.0, 1.0);
    }
    
    vector<Vector3> points, normals;
    pType = intersect(ray, sphere, &points, &normals);
    p = points[0];
    if(pType != itNone &&
       iFrustum.contains(p))
    {
        c.set(0.0, 1.0, 0.0, 1.0);
    }
    
    iCells.setCellValue(iCell, c, (p - camPos).norm());
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
    mCells()
{}

