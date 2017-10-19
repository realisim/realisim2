
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
    vector<IntersectionResult> vIr;
    
    Frustum frustum = iCamera.getFrustum();
    const vector<shared_ptr<IRenderable>>& vr = iScene.getRenderables();
    for( auto r : vr )
    {
        if(r->intersect(iLine, &ir) && frustum.contains(ir.mPointInWorldSpace))
        { vIr.push_back(ir); }
    }
    
    
    // sort and keep closest to camera result.
    int closestIndex = -1;
    double distance = std::numeric_limits<double>::max();
    
    int i = 0;
    for(auto irIt : vIr)
    {
        const Vector3 cp = iCamera.getPosition() - irIt.mPointInWorldSpace;
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
        ir = vIr[closestIndex];
        ir.mW0 = iCamera.getPosition() - ir.mPointInWorldSpace;
        ir.mW0.normalize();
        vIr.clear();
        
        // comput lights contribution
        Vector3 lightDir(1.0, 1.0, 1.0);
        lightDir.normalize();
        
        const double nDotL = ir.mNormal * lightDir;
        // should we return fabs(nDotL)???
        spectrum += nDotL > 0.0 ? nDotL : 0.0;
     
        //fill the results.
        if(opResult)
        {
            (*opResult) = ir;
        }
        
        // fill the visibility tester if necessary
        if(opVisibilityTester)
        {
            opVisibilityTester->set(ir.mPointInWorldSpace, Vector3(1e8), &iScene);
        }
    }
    
    
    return spectrum;
}