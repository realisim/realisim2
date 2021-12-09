
#pragma once

#include "3d/Scene/SceneNode.h"
#include "Core/Image.h"


namespace Realisim
{
namespace Reactor
{
    class ImageNode : public ThreeD::SceneNode {
    public:
        ImageNode();
        ImageNode(const ImageNode&) = delete;
        ImageNode& operator=(const ImageNode&) = delete;
        virtual ~ImageNode();

        void clear();

        const Core::Image& getImage() const;
        Core::Image& getImageRef();

    protected:
        Core::Image mImage;
    };

}
}

