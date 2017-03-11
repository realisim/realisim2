
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
        bool isProjectionProportionalToViewport() const;
        const Math::Vector3& lateralVector() const;
        const Math::Vector3& lookAt() const;
        const Math::Vector3& position() const;
        const Projection& projection() const;
        const Math::Matrix4& projectionMatrix() const;
        void rotate( double iRad, Math::Vector3 iAxis, Math::Vector3 iAxisPos = Math::Vector3() );
        Math::Vector3 screenToWorld( Math::Vector2, const Math::Vector3& = Math::Vector3(0.0)) const;
        Math::Vector3 screenDeltaToWorld( Math::Vector2, const Math::Vector3& = Math::Vector3(0.0)) const;
        void set( const Math::Vector3& iEye, const Math::Vector3& iLookAt, const Math::Vector3& iUp );
        void setProjection( const Projection&, bool iProportional );
        void setViewport( const Viewport& );
        void setZoomFactor(double);
        void translate( const Math::Vector3& );
        void translateTo( const Math::Vector3& );
        const Math::Vector3& upVector() const;
        const Math::Matrix4& viewMatrix() const;
double getVisibleHeight() const;
double getVisibleWidth() const;
        const Viewport& viewport() const;
        Math::Vector3 worldToCamera( const Math::Vector3& ) const;
        Math::Vector3 worldDeltaToCamera( const Math::Vector3& ) const;
        Math::Vector2 worldToSreen( const Math::Vector3& ) const;
        Math::Vector2 worldDeltaToSreen( const Math::Vector3& ) const;
        const double zoomFactor() const { return mProjection.zoom(); }
        
        std::string toString() const;
        
    protected:  
        void computeProjection(); 
        void computeViewMatrix();
        const Math::Vector3& lookVector() const;
        
        Math::Vector3 mPosition;
        Math::Vector3 mLateralVector;
        Math::Vector3 mLookAt;
        Math::Vector3 mLookVector;
        Math::Vector3 mUpVector;
        Projection mProjection;
        bool mIsProjectionProportionalToViewport;
        Viewport mViewport;
        mutable Math::Matrix4 mProjectionMatrix;
        mutable Math::Matrix4 mViewMatrix;
    };
    
} //treeD
} //realisim

