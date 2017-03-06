
#pragma once

#include "3d/Projection.h"
#include "3d/Viewport.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"

namespace Realisim
{
namespace TreeD
{
    class Camera
    {
    public:
        
        Camera();
        Camera(const Camera&) = default;
        Camera& operator=( const Camera& ) = default;
        virtual ~Camera();
        
        Math::Vector3 cameraToWorld( const Math::Vector3& ) const;
        Math::Vector3 cameraDeltaToWorld( const Math::Vector3& ) const;
        const Math::Vector3& getLat() const { return mLat; }
        const Math::Vector3& getLook() const { return mLook; }
        const Math::Vector3& getPos() const { return mPos; }
        const Projection& getProjection() const {return mProjection;}
        const Math::Matrix4& getProjectionMatrix() const;
        const Math::Vector3& getUp() const { return mUp; }
        const Math::Matrix4& getViewMatrix() const;
double getVisibleHeight() const;
double getVisibleWidth() const;
        const Viewport& getViewport() const {return mViewport;}
        const double getZoom() const { return mProjection.zoom(); }
        bool isProjectionProportionalToViewport() const;
        //void popMatrices() const;
        //void pushAndApplyMatrices() const;
        void rotate( double, Math::Vector3, Math::Vector3 = Math::Vector3() );
        Math::Vector3 screenToWorld( Math::Vector2, const Math::Vector3& = Math::Vector3(0.0)) const;
        Math::Vector3 screenDeltaToWorld( Math::Vector2, const Math::Vector3& = Math::Vector3(0.0)) const;
        void set( const Math::Vector3&, const Math::Vector3&, const Math::Vector3& );
        //void setOrthoProjection(double, double, double);
        //void setOrthoProjection(double, double, double, double);
        //void setPerspectiveProjection(double iFov, double iRatio, double iNear, double iFar, bool iProportional=true);
        void setProjection( const Projection&, bool iProportional );
        //void setProjection(double, double, double, double, double, double, Projection::type, bool = true);
        void setViewport( const Viewport& );
        void setZoom(double);
        void translate( const Math::Vector3& );
        void translateTo( const Math::Vector3& );
        Math::Vector3 worldToCamera( const Math::Vector3& ) const;
        Math::Vector3 worldDeltaToCamera( const Math::Vector3& ) const;
        Math::Vector2 worldToSreen( const Math::Vector3& ) const;
        Math::Vector2 worldDeltaToSreen( const Math::Vector3& ) const;
        
        //  QString toString() const;
        void print() const; //std::string toString() const;
        
    protected:  
        void computeProjection(); 
        void computeViewMatrix();
        const Math::Vector3& getLookVector() const;
        
        Math::Vector3 mPos; //mEye
        Math::Vector3 mLat; //mLateralVector
        Math::Vector3 mLook;  //mLookAt
        Math::Vector3 mLookVector; //mLookVector
        Math::Vector3 mUp; //mUpVector
        Projection mProjection;
        bool mIsProjectionProportionalToViewport;
        Viewport mViewport;
        mutable Math::Matrix4 mProjectionMatrix;
        mutable Math::Matrix4 mViewMatrix;
    };
    
} //treeD
} //realisim

