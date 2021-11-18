
#include "Math/Matrix.h"
#include "Rendering/Gpu/BufferObject.h"
#include "Rendering/Gpu/VertexArrayObject.h"
#include <vector>

namespace Realisim
{
    class Painter
    {
    public:
        Painter();
        ~Painter();
        Painter(const Painter&) = delete;
        Painter& operator=(const Painter&) = delete;

        void drawAxis();
        void drawCube();

    protected:
        void createAxis();
        void createCube();

        Rendering::VertexArrayObject mAxis;
        Rendering::VertexArrayObject mCube;
    };

}