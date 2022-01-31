#pragma once

#include "Reactor/DataStructures/Scene/ModelNode.h"
#include "Geometry/LineSegment.h"
#include "Geometry/Rectangle.h"
#include <vector>

namespace Realisim {
namespace Synthroll {

    //-----------------------------------------------------------------------------------------------------------------
    struct Polygon2d {
    public:
        Polygon2d() = default;
        ~Polygon2d() = default;

        int getNumberOfVertices() const { return (int)mVertices.size(); }
        const std::vector<Math::Vector2>& getVertices() const { return mVertices; }
        void setVertices(const std::vector<Math::Vector2>& iVertices) { mVertices = iVertices; }

    protected:
        std::vector<Math::Vector2> mVertices;
    };

    //-----------------------------------------------------------------------------------------------------------------
    struct TerrainSpline
    {
        TerrainSpline() = default;
        TerrainSpline(const TerrainSpline&) = delete;
        TerrainSpline& operator=(const TerrainSpline&) = delete;
        ~TerrainSpline();

        std::vector<Geometry::LineSegment> mSegments;
        std::vector<Polygon2d> m2dPolygons;
    };

    //-----------------------------------------------------------------------------------------------------------------
    class Terrain {
    public:
        Terrain();
        ~Terrain();

        int addSpline();
        int addSegment(int iSplineIndex, const Geometry::LineSegment&);
        void clear();
        int getNumberOf2dPolygons(int iSplineIndex) const;
        int getNumberOfSegments(int iSplineIndex) const;
        int getNumberOfSplines() const { return (int)mSplines.size();  }        
        const Polygon2d& get2dPolygon(int iSplineIndex, int iIndex) const;
        const Geometry::LineSegment& getSegment(int iSplineIndex, int iIndex) const;
        Geometry::LineSegment& getSegmentRef(int iSplineIndex, int iIndex);
        const TerrainSpline& getSpline(int iSplineIndex) const;
        TerrainSpline& getSplineRef(int iSplineIndex);
        const Math::Vector2& getStartPoint() const { return mStartPoint; }
        const Reactor::ModelNode* getSplineModelNode(int iSplineIndex) const;
        Reactor::ModelNode* getSplineModelNodePtr(int iSplineIndex);
        bool isDirty() const { return mIsDirty; }
        bool isSplineDirty(int iSplineIndex) const;
        int removeSpline(int iSplineIndex);
        int removeSegment(int iSplineIndex, int iIndex);
        void setSplineModelNode(int iSplineIndex, Reactor::ModelNode*);
        void setSplineAsDirty(int iSplineIndex, bool);
        void setStartPoint(const Math::Vector2& iV){ mStartPoint = iV; }

    protected:
        void computePolygons(int iSplineIndex);

        Math::Vector2 mStartPoint;
        Geometry::Rectangle mEndRect;

        std::vector<TerrainSpline*> mSplines;
        std::vector<Reactor::ModelNode*> mSplineModelNodes; // not owned
        std::vector<bool> mSplineIsDirty;
        bool mIsDirty;

        static TerrainSpline mDummySpline;
        static Geometry::LineSegment mDummySegment;
        static Polygon2d mDummyPolygon;
    };
}
}

