
#pragma once

#include "Math/Vector.h"

namespace Realisim
{
    namespace TreeD
    {
        class Viewport
        {
        public:
            Viewport();
            Viewport(int iWidth, int iHeight);
            Viewport(const Viewport&) = default;
            Viewport& operator=(const Viewport&) = default;
            ~Viewport() = default;
            
            enum Orientation { oHorizontal, oVertical };
            
            Orientation orientation() const;
            int height() const;
            int width() const;
            void set(int, int);
            Math::Vector2 size() const;
            
        protected:
            Orientation mOrientation;
            int mWidth;
            int mHeight;
        };
    }
}
