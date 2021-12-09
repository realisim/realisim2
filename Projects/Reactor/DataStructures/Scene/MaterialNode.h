
#pragma once

#include "3d/Scene/SceneNode.h"
#include "3d/Material.h"
#include "DataStructures/Scene/ImageNode.h"

namespace Realisim
{
    namespace Reactor
    {
        class MaterialNode : public ThreeD::SceneNode {
        public:
            MaterialNode();
            MaterialNode(const MaterialNode&) = delete;
            MaterialNode& operator=(const MaterialNode&) = delete;
            virtual ~MaterialNode();

            void addImageNode(ImageNode*, ThreeD::Material::ImageLayer iLayer);
            void clear();
            const ThreeD::Material& getMaterial() const { return mMaterial; }
            ThreeD::Material& getMaterialRef() { return mMaterial; }
            ImageNode* getImageNode(ThreeD::Material::ImageLayer iLayer);
            void removeImageNode(ThreeD::Material::ImageLayer iLayer);

        protected:
            ThreeD::Material mMaterial;
            std::array<ImageNode*, ThreeD::Material::ilNumberOfLayers> mImageNodes; //not owned
        };
    }
}

