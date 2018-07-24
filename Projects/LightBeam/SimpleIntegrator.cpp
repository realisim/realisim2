
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
#include "Utilities.h"

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
    double spectrum = 0.0; // mal nommé...
    
    // collect all intersections from ray with scene
    IntersectionResult ir;
    vector<IntersectionResult> results;
    
    const vector<shared_ptr<IRenderable>>& vr = iScene.getRenderables();
    for( auto renderable : vr )
    {
        // ir.mD >= 1e-5 for intersection in front of the ray origin...
        // also to prevent intersection with the origin, we compare
        // with 1e-5... somehow a cheap error progation work around.
        // 
        //
        if(renderable->intersect(iLine, &ir) && ir.mD >= 1e-5 )
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
            
            // compute the shading or diffuse
            const double nDotL = ir.mNormal * light.getDirection();
            spectrum += fabs(nDotL);
            
            // compute the specular factor
            Vector3 reflectRay = reflect(light.getDirection(), ir.mNormal);
            Vector3 viewVector = (iCamera.getPosition() - intersectionInWorldSpace).normalize();
            double specularFactor = pow(reflectRay * viewVector, 12.0);
            spectrum += specularFactor;
            
            //fill the results.
            if(opResult)
            {
                (*opResult) = ir;
            }
            
            // fill the visibility tester if necessary
            if(opVisibilityTester)
            {
                // using the intersection point as starting point for the
                // visibility tester.
                //
                // Furthermore, since this hardcoded light is directional, the
                // light position is equal to the point of intersection + offset in
                // the light direction.
                //
                Vector3 wi = light.getDirection();
                wi.normalize();
                const Vector3 lightPos = intersectionInWorldSpace + 1e8 * wi;
                
                const Vector3 p = intersectionInWorldSpace + wi;
                opVisibilityTester->set(p, lightPos, &iScene);
            }
        }
        
    }
    
    
    return spectrum;
}