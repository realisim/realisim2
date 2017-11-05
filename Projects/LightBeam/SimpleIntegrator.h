#pragma once


namespace Realisim
{
    namespace Geometry { class Line; }
    namespace LightBeam
    {
        class IntersectionResult;
        class Scene;
        class VisibilityTester;
    }
    namespace Rendering { class Camera; }
}

namespace Realisim
{
namespace LightBeam
{
    class SimpleIntegrator
    {
    public:
        SimpleIntegrator();
        SimpleIntegrator(const SimpleIntegrator&) = delete;
        SimpleIntegrator& operator=(const SimpleIntegrator&) = delete;
        ~SimpleIntegrator();

        double computeLi(const Geometry::Line&, const Scene&, const Rendering::Camera&, IntersectionResult *, VisibilityTester *);
        
    protected:
    };
}
}