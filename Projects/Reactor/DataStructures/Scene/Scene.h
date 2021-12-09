#pragma once

#include "3d/Scene/SceneNode.h"

namespace Realisim
{
namespace Reactor
{
    class Scene
    {
    public:
        Scene();
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;
        ~Scene();

        //void addImage(ImageNode* pImage);
        //void addMaterial(MaterialNode* pMat);
        void clear();
        const ThreeD::SceneNode& getRoot() const { return *mpRoot; }
        ThreeD::SceneNode& getRootRef() { return *mpRoot; }

        const ThreeD::SceneNode& getImageLibrary() const { return *mpImageLibrary; }
        ThreeD::SceneNode& getImageLibraryRef() { return *mpImageLibrary; }

        const ThreeD::SceneNode& getMaterialLibrary() const { return *mpMaterialLibrary; }
        ThreeD::SceneNode& getMaterialLibraryRef() { return *mpMaterialLibrary; }

    protected:
        void makeNewScene();

        ThreeD::SceneNode* mpRoot; // owned never null

        ThreeD::SceneNode* mpMaterialLibrary; //owned never null
        ThreeD::SceneNode* mpImageLibrary; //owned never null
    };

}
}

