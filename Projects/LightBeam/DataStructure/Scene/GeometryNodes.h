#pragma once

#include "Interfaces.h"
#include "Geometry/Line.h"
#include "Geometry/Mesh.h"
#include "Geometry/OctreeOfMeshFaces.h"
#include "Geometry/Plane.h"
#include "Geometry/Sphere.h"

namespace Realisim
{
namespace LightBeam
{
    class IntersectionResult;

    //-------------------------------------------------------------------------
    class PlaneNode : public ISceneNode, public IRenderable
    {
    public:
        PlaneNode();
        ~PlaneNode() = default;

        const Geometry::Plane& getPlane() const;
        virtual bool intersects(const Geometry::Line& iRay) const override;
        virtual bool intersect(const Geometry::Line& iRay, IntersectionResult* opResult) const override;
        void setPlane(const Geometry::Plane&);

    protected:
        Geometry::Plane mPlane;
    };

    //-------------------------------------------------------------------------
    class SphereNode : public ISceneNode, public IRenderable
    {
    public:
        SphereNode();
        ~SphereNode() = default;

        const Geometry::Sphere& getSphere() const;
        virtual bool intersects(const Geometry::Line& iRay) const override;
        virtual bool intersect(const Geometry::Line& iRay, IntersectionResult* opResult) const override;
        void setSphere(const Geometry::Sphere&);

    protected:
        Geometry::Sphere mSphere;
    };

    //-------------------------------------------------------------------------
    class MeshNode : public ISceneNode, public IRenderable
    {
    public:
        MeshNode();
        MeshNode(const MeshNode&) = delete;
        MeshNode& operator=(const MeshNode&) = delete;
        ~MeshNode();

        const Geometry::Mesh* getMesh() const;
        virtual bool intersects(const Geometry::Line& iRay) const override;
        virtual bool intersect(const Geometry::Line& iRay, IntersectionResult* opResult) const override;
        void setMeshAndTakeOwnership(Geometry::Mesh*);

    protected:
        Geometry::OctreeOfMeshFaces mOctree;
        Geometry::Mesh *mpMesh; // can be null, owned
    };
}
}