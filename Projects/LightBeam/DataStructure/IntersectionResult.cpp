#include <cassert>
#include "IntersectionResult.h"

using namespace Realisim;
    using namespace LightBeam;
using namespace std;

//-----------------------------------------------------------------------------
IntersectionResult::IntersectionResult() :
    mpMaterialNode(make_shared<MaterialNode>())
{}