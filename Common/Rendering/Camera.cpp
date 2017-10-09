

#include "Rendering/Camera.h"
#include <iomanip>
#include <sstream>

using namespace Realisim;
using namespace Math;
using namespace Rendering;

//-----------------------------------------------------------------------------
//--- Camera
//-----------------------------------------------------------------------------
Camera::Camera() :
    mPosition(),
    mLateralVector(),
	mFocal(),
    mLookVector(),
    mUpVector(),
    mProjection(),
    mZoomFactor(1.0),
    mViewport(),
    mProjectionMatrix(),
    mViewMatrix(),
    mViewMatrixInverse(),
	mIsProjectionProportionalToViewport(true)
{
    set(Vector3(0, 0, 100),
        Vector3(0, 0, 0),
        Vector3(0, 1, 0));
    Viewport v(100, 100);
    Projection p;
    p.setPerspectiveProjectionWithRatio(60, v.getRatio(), 0.5, 10000.0);
    setProjection(p);
    
}

//-----------------------------------------------------------------------------
Camera::~Camera()
{}

//-----------------------------------------------------------------------------
Vector3 Camera::cameraToWorld(const Vector3& iP) const
{
    return (getViewMatrixInverse() * Vector4(iP, 1)).xyz();
}

//-----------------------------------------------------------------------------
Vector3 Camera::cameraDeltaToWorld(const Vector3& iV) const
{
    return (getViewMatrixInverse() * Vector4(iV, 0)).xyz();
}

//-----------------------------------------------------------------------------
void Camera::computeProjection()
{
    const Projection& p = getProjection();
    // La projectio est lié au viewport. Ici, nous voulons que la camera
    // conserve la projection demandé par setProjection() même quand le 
    // viewport change via setViewport. Pour ce faire, on recalcule
    // le ratio viewport / projection courant et on ajuste la hauteur
    // de la projection pour correspondre
    // rw: ratio viewportWidth / projectionWidth
    // h = nouvelle hauteur.
    //
	if( isProjectionProportionalToViewport() )
    {
        const double rw = getViewport().getWidth() / p.getWidth();
        const double h = getViewport().getHeight() / rw;
        const double cy = p.getBottom() + p.getHeight() / 2.0;
        mProjection.setTop(cy + h / 2.0);
        mProjection.setBottom(cy - h / 2.0);
    } 

    // Afin d'appliquer correctement le zoom
    // on doit trouver le centre de la projection originale.
    // Ensuite on détermine la taille du rectangle
    // de la projection avec le zoom a partir de ce centre.
    //    
    double cx = 0.0, cy = 0.0, l, r, b, t;
    cx = p.getLeft() + p.getWidth() / 2.0;
    cy = p.getBottom() + p.getHeight() / 2.0;

    const double widthWithZoom = p.getWidth() * 1.0 / getZoomFactor();
    const double heightWithZoom = p.getHeight() * 1.0 / getZoomFactor();

    l = cx - widthWithZoom / 2.0;
    r = cx + widthWithZoom / 2.0;
    b = cy - heightWithZoom / 2.0;
    t = cy + heightWithZoom / 2.0;
    const double f = p.getFar();
    const double n = p.getNear();

    Projection projWithZoom(l, r, b, t, n, f, p.getType());
    mProjectionMatrix = projWithZoom.getProjectionMatrix();
}

//-----------------------------------------------------------------------------
void Camera::computeViewMatrix()
{
    Matrix4 viewMatrix(mLateralVector, mUpVector, mLookVector);
    
    // Classically, we would put the translation in the matrix
    // and then invert the matrix...
    // Here we negate the translation, transpose the matrix (which is equivalent
    // to an inversion in this particualar case). Then we multiply by the
    // negated (inverse) translation.
    //
    Matrix4 t(-getPosition());
    mViewMatrix = viewMatrix.transpose() * t;
    mViewMatrixInverse = mViewMatrix.getInverse();
}

//-----------------------------------------------------------------------------
// returns a vector representing, in world coordinate, the look direction
// of the camera. getFocal() - getPosition()
//
Vector3 Camera::getDirection() const
{
	return getFocal() - getPosition();
}

//-----------------------------------------------------------------------------
// The point in world space where the camera is looking.
//
const Vector3& Camera::getFocal() const
{ return mFocal; }

