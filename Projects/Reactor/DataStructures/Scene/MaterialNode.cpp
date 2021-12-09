
#include <cassert>
#include "DataStructures/Scene/MaterialNode.h"
#include "DataStructures/Scene/SceneNodeEnum.h"

using namespace Realisim;;
using namespace Reactor;
using namespace ThreeD;
using namespace std;

//---------------------------------------------------------------------------------------------------------------------
MaterialNode::MaterialNode() : SceneNode((int)SceneNodeEnum::sneMaterialNode)
{
    clear();
}

//---------------------------------------------------------------------------------------------------------------------
MaterialNode::~MaterialNode()
{
    clear();
}

//---------------------------------------------------------------------------------------------------------------------
void MaterialNode::addImageNode(ImageNode* ipNode, ThreeD::Material::ImageLayer iLayer)
{
    assert(mImageNodes[iLayer] == nullptr);
    mImageNodes[iLayer] = ipNode;
}

//---------------------------------------------------------------------------------------------------------------------
void MaterialNode::clear()
{
    for (int i = 0; i < (int)Material::ilNumberOfLayers; ++i) {
        mImageNodes[i] = nullptr;
    }
}

//---------------------------------------------------------------------------------------------------------------------
ImageNode* MaterialNode::getImageNode(ThreeD::Material::ImageLayer iLayer)
{
    return mImageNodes[(int)iLayer];
}

//---------------------------------------------------------------------------------------------------------------------
void MaterialNode::removeImageNode(ThreeD::Material::ImageLayer iLayer)
{
    mImageNodes[iLayer] = nullptr;
}