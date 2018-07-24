
#include "utilities.h"


namespace Realisim
{
namespace LightBeam
{
    using namespace Math;
    //-----------------------------------------------------------------------------
    // returns the reflected ray.
    //
    // iIncident and iNormal must be normalized
    //
    // http://paulbourke.net/geometry/reflected/
    //
    Vector3 reflect(const Vector3& iIncident, const Vector3 &iNormal)
    {
        return (2.0*iNormal*(-iIncident * iNormal) + iIncident).normalize();
    }

}
}