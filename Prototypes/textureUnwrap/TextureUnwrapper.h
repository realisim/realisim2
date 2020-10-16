
#include "Math/Vector.h"
#include <vector>

namespace Realisim
{

class TextureUnwrapper
{
public:
    TextureUnwrapper();
    ~TextureUnwrapper();
    TextureUnwrapper(const TextureUnwrapper&) = delete;
    TextureUnwrapper operator=(const TextureUnwrapper&) = delete;

protected:
    std::vector<Math::Vector3> mVertices;
};

}