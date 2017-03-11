

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

	/*Afin d'appliquer correctement le zoom, qui est donné par getVisibleWidth
	et getVisibleHeight, on doit trouver le centre de la projection
	originale (sans zoom). Ensuite on détermine la taille du rectangle
	de la projection avec le zoom a partir de ce centre.*/
	double halfVisibleWidth = getVisibleWidth() / 2.0;
	double halfVisibleHeigh = getVisibleHeight() / 2.0;
    double cx = 0.0, cy = 0.0, l, r, b, t;
	cx = p.left() + p.width() / 2.0;
	cy = p.bottom() + p.height() / 2.0;
	l = cx - halfVisibleWidth;
	r = cx + halfVisibleWidth;
	b = cy - halfVisibleHeigh;
	t = cy + halfVisibleHeigh;
    const double f = p.far();
    const double n = p.near();

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
/*retourne la largeur visible en unité GL*/
double Camera::getVisibleHeight() const
{
	return mProjection.height() * 1.0 / zoomFactor();
}

//-----------------------------------------------------------------------------
double Camera::getVisibleWidth() const
{
	return mProjection.width() * 1.0 / zoomFactor();
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
const Matrix4& Camera::projectionMatrix() const
{
    return mProjectionMatrix;
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
/*Convertie une position pixel a l'écran en coordonnée GL.
  Les paramètres sont en pixels. Puisque le systeme de fenetrage est Qt,
  contrairement a openGL, l'origine (0, 0) est dans le coin supérieur gauche.
  Ainsi la coordonné y est inversée. Le point converti sera en  coordonné
  globale. iPoint doit être en coordonné locale de la camera. Ce point
  représente la position d'un plan, parallele à la camera. Le Vector3 retourné
  par cette fonction est donc l'intersection de la projection du point iX, iY
  avec ce plan. La projection de caméra est tenue en compte.
  Note: cette methode applique les projections de projection et de modelview
	afin de faire les calculs.*/
Vector3 Camera::screenToWorld(Vector2 iP, const Vector3& iR /*=Vector3(0.0)*/) const
{
//	double x = iP.x();
//	//l'axe de Qt est inversé par rapport a openGL
//	double y = getViewport().getHeight() - iP.y();
//
//	pushAndApplyMatrices();
//
//	double modelView[16];
//	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
//	double projection[16];
//	glGetDoublev(GL_PROJECTION_MATRIX, projection);
//	int viewport[4];
//	glGetIntegerv(GL_VIEWPORT, viewport);
//
//	double winx, winy, winz, p0x, p0y, p0z;
//
//	/*A partir du Vector3 qui represente le point sur le plan de travail
//	  perpendiculaire a la camera, on le projete a l'écran. Ce qui nous
//	  intéresse c'est la coordonnée en z normalisé qui nous donne la profondeur
//	  de ce Vector3 dans le zBuffer qui sera utilisé pour trouver le point
//	  en coordonnée GL*/
//	gluProject(iR.x(), iR.y(), iR.z(),
//		modelView, projection, viewport,
//		&winx, &winy, &winz);
//
//	/*On projete le point d'écran iX, iY, winz qui nous donnera un point (p0)
//	  qui correspond au point d'écran (iX, iY) a la profondeur du plan de
//	  travail. */
//	gluUnProject(x, y, winz,
//		modelView, projection, viewport,
//		&p0x, &p0y, &p0z);
//
//	popMatrices();
//	return Vector3(p0x, p0y, p0z);
    return Vector3();
}

//-----------------------------------------------------------------------------
Vector3 Camera::screenDeltaToWorld(Vector2 iV, const Vector3& iP/*=Vector3(0.0)*/) const
{
	Vector3 p0 = screenToWorld(Vector2(0.0), iP);
	Vector3 p1 = screenToWorld(iV, iP);
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
	//mProjection.mZoomFactor = iZoom;
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
Vector2 Camera::worldToSreen(const Vector3& iP) const // comme glToPixel
{
//	pushAndApplyMatrices();
//
//	double modelView[16];
//	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
//	double projection[16];
//	glGetDoublev(GL_PROJECTION_MATRIX, projection);
//	int viewport[4];
//	glGetIntegerv(GL_VIEWPORT, viewport);
//
//	double x, y, z;
//	bool sucess = gluProject(iP.x(), iP.y(), iP.z(),
//		modelView,
//		projection,
//		viewport,
//		&x, &y, &z);
//	if (!sucess) { x = 0; y = 0; z = 0; }
//
//	popMatrices();
//
//	//dépendant de l'orientation de la camera?
//	return Vector2(x, y);
    return Vector2();
}

//-----------------------------------------------------------------------------
Vector2 Camera::worldDeltaToSreen(const Vector3& iV) const
{
	Vector2 v0 = worldToSreen(Vector3(0.0));
	Vector2 v1 = worldToSreen(iV);
	return v1 - v0;
}