
#include "DataStructure/IntersectionResult.h"
#include "DataStructure/Scene.h"
#include "Geometry/Line.h"
#include "VisibilityTester.h"

using namespace Realisim;
    using namespace LightBeam;
    using namespace Math;
using namespace std;

//-----------------------------------------------------------------------------
VisibilityTester::VisibilityTester() : mpScene(nullptr)
{}

//-----------------------------------------------------------------------------
void VisibilityTester::set(const Math::Vector3& iPoint,
    const Math::Vector3& iLightPos,
    const Scene *ipScene)
{
    mpScene = ipScene;
    mShadedPoint = iPoint;
    mLightPosition = iLightPos;
}

//-----------------------------------------------------------------------------
bool VisibilityTester::isOccluded()
{
    bool r = false;
    
    // make a ray from the intersection point ( the point to be shaded ) with
    // the light position.
    // We will cast that ray(shadowray) in the scene. If an intersection exists
    // between the shadedPoint and the light, then the shaded point is occluded.
    //
    Geometry::Line shadowRay(mShadedPoint, mLightPosition);
    
    if(mpScene)
    {
        IntersectionResult ir;
        const vector<shared_ptr<IRenderable>>& vr = mpScene->getRenderables();
        for( size_t i = 0; i < vr.size() && !r; ++i )
        {
            if(vr[i]->intersect(shadowRay, &ir))
            {
                // check ir is between the light pos and the intersection point.
                // if so, then there is occlusion
                //
                
                r = ir.mD > 0 &&
                    ir.mD < (mLightPosition - mShadedPoint).norm();
            }
        }
    }
    
    return r;
}