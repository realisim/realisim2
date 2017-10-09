
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
    const Viewport& viewport = cRef.getViewport();
    
    ImageCells& cells = b.getImageCells();
    cells.setCoverage(0, 0, viewport.getWidth(), viewport.getHeight());
    
    render(cells.getRoot());
}

//-----------------------------------------------------------------------------
void RayTracer::rayCast(ImageCells::Node* ipNode)
{
    Broker &b = getBroker();
    Camera &cRef = b.getCamera();
    const Frustum frustum = cRef.getFrustum();
    
    // make ray for given pixel
    const Rectangle& r = ipNode->getCoverage();
    Vector2 pixelPos = r.getCenter();
    
    const Vector3 camPos = cRef.getPosition();
    Line ray(camPos,
        cRef.pixelToWorld(pixelPos,
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
    
    ipNode->setColor(c);
    ipNode->setDepth( (p - camPos).norm() );
}

//-----------------------------------------------------------------------------
void RayTracer::render(ImageCells::Node* ipNode)
{
    // breadth first approach
    deque<ImageCells::Node*> q;
    q.push_back(ipNode);
    
    int depth = 0;
    while(!q.empty())
    {
        // grab and remove the front node.
        ImageCells::Node *n = q.front();
        q.pop_front();
        
        rayCast(n);
    
        caca de caca.
        
        /*Le quad tree c'Est mauvais...
         On serait mieux avec un frameBuffer avec un coverage
         de la taille du viewport. Et un nombre de pixel 
         corresoind au nombre de cellule pour ce niveau de raster
         Ainsi on minimize l'Espace mémoire de calcul et on
         facilite le rende...
        
        */
        // add another level
        if(depth <= 3)
        {
            //split and add childs.
            n->split();
            for(size_t i = 0; i < n->mChilds.size(); ++i)
            {
                q.push_back(n->mChilds[i]);
            }
            depth++;
        }
    }
    
    
}
