
#include "Broker.h"
#include <cassert>
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
        bind(&RayTracer::processRenderMessage, this, _1);
    mRenderQueue.setProcessingFunction(f);
    mRenderQueue.startInThread();
}

//-----------------------------------------------------------------------------
RayTracer::~RayTracer()
{}

//-----------------------------------------------------------------------------
bool RayTracer::hasNewFrameAvailable() const
{
    bool r = !mDoneQueue.isEmpty();
    mDoneQueue.clear();
    return r;
}

//-----------------------------------------------------------------------------
Broker& RayTracer::getBroker()
{ return mBrokerRef; }

//-----------------------------------------------------------------------------
void RayTracer::processRenderMessage(MessageQueue::Message* ipM)
{
    Message *m = dynamic_cast<Message*>(ipM);
    if(m)
    {
        switch (m->mAction)
        {
            case Message::aRender:
            {
                Broker &b = getBroker();
                Camera &cRef = b.getCamera();
                const Viewport& viewport = cRef.getViewport();
                RenderStack& rs = b.getRenderStack();
            
                size_t nextStackIndex = rs.mStack.size();
                int numberOfCells = (int)pow(2, nextStackIndex);
            
                ImageCells cells;
                Rectangle coverage(Vector2i(0, 0),
                                   viewport.getWidth(), viewport.getHeight());
                cells.setSize(Vector2i(numberOfCells, numberOfCells));
                cells.setCoverage(coverage);
                
                render(cells);
                rs.mStack.push_back(cells);
                
                mDoneQueue.post(makeMessage(Message::aFrameDone));
                
                if(rs.mStack.size() < 10)
                {
                    mRenderQueue.post(makeMessage(Message::aRender));
                }
            }
            break;
            case Message::aStopRendering: break;
            default: assert(0); break;
        }
    }
}

//-----------------------------------------------------------------------------
RayTracer::Message* RayTracer::makeMessage(Message::Action iAction)
{
    Message *m = new Message(this);
    m->mAction = iAction;
    return m;
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
    
    Vector3 p, n;
    IntersectionType pType = intersect(ray, plane, &p, &n);
    if(pType != itNone &&
       iFrustum.contains(p))
    {
        c.set(1.0, 0.0, 0.0, 1.0);
    }
    
    iCells.setCellValue(iCell, c, (p - camPos).norm());
}

//-----------------------------------------------------------------------------
void RayTracer::render()
{
    Broker &b = getBroker();
    RenderStack& rs = b.getRenderStack();

    // send stop request
    mRenderQueue.post(makeMessage(Message::aStopRendering));
    
    // wait for thread to finish
    mRenderQueue.stopThread();
    
    // clear the render stack
    rs.mStack.clear();
    rs.mStack.reserve(20);
    
    // start thread
    mRenderQueue.startInThread();
    
    // send request to compute.
    mRenderQueue.post(makeMessage(Message::aRender));
}

//-----------------------------------------------------------------------------
void RayTracer::render(ImageCells& iCells)
{
    Broker &b = getBroker();
    Camera &cRef = b.getCamera();
    const Frustum frustum = cRef.getFrustum();
    
    const int w = iCells.getWidthInCells();
    const int h = iCells.getHeightInCells();
    for(int y = 0; y < h; ++y)
    {
        for(int x = 0; x < w; ++x)
        {
            const Vector2i cell(x, y);
            
            rayCast(iCells, cell, frustum);
        }
        
        // stop the render if something is in the
        // queue... it should be empty, so it is
        // probably a request to stop anyway...
        //
        if(!mRenderQueue.isEmpty()){return;}
    }
}

//-----------------------------------------------------------------------------
//--- RayTracer::RenderMessage
//-----------------------------------------------------------------------------
RayTracer::Message::Message(void *ipSender) :
    Core::MessageQueue::Message(ipSender),
    mAction(Message::aNone)
{}

