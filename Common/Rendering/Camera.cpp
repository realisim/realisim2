

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
    mLookAt(),
    mLookVector(),
    mUpVector(),
    mProjection(),
    mZoomFactor(1.0),
    mViewport(),
    mProjectionMatrix(),
    mViewMatrix()
{
    set(Vector3(0, 0, 100),
        Vector3(0, 0, 0),
        Vector3(0, 1, 0));
    Viewport v(100, 100);
    Projection p;
    p.setPerspectiveProjection(60, v.ratio(), 0.5, 10000.0);
    setProjection(p);
    
}

//-----------------------------------------------------------------------------
Camera::~Camera()
{}

//-----------------------------------------------------------------------------
Vector3 Camera::cameraToWorld(const Vector3& iP) const
{
    return (viewMatrix().inverse() * Vector4(iP, 1)).xyz();
}

//-----------------------------------------------------------------------------
Vector3 Camera::cameraDeltaToWorld(const Vector3& iV) const
{
    return (viewMatrix().inverse() * Vector4(iV, 0)).xyz();
}

//-----------------------------------------------------------------------------
void Camera::computeProjection()
{
    const Projection& p = projection();
    // La projectio est lié au viewport. Ici, nous voulons que la camera
    // conserve la projection demandé par setProjection() même quand le 
    // viewport change via setViewport. Pour ce faire, on recalcule
    // le ratio viewport / projection courant et on ajuste la hauteur
    // de la projection pour correspondre
    // rw: ratio viewportWidth / projectionWidth
    // h = nouvelle hauteur.
    //
    {
        const double rw = viewport().width() / p.width();
        const double h = viewport().height() / rw;
        const double cy = p.bottom() + p.height() / 2.0;
        mProjection.setTop(cy + h / 2.0);
        mProjection.setBottom(cy - h / 2.0);
    }    

    // Afin d'appliquer correctement le zoom
    // on doit trouver le centre de la projection originale.
    // Ensuite on détermine la taille du rectangle
    // de la projection avec le zoom a partir de ce centre.
    //    
    double cx = 0.0, cy = 0.0, l, r, b, t;
    cx = p.left() + p.width() / 2.0;
    cy = p.bottom() + p.height() / 2.0;

    const double widthWithZoom = p.width() * 1.0 / zoomFactor();
    const double heightWithZoom = p.height() * 1.0 / zoomFactor();

    l = cx - widthWithZoom / 2.0;
    r = cx + widthWithZoom / 2.0;
    b = cy - heightWithZoom / 2.0;
    t = cy + heightWithZoom / 2.0;
    const double f = p.farPlane();
    const double n = p.nearPlane();

    Projection projWithZoom(l, r, b, t, n, f, p.type());
    mProjectionMatrix = projWithZoom.projectionMatrix();
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
    Matrix4 t(-position());
    mViewMatrix = viewMatrix.transpose() * t;
}

//-----------------------------------------------------------------------------
// The lateral vector (in world space) represents the axis of rotation for Yaw.
//
const Vector3& Camera::lateralVector() const
{ return mLateralVector; }

//-----------------------------------------------------------------------------
// The point in world space where the camera is looking.
//
const Vector3& Camera::lookAt() const
{ return mLookAt; }

//-----------------------------------------------------------------------------
// The vector formed by position() - lookAt(). It is the vector representing
// the axis of rotation for Roll.
//
const Vector3& Camera::lookVector() const
{ return mLookVector; }

//-----------------------------------------------------------------------------
// The position of the camera in world space.
//
const Vector3& Camera::position() const
{ return mPosition; }

//-----------------------------------------------------------------------------
const Projection& Camera::projection() const
{return mProjection;}

//-----------------------------------------------------------------------------
const Matrix4& Camera::projectionMatrix() const
{
    return mProjectionMatrix;
}

//-----------------------------------------------------------------------------
// Projects the 3d world coordinate iWorld to ndc
//
Vector3 Camera::projectToNdc(const Math::Vector3& iWorld) const
{
    Vector4 ndc = projectionMatrix() * viewMatrix() * Vector4(iWorld, 1);
    return ndc.xyz() / ndc.w();
}

//-----------------------------------------------------------------------------
// Rotates the camera by iRad around axis iAxis positionned
// at iAxisPos.
// Axis and position are in world coordinates.
//
void Camera::rotate(double iRad, Vector3 iAxis,
    Vector3 iAxisPos /*= Vector3()*/)
{
    Vector3 eye = position(), lookat = lookAt();
    Vector3 up = upVector();

    Matrix4 r(iRad, iAxis);
    Matrix4 t(iAxisPos);
    r = t * r * t.inverse();

    eye = (r * Vector4(eye, 1)).xyz();
    lookat = (r * Vector4(lookat, 1)).xyz();
    up = (r * Vector4(up, 0)).xyz();
    set(eye, lookat, up);
}

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
//                  const double pixelY = viewport().height() - iPixel.y();
//
Vector3 Camera::screenToWorld(const Vector2& iPixel, const Vector3& iRerefence) const
{
    Vector3 referenceNdc = projectToNdc(iRerefence);
    
    const Viewport& v = viewport();
    Vector3 pixelNdc(iPixel.x() / v.width() * 2.0 - 1,
                     iPixel.y() / v.height() * 2.0 - 1,
                     referenceNdc.z() );
    
    // unproject the pixel ndc to get world coordinate
    return unprojectFromNdc(pixelNdc);
}

