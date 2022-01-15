#pragma once

#include "Reactor/DataStructures/Scene/ModelNode.h"
#include "Geometry/LineSegment.h"
#include "Geometry/Rectangle.h"
#include <vector>

namespace Realisim {
namespace Synthroll {

    class Terrain {
    public:
        Terrain();
        ~Terrain();

        int addSegment(const Geometry::LineSegment&);
        void clear();
        int getNumberOfSegments() const;
        const Geometry::LineSegment& getSegment(int iIndex) const;
        Geometry::LineSegment& getSegmentRef(int iIndex);
        const Math::Vector2& getStartPoint() const { return mStartPoint; }
        const Reactor::ModelNode* getModelNode(int iIndex) const;
        Reactor::ModelNode* getModelNodePtr(int iIndex);
        bool isDirty() const { return mIsDirty; }
        bool isSegmentDirty(int iIndex) const;
        void removeSegment(int iIndex);
        void setModelNode(int iIndex, Reactor::ModelNode*);
        void setSegmentAsDirty(int iIndex, bool);
        void setStartPoint(const Math::Vector2& iV){ mStartPoint = iV; }

    protected:
        Math::Vector2 mStartPoint;
        Geometry::Rectangle mEndRect;

        std::vector<Geometry::LineSegment> mSegments;
        std::vector<Reactor::ModelNode*> mModelNodes; // not owned
        std::vector<bool> mSegmentIsDirty;
        bool mIsDirty;

        static Geometry::LineSegment mDummySegment;
    };
}
}

