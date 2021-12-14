
#include <cassert>
#include "DataStructures/Scene/ImageNode.h"
#include "Rendering/Gpu/Texture2d.h"
#include "Systems/Renderer/TextureRenderable.h"

using namespace Realisim;
    using namespace Reactor;
    using namespace Rendering;
using namespace std;

//---------------------------------------------------------------------------------------------------------------------
TextureRenderable::TextureRenderable(ImageNode* ipNode) : IRenderable(ipNode),
    mpNode(ipNode)
{
    assert(mpNode != nullptr);
}

//---------------------------------------------------------------------------------------------------------------------
TextureRenderable::~TextureRenderable()
{
    releaseGpuRessources();
}

//---------------------------------------------------------------------------------------------------------------------
void TextureRenderable::initializeGpuRessources()
{
    releaseGpuRessources();

    // all renderables have anisotropic filtering set to 8
    mTexture.setMaxAnisotropicFilteringValue(8.0);
    const Core::Image& image = mpNode->getImage();
    assert(image.isValid() && image.hasImageData());

    bool needsGammaCorrection = false; //unless a linear data such as normal or specular
    Rendering::TextureFormatDefinition tfd = Rendering::toTextureFormatDefinition(image.getInternalFormat(), needsGammaCorrection);

    mTexture.set(Rendering::tt2d,
        0,
        8,
        tfd.textureInternalFormat,
        image.getWidth(),
        image.getHeight(),
        tfd.textureFormat,
        tfd.type,
        (void*)image.getImageData().constData());

    // always generate mipmaps
    mTexture.generateMipmap();

    mTexture.setMagnificationFilter(Rendering::TextureFilter::tfLinearMipmapLinear);
    mTexture.setMinificationFilter(Rendering::TextureFilter::tfLinearMipmapLinear);
    mTexture.setWrapMode(mpNode->getTextureWrapMode());
}

//---------------------------------------------------------------------------------------------------------------------
void TextureRenderable::releaseGpuRessources()
{
    mTexture.clear();
}