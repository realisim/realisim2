
#include <cmath>
#include "DataStructure/IntersectionResult.h"
#include "DataStructure/Scene/Interfaces.h"
#include "DataStructure/Scene/LightNode.h"
#include "DataStructure/Scene/Scene.h"
#include "Geometry/Line.h"
#include "Math/Vector.h"
#include <memory>
#include "Rendering/Camera.h"
#include "SimpleIntegrator.h"
#include <vector>
#include "VisibilityTester.h"

using namespace Realisim;
    using namespace Geometry;
    using namespace LightBeam;
    using namespace Math;
using namespace std;

//------------------------------------------------------------------
SimpleIntegrator::SimpleIntegrator()
{}

//------------------------------------------------------------------
SimpleIntegrator::~SimpleIntegrator()
{}

//------------------------------------------------------------------
double SimpleIntegrator::computeLi(const Line &iLine,
    const Scene &iScene,
    const Rendering::Camera &iCamera,
    IntersectionResult *opResult,
    VisibilityTester *opVisibilityTester)
{
    double spectrum = 0.0;
    
    // collect all intersections from ray with scene
    IntersectionResult ir;
    vector<IntersectionResult> results;
    
    const vector<shared_ptr<IRenderable>>& vr = iScene.getRenderables();
    for( auto renderable : vr )
    {
        // ir.mD >= 0 for intersection in from of the ray origin...
        if(renderable->intersect(iLine, &ir) && ir.mD >= 0 )
        { results.push_back(ir); }
    }
    
    // compute light contribution
    if(!results.empty())
    {
        // sort and keep closest to camera result.
        // this means, smallest mD
        std::sort(results.begin(), results.end(),
                  [](IntersectionResult &a, IntersectionResult &b) {
                      return a.mD < b.mD;
                  });
        
        ir = results[0];
        results.clear();
        
        const Vector3 intersectionInWorldSpace = iLine.getOrigin() +
          ir.mD*iLine.getDirection();
        ir.mW0 = iCamera.getPosition() - intersectionInWorldSpace;
        ir.mW0.normalize();
        
        const vector<shared_ptr<ILightNode>>& lights = iScene.getLights();
        
        // comput lights contribution
        for(auto lightNode : lights)
        {
            const Light &light = lightNode->getLight();
            
            const double nDotL = ir.mNormal * light.getDirection();
            spectrum += fabs(nDotL);
            
            //fill the results.
            if(opResult)
            {
                (*opResult) = ir;
            }
            
            // fill the visibility tester if necessary
            if(opVisibilityTester)
            {
                // using the intersection point as starting point for the visiblily
                // tester is not a good idea, since that point lies on the surface
                // and will be the first point of intersection for the shadowray.
                // for that reason, we add a little offset to the intersection
                // point.
                //
                // Furthermore, since this hardcoded light is directional, the
                // light position is equal to the point of intersection + offset in
                // the light direction.
                //
                Vector3 wi = light.getDirection();
                wi.normalize();
                const Vector3 lightPos = intersectionInWorldSpace + 1e8 * wi;
                
                const Vector3 p = intersectionInWorldSpace + (1e-5 * wi);
                opVisibilityTester->set(p, lightPos, &iScene);
            }
        }
        
    }
    
    
    return spectrum;
}