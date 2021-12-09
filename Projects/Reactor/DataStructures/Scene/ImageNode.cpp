
#include "DataStructures/Scene/ImageNode.h"
#include "DataStructures/Scene/SceneNodeEnum.h"

using namespace Realisim;;
using namespace Reactor;
using namespace std;

//---------------------------------------------------------------------------------------------------------------------
ImageNode::ImageNode() : SceneNode((int)SceneNodeEnum::sneImageNode)
{}

//---------------------------------------------------------------------------------------------------------------------
ImageNode::~ImageNode()
{
    clear();
}

//---------------------------------------------------------------------------------------------------------------------
void ImageNode::clear()
{
    mImage.clear();
}

//---------------------------------------------------------------------------------------------------------------------
const Core::Image& ImageNode::getImage() const {
    return mImage;
}

//---------------------------------------------------------------------------------------------------------------------
Core::Image& ImageNode::getImageRef() {
    return mImage;
}