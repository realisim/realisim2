#pragma once

#include "Core/Color.h"
#include "Geometry/Rectangle.h"
#include "Math/Vector.h"

namespace Realisim
{
namespace LightBeam
{
    class ImageCells
    {
    public:
        ImageCells();
        ImageCells(const ImageCells&) = delete;
        ImageCells& operator=(const ImageCells&) = delete;
        ~ImageCells() = default;

        struct Node
        {
            Node();
            ~Node();

            friend class ImageCells;

            const Core::Color& getColor() const;
            const Geometry::Rectangle& getCoverage() const;
            double getDepth() const;
            void setColor(const Core::Color&);
            void setDepth(double);
            void split();

            Node* mpParent;
            std::vector<Node*> mChilds;

        protected:

            Geometry::Rectangle mCoverage;
            Core::Color mRgba;
            double mDepth;
        };

        void clear();
        static uint64_t getNumberOfNodeAtDepthLevel(int);
        Node *getRoot();
        void setCoverage(double iX, double iY, double iW, double iH);

    protected:

        Node *mpRoot; //never null
        
    };
}
}