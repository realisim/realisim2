
#pragma once

#include "Math/Vector.h"

namespace Realisim
{
    namespace Rendering
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
			double ratio() const;
            void set(int, int);
            Math::Vector2 size() const;
			int width() const;
            
        protected:
            Orientation mOrientation;
            int mWidth;
            int mHeight;
        };
    }
}
