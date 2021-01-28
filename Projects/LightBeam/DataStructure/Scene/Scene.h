#pragma once


#include "Core/Image.h"
#include "DataStructure/Scene/IRenderable.h"
#include "DataStructure/Scene/MaterialNode.h"
#include "DataStructure/Scene/Interfaces.h"
#include "Rendering/Camera.h"

#include <map>
#include <memory>

namespace Realisim
{
namespace LightBeam
{
    class Scene
    {
    public:
        Scene();
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;
        ~Scene();

        void clear();
void createDummyScene();
        const Rendering::Camera& getCamera() const;
        Rendering::Camera& getCameraRef();
void importObj(const std::string &iFilenamePath);
        void setCamera(const Rendering::Camera &iCamera);
        void addNode(std::shared_ptr<ISceneNode>);
        const std::vector< std::shared_ptr<ILightNode> >& getLights() const;
        const std::vector< std::shared_ptr<IRenderable> >& getRenderables() const;

    protected:
        Rendering::Camera mCamera;
        std::vector< std::shared_ptr<ISceneNode> > mNodes;
        std::vector< std::shared_ptr<IRenderable> > mRenderables;
        std::vector< std::shared_ptr<ILightNode> > mLights;
        std::vector< std::shared_ptr<MaterialNode> > mMaterials;

        std::map<std::string, Core::Image> mKeyToImage; //the key is the full canninical path
    };

}
}