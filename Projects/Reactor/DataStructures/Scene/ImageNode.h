
#pragma once

#include "3d/Scene/SceneNode.h"
#include "Core/Image.h"
#include "Rendering/Gpu/DataType.h"

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
        Rendering::TextureWrapMode getTextureWrapMode() const { return mWrapMode; }
        void setFilenamePath(const std::string& iF);
        void setTextureWrapMode(Rendering::TextureWrapMode iV) { mWrapMode = iV; }

    protected:
        Core::Image mImage;
        Rendering::TextureWrapMode mWrapMode;
    };

}
}

