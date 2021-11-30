
#include <cassert>
#include "3d/Scene/IPositionableNode.h"


using namespace Realisim;
using namespace Math;
using namespace ThreeD;
using namespace std;

//---------------------------------------------------------------------------------------------------------------------
IPositionableNode::IPositionableNode() :
    mIsTransformDirty(true)
{}

//---------------------------------------------------------------------------------------------------------------------
IPositionableNode::~IPositionableNode()
{}

//---------------------------------------------------------------------------------------------------------------------
void IPositionableNode::initializeModelSpaceAABB()
{}

//---------------------------------------------------------------------------------------------------------------------
void IPositionableNode::setParentTransform(const Math::Matrix4& iV)
{
    mParentTransform = iV;
    mIsTransformDirty = true;
}

//---------------------------------------------------------------------------------------------------------------------
void IPositionableNode::setWorldTransform(const Math::Matrix4& iV)
{
    (void)iV;
    // make the necessary math to set the parent transform
    assert(false);
}

//---------------------------------------------------------------------------------------------------------------------
void IPositionableNode::updateWorldSpaceAABB()
{
    mUpdatedWorldSpaceAABB.reset();

    vector<Vector3> corners = mOriginalModelSpaceAABB.getCorners();
    for (const auto& c : corners) {
        mUpdatedWorldSpaceAABB.addPoint( (mWorldTransform * Vector4(c, 1)).xyz() );
    }
}
