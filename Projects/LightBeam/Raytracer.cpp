
#include "Broker.h"
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

//-----------------------------------------------------------------------------
RayTracer::RayTracer(Broker *ipBroker) :
    mBrokerRef(*ipBroker)
{}

//-----------------------------------------------------------------------------
RayTracer::~RayTracer()
{}

//-----------------------------------------------------------------------------
Broker& RayTracer::getBroker()
{ return mBrokerRef; }

//-----------------------------------------------------------------------------
void RayTracer::render()
{
    Broker &b = getBroker();
    Camera &cRef = b.getCamera();
    const Frustum frustum = cRef.getFrustum();
    const Viewport& viewport = cRef.getViewport();
    FrameBuffer &fRef = b.getFrameBuffer();
    Image &colorBuffer = fRef.getColorBuffer();
    
    const int w = viewport.getWidth();
    const int h = viewport.getHeight();
    for(int y = 0; y < h; ++y)
    {
        for(int x = 0; w; ++x)
        {
            const Vector2i pixel(x, y);

            // make ray for given pixel
            const Vector3 camPos = cRef.getPosition();
            Line ray(camPos,
               cRef.pixelToWorld(pixel,
               camPos + cRef.getDirection().normalize()));
            
            //--- default color
            Color c(0.0, 0.0, 0.0, 1.0);

            //--- intersects with scene.
            Plane plane(Vector3(0.0, -12, 0.0), Vector3(0.0, 1.0, 0.0));

            Vector3 p, n;
            IntersectionType pType = intersect(ray, plane, &p, &n);
            if(pType != itNone &&
               frustum.contains(p))
            {
                c.set(1.0, 0.0, 0.0, 1.0);
            }
            
            
            //--- set final pixel color...
            colorBuffer.setPixelColor(pixel, c);
        }
    }
}
