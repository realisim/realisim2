#pragma once

#include <string>
#include <vector>

#include "Math/Vector.h"

namespace Realisim
{
    namespace Rendering
    {

        std::string getAndClearLastOpenGlErrors();
        void waitOnGlFenceSync(void* sync);

        std::vector<float>   toFloatArray(const std::vector<Math::Vector2>& in);
        std::vector<float>   toFloatArray(const std::vector<Math::Vector3>& in);
        std::vector<float>   toFloatArray(const std::vector<Math::Vector4>& in);

    }
}