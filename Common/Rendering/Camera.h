
#pragma once

#include "Rendering/Projection.h"
#include "Rendering/Viewport.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Geometry/Frustum.h"

namespace Realisim
{
namespace Rendering
{
    // This class present a Camera in 3d space.
    //
    // In its simple form a camera is positionned in space and looks in a certain
    // direction. Many parameters concerning the projection and the zoom can be
    // modified. It is also possible to convert 3d world position to camera space 
    // or screen space (and vice versa) via this class.
    //
    // 1- Positionning the camera
    //      Function set( Vector3 iEye, Vector3 iFocal, Vector3 iUp ) is used to 
    //      position the camera. see cpp file for details.
    //
    // 2- Tuning the projection parameters
    //      Modification to the projection is mostly done via method setProjection.
    //      See Projection class. An optical zoom can be applied via method
    //      setZoomFactor(). The zoom factor is applied to the current projection
    //      and can be queried via getZoomFactor().
    //
    // 3- Coordinate systems
    //      The camera has a right hand coordinate system. The coordinate system is 
    //      dependant on method set(). Here are a couple of example:
    //
    //      1- 
    //      set(Vector3(0, 0, 100),     // positionned at 100 on z axis
    //          Vector3(0, 0, 0),       // looking at zero
    //          Vector3(0, 1, 0) );     // y is up.
    //
    //      The resulting camera coordinate system will be: 
    //          As specified, looking down on the z axis with y as up vector.
    //          Following the right hand system, x as the lateral vector.
    //
    //            Y
    //            ^
    //            |
    //            |
    //            ------> x
    //           /
    //          /
    //         Z
    //
    //      2- 
    //      set(Vector3(0, 0, -100),     // positionned at 100 on z axis
    //          Vector3(0, 0, 0),       // looking at zero
    //          Vector3(0, 1, 0) );     // y is up.
    //
    //      The resulting camera coordinate system will be: 
    //          As specified, looking down on the -z axis with y as up vector.
    //          Following the right hand system, -x is as the lateral vector.
    //
    //            Y
    //            ^
    //            |  -Z
    //            | /
    //    -X ------/
    //
    //
    //      3- 
    //      set(Vector3(0, 100, 0),     // positionned at 100 on z axis
    //          Vector3(0, 0, 0),       // looking at zero
    //          Vector3(0, 0, 1) );     // y is up.
    //
    //      The resulting camera coordinate system will be: 
    //          As specified, looking down on the y axis with Z as up vector.
    //          Following the right hand system, x  as the lateral vector.
    //
    //            Z
    //            ^
    //            |
    //            |
    //     X ------
    //           /
    //          /
    //         y
    //
    //
    //  4- Tranformation between coordinate systems:
    //      The following methods performs transformation between world, camera
    //      and screen space:
    //      
    //      cameraToWorld(), cameraDeltaToWorld()
    //      pixelToWorld(), pixelDeltaToWorld()
    //      worldToCamera(), worldDeltaToCamera()
    //      worldToScreen(), worldDeltaToSreen()
    //
    //      These methods come with two variant. The non *DeltaTo* transform a point
    //      from one corrdinate system to another. worldToScreen() will convert a
    //      3d point in world space to a 2d point in screen (pixel) space.
    //
    //      The *DeltaTo* variation is simply a convenience to transform a magnitude
    //      from one coordinate system to another.
    //
    //      Note: cameraToScreen is not present but can be achieve by calling
    //          worldToScreen( cameraToWorld() );
    //
    class Camera
    {
    public:
        
        Camera();
        Camera(const Camera&) = default;
        Camera& operator=( const Camera& ) = default;
        ~Camera();
        
        Math::Vector3 cameraToWorld( const Math::Vector3& ) const;
        Math::Vector3 cameraDeltaToWorld( const Math::Vector3& ) const;
		Math::Vector3 getDirection() const;
        const Math::Vector3& getFocal() const;
        Realisim::Geometry::Frustum getFrustum() const;
		const Math::Vector3& getLateralVector() const;
        const Math::Vector3& getLookVector() const;
        const Math::Vector3& getPosition() const;
        const Projection& getProjection() const;
        const Math::Matrix4& getProjectionMatrix() const;
		const Math::Vector3& getUp() const;
		const Math::Matrix4& getViewMatrix() const;
        const Math::Matrix4& getViewMatrixInverse() const;
		const Viewport& getViewport() const;
		double getZoomFactor() const;
		bool isProjectionProportionalToViewport() const;
		Math::Vector3 ndcToWorld(const Math::Vector3&) const;
//		Math::Vector3 ndcDeltaToWorld(const Math::Vector3&) const;  
        Math::Vector3 pixelToWorld( const Math::Vector2& iPixel, const Math::Vector3& iReference ) const;
        Math::Vector3 pixelDeltaToWorld( const Math::Vector2& iPixel, const Math::Vector2& iPixelDelta, const Math::Vector3& iReference ) const;
		void rotate(double iRad, Math::Vector3 iAxis, Math::Vector3 iAxisPos = Math::Vector3());
        void set( const Math::Vector3& iEye, const Math::Vector3& iFocal, const Math::Vector3& iUp );
        void setProjection( const Projection& );
		void setProjectionProportionalToViewport(bool);
        void setViewport( const Viewport& );
        void setZoomFactor(double);
		std::string toString() const;
        void translate( const Math::Vector3& );
        void translateTo( const Math::Vector3& );       
        Math::Vector3 worldToCamera( const Math::Vector3& ) const;
        Math::Vector3 worldDeltaToCamera( const Math::Vector3& ) const;
		Math::Vector3 worldToNdc(const Math::Vector3&) const;
		Math::Vector3 worldDeltaToNdc(const Math::Vector3&) const;
        Math::Vector2 worldToPixel( const Math::Vector3&, bool *iIsOnscreen = nullptr ) const;
        Math::Vector2 worldDeltaToPixel( const Math::Vector3&, bool *iIsOnscreen = nullptr ) const;

    protected:  
        void computeProjection(); 
        void computeViewMatrix();
        
        Math::Vector3 mPosition;
        Math::Vector3 mLateralVector;
        Math::Vector3 mFocal;
        Math::Vector3 mLookVector;
        Math::Vector3 mUpVector;
        Projection mProjection;
        double mZoomFactor;
        Viewport mViewport;
        mutable Math::Matrix4 mProjectionMatrix;
        mutable Math::Matrix4 mViewMatrix;
        mutable Math::Matrix4 mViewMatrixInverse;
		bool mIsProjectionProportionalToViewport;


    };
    
} //treeD
} //realisim

