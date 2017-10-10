
#include "Broker.h"
#include <deque>
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
//    using namespace placeholders;
//    mRenderQueue.setProcessingFunction()
}

//-----------------------------------------------------------------------------
RayTracer::~RayTracer()
{}

//-----------------------------------------------------------------------------
Broker& RayTracer::getBroker()
{ return mBrokerRef; }

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
    Camera &cRef = b.getCamera();
    const Viewport& viewport = cRef.getViewport();
    
    RenderStack& rs = b.getRenderStack();

// dummy stuff avant de mettre le thread...
rs.mStack.clear();
rs.mStack.push_back(ImageCells());
rs.mStack.push_back(ImageCells());
    
    int nextStackIndex = rs.mStack.size();
    uint64_t numberOfCells = pow(2, nextStackIndex);
    
    ImageCells cells;
    Rectangle coverage(Vector2i(0, 0),
                       viewport.getWidth(), viewport.getHeight());
    cells.setSize(Vector2i(numberOfCells, numberOfCells));
    cells.setCoverage(coverage);
    
    render(cells);
    rs.mStack.push_back(cells);
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
    }
}
