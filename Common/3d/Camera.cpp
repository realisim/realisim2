

#include "3d/Camera.h"
#include <iomanip>
#include <sstream>

using namespace Realisim;
using namespace Math;
using namespace TreeD;

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
    mIsProjectionProportionalToViewport(false),
	mViewport(),
	mProjectionMatrix(),
	mViewMatrix()
{
	set(Vector3(0, 0, 100),
		Vector3(0, 0, 0),
		Vector3(0, 1, 0));
    Projection p;
    p.setPerspectiveProjection(60, 1, 0.5, 10000.0);
    setProjection(p, true);
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
	if (isProjectionProportionalToViewport())
	{
		/*Quand on veut une projection proportionnelle, on commence par trouver
		la hauteur (h) proportionnel a la largeur (w) courante. Ensuite, on
		trouve le centre (cy) de la projection et y ajoute la moitié de h
		de part et d'autre.*/
		double w = viewport().width() / p.width();
		double h = viewport().height() / w / 2.0;
		double cy = p.bottom() + p.height() / 2.0;
		mProjection.setTop( cy + h );
		mProjection.setBottom( cy - h );
	}

	/*Afin d'appliquer correctement le zoom, qui est donné par projectionWidthWithoutZoom
    on doit trouver le centre de la projection
	originale (sans zoom). Ensuite on détermine la taille du rectangle
	de la projection avec le zoom a partir de ce centre.*/
	double halfVisibleWidth = projectionWidthWithoutZoom() / 2.0;
	double halfVisibleHeigh = projectionHeightWithoutZoom() / 2.0;
    double cx = 0.0, cy = 0.0, l, r, b, t;
	cx = p.left() + p.width() / 2.0;
	cy = p.bottom() + p.height() / 2.0;
	l = cx - halfVisibleWidth;
	r = cx + halfVisibleWidth;
	b = cy - halfVisibleHeigh;
	t = cy + halfVisibleHeigh;
    const double f = p.farPlane();
    const double n = p.nearPlane();

	switch (p.type())
	{
	case Projection::tOrthogonal:
	{
		double m[4][4] = {
		{ 2.0 / (r - l), 0.0, 0.0, -(r + l) / (r - l) },
		{ 0.0, 2.0 / (t - b), 0.0, -(t + b) / (t - b) },
		{ 0.0, 0.0, -2.0 / (f - n), -(f + n) / (f - n) },
		{ 0, 0, 0, 1.0 },
		};
		mProjectionMatrix = Matrix4(m[0]);
	}	break;
	case Projection::tPerspective:
	{
		double m[4][4] = {
		{ (2.0*n) / (r - l), 0.0, (r + l) / (r - l), 0.0 },
		{ 0.0, (2.0*n) / (t - b), (t + b) / (t - b), 0.0 },
		{ 0, 0, -(f + n) / (f - n), (-2 * f * n) / (f - n) },
		{ 0.0, 0.0, -1, 0.0 },
		};
		mProjectionMatrix = Matrix4(m[0]);
	} break;
	default: break;
	}
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
bool Camera::isProjectionProportionalToViewport() const
{ return mIsProjectionProportionalToViewport; }

//-----------------------------------------------------------------------------
const Vector3& Camera::lateralVector() const
{ return mLateralVector; }

//-----------------------------------------------------------------------------
const Vector3& Camera::lookAt() const
{ return mLookAt; }

//-----------------------------------------------------------------------------
const Vector3& Camera::lookVector() const
{ return mLookVector; }

//-----------------------------------------------------------------------------
const Vector3& Camera::position() const
{ return mPosition; }

//-----------------------------------------------------------------------------
const Projection& Camera::projection() const
{return mProjection;}

//-----------------------------------------------------------------------------
/*retourne la largeur visible en unité GL de la projection. En
 gros, on enleve le zoom qui a été appliqué sur la projection */
double Camera::projectionHeightWithoutZoom() const
{
    return mProjection.height() * 1.0 / zoomFactor();
}

//-----------------------------------------------------------------------------
const Matrix4& Camera::projectionMatrix() const
{
    return mProjectionMatrix;
}

//-----------------------------------------------------------------------------
// Projects the 3d world coordinate to ndc
//
Vector3 Camera::projectToNdc(const Math::Vector3& iWorld) const
{
    Vector4 ndc = projectionMatrix() * viewMatrix() * Vector4(iWorld, 1);
    return ndc.xyz() / ndc.w();
}

//-----------------------------------------------------------------------------
double Camera::projectionWidthWithoutZoom() const
{
    return mProjection.width() * 1.0 / zoomFactor();
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
//              	//Invert y axis since Qt and openGL are inverted on y...
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
/*Définie la position de la caméra, le point visé et le vecteur up. Le
  vecteur Latéral sera calculé à partir du vecteur up et ensuite le up
  sera recalculé afin d'assurer une base normale.*/
void Camera::set(const Vector3& iEye,
	const Vector3& iLookAt,
	const Vector3& iUpVector)
{
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
void Camera::setProjection(const Projection& iP, bool iProportional)
{
	mProjection = iP;
    mIsProjectionProportionalToViewport = iProportional;
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
void Camera::translate(const Vector3& iV)
{
    set(mPosition + iV, mLookAt + iV, upVector());
}

//-----------------------------------------------------------------------------
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