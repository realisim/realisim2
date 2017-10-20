
#include <cmath>
#include "DataStructure/Interfaces.h"
#include "DataStructure/IntersectionResult.h"
#include "DataStructure/Scene.h"
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
        if(renderable->intersect(iLine, &ir) && ir.mD >= 0 )
        { results.push_back(ir); }
    }
    
    // sort and keep closest to camera result.
    int closestIndex = -1;
    double distance = std::numeric_limits<double>::max();
    
    int i = 0;
    for(auto result : results)
    {
        const Vector3 cp = iCamera.getPosition() - result.mPointInWorldSpace;
        const double d = cp.norm();
        if( d < distance)
        {
            distance = d;
            closestIndex = i;
        }
        ++i;
    }
    
    // compute light contribution
    if(closestIndex != -1)
    {
        ir = results[closestIndex];
        ir.mW0 = iCamera.getPosition() - ir.mPointInWorldSpace;
        ir.mW0.normalize();
        results.clear();
        
        // comput lights contribution
        Vector3 lightDir(1.0, 1.0, 1.0);
        lightDir.normalize();
        
        const double nDotL = ir.mNormal * lightDir;
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
            Vector3 wi = lightDir;
            wi.normalize();
            const Vector3 lightPos = ir.mPointInWorldSpace + 100 * wi;
            
            const Vector3 p = ir.mPointInWorldSpace + (1e-5 * wi);
            opVisibilityTester->set(p, lightPos, &iScene);
        }
    }
    
    
    return spectrum;
}