//-----------------------------------------------------------------------------
// see screenToWorld
//
Vector3 Camera::screenDeltaToWorld(const Math::Vector2& iPixel,
                                   const Vector2& iPixelDelta,
                                   const Vector3& iReference) const
{
    Vector3 p0 = screenToWorld(iPixel, iReference);
    Vector3 p1 = screenToWorld(iPixel + iPixelDelta, iReference);
    return p1 - p0;
}

//-----------------------------------------------------------------------------
// Positions the camera in 3d space.
// iEye: is the position of the camera.
// iLookAt: is the point looked at by the camera. The look vector is defined
//        by iEye - iLookAt.
// iUpVector: is the up vector of the camera, this mostly define the orientation
//        of the camera. The coordinate system of the camera will be computed
//        from the look vector and the up vector. It is to note that if the
//        up vector and look vector are colinear, the camera will be invalid.
//        
//        When the up vector is not perpendicular to the look vector, it will
//        be recomputed so the coordinate system of the camera is orthonormal.
//
void Camera::set(const Vector3& iEye,
    const Vector3& iLookAt,
    const Vector3& iUpVector)
{
    // Définie la position de la caméra, le point visé et le vecteur up.Le
    //vecteur Latéral sera calculé à partir du vecteur up et ensuite le up
    // sera recalculé afin d'assurer une base normale.
    //
    mPosition = iEye;
    mLookAt = iLookAt;
    mUpVector = iUpVector;

    //on commence par calculer le vecteur laterale parce que le vecteur up
    //final est dependant du vecteur lateral
    mLookVector = position() - lookAt();
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
    mProjection = iP;
    computeProjection();
}

//-----------------------------------------------------------------------------
void Camera::setViewport( const Viewport& iV)
{
    mViewport = iV;
    computeProjection();
}

//-----------------------------------------------------------------------------
void Camera::setZoomFactor(double iZoom)
{
    mZoomFactor = iZoom;
    computeProjection();
}

//-----------------------------------------------------------------------------
std::string Camera::toString() const
{
    using namespace std;
    ostringstream oss;
    
    oss << fixed << setprecision(3);
    oss << "\n\nPos: " << mPosition.x() << " " << mPosition.y() << " " << mPosition.z();
    oss << "\nLook: " << mLookAt.x() << " " << mLookAt.y() << " " << mLookAt.z();
    oss << "\nLat: " << mLateralVector.x() << " " << mLateralVector.y() << " " << mLateralVector.z();
    oss << "\nUp: " << mUpVector.x() << " " << mUpVector.y() << " " << mUpVector.z();
    return oss.str();
}

//-----------------------------------------------------------------------------
// Translates the camera position and lookAt position by iV in world space.
//
void Camera::translate(const Vector3& iV)
{
    set(mPosition + iV, mLookAt + iV, upVector());
}

//-----------------------------------------------------------------------------
// Translates the camera position to iV in world space. the lookAt point will
// follow accordingly.
//
void Camera::translateTo(const Vector3& iV)
{
    Vector3 d = iV - position();
    translate(d);
}

//-----------------------------------------------------------------------------
// Unprojects a ndc coordinates back to 3d world coordinate.
//
Vector3 Camera::unprojectFromNdc(const Vector3& iNdc) const
{
    Vector4 r = (projectionMatrix() * viewMatrix()).inverse() * Vector4(iNdc, 1);
    return r.xyz() / r.w();
}

//-----------------------------------------------------------------------------
const Vector3& Camera::upVector() const
{ return mUpVector; }

//-----------------------------------------------------------------------------
const Matrix4& Camera::viewMatrix() const
{
    return mViewMatrix;
}

//-----------------------------------------------------------------------------
const Viewport& Camera::viewport() const
{return mViewport;}

//-----------------------------------------------------------------------------
Vector3 Camera::worldToCamera(const Vector3& iP) const
{
    return (viewMatrix() * Vector4(iP, 1)).xyz();
}

//-----------------------------------------------------------------------------
Vector3 Camera::worldDeltaToCamera(const Vector3& iV) const
{
    return (viewMatrix() * Vector4(iV, 0)).xyz();
}

//-----------------------------------------------------------------------------
// Convert a 3d world position to screen coordinates.
//
// Note: Qt y axis is inverted from openGL, so if the result is to be displayed
//  in a qt window, the y axis will need to be inverted
//
Vector2 Camera::worldToScreen(const Vector3& iP) const
{
    Vector3 ndc = projectToNdc(iP);
    
    // from range [-1, 1] to [0, 1]
    ndc = (ndc + Vector3(1)) / 2.0;
    
    // range [0, 1] multiplied by viewport to get screen coordinates
    const Viewport& v = viewport();
    
    return Vector2(ndc.x() * v.width(),
                   ndc.y() * v.height() );
}

//-----------------------------------------------------------------------------
Vector2 Camera::worldDeltaToSreen(const Vector3& iV) const
{
    Vector2 v0 = worldToScreen(Vector3(0.0));
    Vector2 v1 = worldToScreen(iV);
    return v1 - v0;
}

//-----------------------------------------------------------------------------
double Camera::zoomFactor() const
{
    return mZoomFactor;
}