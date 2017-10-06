
#include "Broker.h"
#include "Math/Vector.h"
#include "Math/VectorI.h"
#include "RayTracer.h"

using namespace Realisim;
    using namespace Core;
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
//    FrameBuffer &fRef = b.getFrameBuffer();
  
    Camera &cRef = b.getCamera();
    Viewport v = cRef.getViewport();
    
    for(int y = 0; y < v.getHeight(); ++y)
    {
        for(int x = 0; x < v.getWidth(); ++x)
        {
            
        }
    }
    
//    Image &colorBuffer = fRef.getColorBuffer();
    
//    Color c;
//    const int w = colorBuffer.getWidth();
//    const int h = colorBuffer.getHeight();
//    for(int y = 0; y < h; ++y)
//        for(int x = 0; x < w; ++x)
//        {
//            c.set(x / (double)w, y / double(h), 0.0, 1.0);
//            colorBuffer.setPixelColor(Vector2i(x, y), c);
//        }
}