//-----------------------------------------------------------------------------
Realisim::Geometry::Frustum Camera::getFrustum() const
{
    const Realisim::Rendering::Projection& projection = this->getProjection();
    Realisim::Math::Vector3 corners[8];
    corners[0] = mPosition;
    corners[1] = mPosition;
    corners[2] = mPosition;
    corners[3] = mPosition;
    corners[4] = mPosition;
    corners[5] = mPosition;
    corners[6] = mPosition;
    corners[7] = mPosition;

    const double distanceRatio = projection.getFar() / projection.getNear();

    corners[0] += mLateralVector*(projection.getLeft());     // low left near
    corners[0] += mUpVector*(projection.getBottom());
    corners[0] += mLookVector*(-projection.getNear());

    corners[1] += mLateralVector*(projection.getRight());    // low right near
    corners[1] += mUpVector*(projection.getBottom());
    corners[1] += mLookVector*(-projection.getNear());

    corners[2] += mLateralVector*(projection.getRight());    // high right near
    corners[2] += mUpVector*(projection.getTop());
    corners[2] += mLookVector*(-projection.getNear());

    corners[3] += mLateralVector*(projection.getLeft());     // high left near
    corners[3] += mUpVector*(projection.getTop());
    corners[3] += mLookVector*(-projection.getNear());

    corners[4] += mLateralVector*(distanceRatio*projection.getLeft());     // low left far
    corners[4] += mUpVector*(distanceRatio*projection.getBottom());
    corners[4] += mLookVector*(-projection.getFar());

    corners[5] += mLateralVector*(distanceRatio*projection.getRight());    // low right far
    corners[5] += mUpVector*(distanceRatio*projection.getBottom());
    corners[5] += mLookVector*(-projection.getFar());

    corners[6] += mLateralVector*(distanceRatio*projection.getRight());    // high right far
    corners[6] += mUpVector*(distanceRatio*projection.getTop());
    corners[6] += mLookVector*(-projection.getFar());

    corners[7] += mLateralVector*(distanceRatio*projection.getLeft());     // high left far
    corners[7] += mUpVector*(distanceRatio*projection.getTop());
    corners[7] += mLookVector*(-projection.getFar());

    Realisim::Geometry::Frustum frustum;
    frustum.set(corners);

    return frustum;
}

//-----------------------------------------------------------------------------
// The lateral vector (in world space) represents the axis of rotation for Yaw.
//
const Vector3& Camera::getLateralVector() const
{ return mLateralVector; }

//-----------------------------------------------------------------------------
// The vector formed by getPosition() - getFocal(). It is the vector representing
// the axis of rotation for Roll.
//
const Vector3& Camera::getLookVector() const
{ return mLookVector; }

//-----------------------------------------------------------------------------
// The position of the camera in world space.
//
const Vector3& Camera::getPosition() const
{ return mPosition; }

//-----------------------------------------------------------------------------
const Projection& Camera::getProjection() const
{return mProjection;}

//-----------------------------------------------------------------------------
const Matrix4& Camera::getProjectionMatrix() const
{
    return mProjectionMatrix;
}

//-----------------------------------------------------------------------------
const Vector3& Camera::getUp() const
{ return mUpVector; }

//-----------------------------------------------------------------------------
const Matrix4& Camera::getViewMatrix() const
{
    return mViewMatrix;
}

//-----------------------------------------------------------------------------
const Matrix4& Camera::getViewMatrixInverse() const
{
    return mViewMatrixInverse;
}

//-----------------------------------------------------------------------------
const Viewport& Camera::getViewport() const
{return mViewport;}

//-----------------------------------------------------------------------------
double Camera::getZoomFactor() const
{
    return mZoomFactor;
}

//-----------------------------------------------------------------------------
bool Camera::isProjectionProportionalToViewport() const
{ return mIsProjectionProportionalToViewport; }

//-----------------------------------------------------------------------------
// Unprojects a ndc coordinates back to 3d world coordinate.
//
Vector3 Camera::ndcToWorld(const Vector3& iNdc) const
{
	Vector4 r = (getProjectionMatrix() * getViewMatrix()).getInverse() * Vector4(iNdc, 1);
	return r.xyz() / r.w();
}

