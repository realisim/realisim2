
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
        if(renderable->intersects(iLine) &&
            renderable->intersect(iLine, &ir) && ir.mD >= 1e-5 )
        {
            results.push_back(ir);
        }
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
// THIS CODE SHOULD GO IN Light::Li() method
//
//
        double perLightSpectrum = 0;
        for(auto lightNode : lights)
        {
            perLightSpectrum = 0.0;
            const Light &light = lightNode->getLight();
            
            Vector3 lightDirection; // wi
            Vector3 lightPosition;
            // compute the shading or diffuse
            switch (light.getType())
            {
            case Light::tPoint:
                lightDirection = light.getPosition() - intersectionInWorldSpace;
                lightPosition = light.getPosition();
                break;
            case Light::tSpot: break;
            case Light::tDirectionnal: 
                lightDirection = light.getDirection();
                lightPosition = intersectionInWorldSpace + 1e8*lightDirection; // very far in the light direction...
                break;
            case Light::tArea: break;
            default: break;
            }

            // light contribution
            const double lightDistance = (light.getPosition() - intersectionInWorldSpace).norm();
            double attenuation = 1.0;
            switch (light.getAttenuationType())
            {
            case Light::atNone: break;
            case Light::atQuadratic: attenuation = 1.0 / (lightDistance*lightDistance); break;
            case Light::atLinear: attenuation = 1.0 / (lightDistance); break;
            default: break;
            }

            // using the intersection point as starting point for the
            // visibility tester.
            //
            //
            const Vector3 p = intersectionInWorldSpace + 1e-5 * lightDirection;
            opVisibilityTester->set(p, lightPosition, &iScene);

            const double nDotL = ir.mNormal * lightDirection;
            perLightSpectrum += fabs(nDotL);
            
            // compute the specular factor
            Vector3 reflectRay = reflect(lightDirection, ir.mNormal);
            double specularFactor = pow(reflectRay * ir.mW0, 128.0);
            perLightSpectrum += specularFactor;
            
            const double power = 1.0;
            perLightSpectrum *= attenuation * power;

            spectrum += perLightSpectrum;
            if (opVisibilityTester->isOccluded())
            {
                spectrum -= perLightSpectrum;
            }

            //fill the results.
            if(opResult)
            {
                (*opResult) = ir;
            }
        }
        
    }
    
    
    return spectrum;
}