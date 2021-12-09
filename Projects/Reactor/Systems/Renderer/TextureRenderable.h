#pragma once

#include "Rendering/Gpu/Texture2d.h"
#include "Systems/Renderer/IRenderable.h"

namespace Realisim
{
namespace Reactor {

    class ImageNode;

    class TextureRenderable : public IRenderable
    {
    public:
        TextureRenderable() = delete;
        explicit TextureRenderable(ImageNode* ipNode);
        TextureRenderable(const TextureRenderable&) = delete;
        TextureRenderable& operator=(const TextureRenderable&) = delete;
        virtual ~TextureRenderable();

        virtual void initializeGpuRessources() override;
        virtual void draw() override { ; } // a enlever... faire un IDrawable
        const Rendering::Texture2d& getTexture() const { return mTexture; }
        Rendering::Texture2d& getTextureRef() { return mTexture; }
        ImageNode* getImageNode() { return mpNode; }
        virtual void releaseGpuRessources() override;

    protected:
        ImageNode* mpNode; //not owned, shall never be null

        Rendering::Texture2d mTexture;
    };

}
}