////-----------------------------------------------------------------------------
//Vector3 Camera::ndcDeltaToWorld(const Vector3& iNdc) const
//{
//}

//-----------------------------------------------------------------------------
// Convert a screen coordinate in pixel to a 3d world coordinate.
// In order to have a decent z value, a reference point must be passed in. The
// reference point represents a point on a perpendicular plane to the camera. The
// The returned 3d value will be the intersection of that plane and a line passing
// by the pixel coordinate.
//
// Note:    since Qt has (0,0) at top-left of the window and openGL has (0,0) at
//          bottom-left, the y axis need to be inverted if the iPixel coordinates
//          comes from qt...
//
//          ex:
//                  //Invert y axis since Qt and openGL are inverted on y...
//                  const double pixelY = getViewport().getHeight() - iPixel.y();
//
Vector3 Camera::pixelToWorld(const Vector2& iPixel, const Vector3& iRerefence) const
{
    Vector3 referenceNdc = worldToNdc(iRerefence);
    
    const Viewport& v = getViewport();
    Vector3 pixelNdc(iPixel.x() / v.getWidth() * 2.0 - 1,
                     iPixel.y() / v.getHeight() * 2.0 - 1,
                     referenceNdc.z() );
    
    // unproject the pixel ndc to get world coordinate
    return ndcToWorld(pixelNdc);
}

//-----------------------------------------------------------------------------
// see pixelToWorld
// The subtlety here is that we pass iPixel, being the origin pixel from which
// the delta is applied. This is to take perspective into account properly.
//
Vector3 Camera::pixelDeltaToWorld(const Math::Vector2& iPixel,
                                   const Vector2& iPixelDelta,
                                   const Vector3& iReference) const
{
    Vector3 p0 = pixelToWorld(iPixel, iReference);
    Vector3 p1 = pixelToWorld(iPixel + iPixelDelta, iReference);
    return p1 - p0;
}

//-----------------------------------------------------------------------------
// Rotates the camera by iRadian around axis iAxis. The position, in world
// coordinate, of the axis can be specified by iAxisPos to rotate around an
// arbitrary point.
//
// For example to rotate around the head of a character...
//		cam.rotate( iRot, vec3(0,1,0), player.getHeadPosition() )...
//
void Camera::rotate(double iRad, Vector3 iAxis,
    Vector3 iAxisPos /*= Vector3()*/)
{
    Vector3 eye = getPosition(), focal = getFocal();
    Vector3 up = getUp();

    Matrix4 r(iRad, iAxis);
    Matrix4 t(iAxisPos);
    r = t * r * t.getInverse();

    eye = (r * Vector4(eye, 1)).xyz();
    focal = (r * Vector4(focal, 1)).xyz();
    up = (r * Vector4(up, 0)).xyz();
    set(eye, focal, up);
}

//-----------------------------------------------------------------------------
// Positions the camera in 3d space.
// iEye: is the position of the camera.
// iFocal: is the point looked at by the camera. The look vector is defined
//        by iEye - iFocal.
// iUpVector: is the up vector of the camera, this mostly define the orientation
//        of the camera. The coordinate system of the camera will be computed
//        from the look vector and the up vector. It is to note that if the
//        up vector and look vector are colinear, the camera will be invalid.
//        
//        When the up vector is not perpendicular to the look vector, it will
//        be recomputed so the coordinate system of the camera is orthonormal.
//
void Camera::set(const Vector3& iEye,
    const Vector3& iFocal,
    const Vector3& iUpVector)
{
    // Définie la position de la caméra, le point visé et le vecteur up.Le
    //vecteur Latéral sera calculé à partir du vecteur up et ensuite le up
    // sera recalculé afin d'assurer une base normale.
    //
    mPosition = iEye;
	mFocal = iFocal;
    mUpVector = iUpVector;

    //on commence par calculer le vecteur laterale parce que le vecteur up
    //final est dependant du vecteur lateral
    mLookVector = getPosition() - getFocal();
    mLookVector.normalize();

    mLateralVector = mUpVector ^ mLookVector;
    mLateralVector.normalize();

    mUpVector = mLookVector ^ mLateralVector;
    mUpVector.normalize();

    computeViewMatrix();
}

//-----------------------------------------------------------------------------
void Camera::setProjection(const Projection& iP)
{
	if (mProjection != iP)
	{
		mProjection = iP;
		computeProjection();
	}
}

