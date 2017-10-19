#include <cassert>
#include "IntersectionResult.h"

using namespace Realisim;
    using namespace LightBeam;
using namespace std;

//-----------------------------------------------------------------------------
IntersectionResult::IntersectionResult() :
    mpMaterial(make_shared<Material>())
{}