//-----------------------------------------------------------------------------
void Camera::setProjectionProportionalToViewport(bool iV)
{
	if (isProjectionProportionalToViewport() != iV)
	{
		mIsProjectionProportionalToViewport = iV;
		computeProjection();
	}
}

//-----------------------------------------------------------------------------
void Camera::setViewport( const Viewport& iV)
{
	if (mViewport != iV)
	{
		mViewport = iV;
		computeProjection();
	}    
}

//-----------------------------------------------------------------------------
void Camera::setZoomFactor(double iZoom)
{
	if (mZoomFactor != iZoom)
	{
		mZoomFactor = iZoom;
		computeProjection();
	}
}

//-----------------------------------------------------------------------------
std::string Camera::toString() const
{
    using namespace std;
    ostringstream oss;
    
    oss << fixed << setprecision(3);
    oss << "\n\nPos: " << mPosition.x() << " " << mPosition.y() << " " << mPosition.z();
    oss << "\nLook: " << mFocal.x() << " " << mFocal.y() << " " << mFocal.z();
    oss << "\nLat: " << mLateralVector.x() << " " << mLateralVector.y() << " " << mLateralVector.z();
    oss << "\nUp: " << mUpVector.x() << " " << mUpVector.y() << " " << mUpVector.z();
    return oss.str();
}

//-----------------------------------------------------------------------------
// Translates the camera position and focal position by iV in world space.
//
void Camera::translate(const Vector3& iV)
{
    set(mPosition + iV, mFocal + iV, getUp());
}

//-----------------------------------------------------------------------------
// Translates the camera position to iV in world space. the focal point will
// follow accordingly.
//
void Camera::translateTo(const Vector3& iV)
{
    Vector3 d = iV - getPosition();
    translate(d);
}

//-----------------------------------------------------------------------------
Vector3 Camera::worldToCamera(const Vector3& iP) const
{
	return (getViewMatrix() * Vector4(iP, 1)).xyz();
}

//-----------------------------------------------------------------------------
Vector3 Camera::worldDeltaToCamera(const Vector3& iV) const
{
    return (getViewMatrix() * Vector4(iV, 0)).xyz();
}

//-----------------------------------------------------------------------------
// Projects the 3d world coordinate iWorld to ndc
//
Vector3 Camera::worldToNdc(const Math::Vector3& iWorld) const
{
	Vector4 ndc = getProjectionMatrix() * getViewMatrix() * Vector4(iWorld, 1);
	return ndc.xyz() / ndc.w();
}

//-----------------------------------------------------------------------------
Vector3 Camera::worldDeltaToNdc(const Math::Vector3& iWorld) const
{
	Vector3 a = worldToNdc(Vector3(0.0));
	Vector3 b = worldToNdc(iWorld);
	return (b - a);
}

//-----------------------------------------------------------------------------
// Convert a 3d world position to screen coordinates.
//
// Note: Qt y axis is inverted from openGL, so if the result is to be displayed
//  in a qt window, the y axis will need to be inverted
//
Vector2 Camera::worldToPixel(const Vector3& iP, bool *iIsOnscreen/*=nullptr*/) const
{
    Vector3 ndc = worldToNdc(iP);
    
    // from range [-1, 1] to [0, 1]
    ndc = (ndc + Vector3(1)) / 2.0;
    
	if(iIsOnscreen)
	{
		// if ndc.z is not in [-1, 1] range, then it is outside the near/far plane,
		// so not on screen
		//
		*iIsOnscreen = ndc.z() >= -1.0 && ndc.z() <= 1.0;
	}

    // range [0, 1] multiplied by viewport to get screen coordinates
    const Viewport& v = getViewport();
    
    return Vector2(ndc.x() * v.getWidth(),
                   ndc.y() * v.getHeight());
}

//-----------------------------------------------------------------------------
Vector2 Camera::worldDeltaToPixel(const Vector3& iV, bool *iIsOnscreen /*= nullptr*/) const
{
	bool a, b;
    Vector2 v0 = worldToPixel(Vector3(0.0), &a);
    Vector2 v1 = worldToPixel(iV, &b);

	if(iIsOnscreen)
	{ 
		*iIsOnscreen = a && b;
	}

    return v1 - v0;
}
