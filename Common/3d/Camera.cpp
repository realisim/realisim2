

#include "3d/Camera.h"
#include <iostream>

using namespace Realisim;
using namespace Math;
using namespace TreeD;

//-----------------------------------------------------------------------------
//--- Camera
//-----------------------------------------------------------------------------
Camera::Camera() :
	mPos(),
	mLat(),
	mLook(),
	mLookVector(),
	mUp(),
	mProjection(),
    mIsProjectionProportionalToViewport(false),
	mViewport(),
	mProjectionMatrix(),
	mViewMatrix()
{
	set(Vector3(0, 0, -100),
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
	return (getViewMatrix().inverse() * Vector4(iP, 1)).xyz();
}
//-----------------------------------------------------------------------------
Vector3 Camera::cameraDeltaToWorld(const Vector3& iV) const
{
	return (getViewMatrix().inverse() * Vector4(iV, 0)).xyz();
}
//-----------------------------------------------------------------------------
void Camera::computeProjection()
{
	const Projection& p = getProjection();
	if (isProjectionProportionalToViewport())
	{
		/*Quand on veut une projection proportionnelle, on commence par trouver
		la hauteur (h) proportionnel a la largeur (w) courante. Ensuite, on
		trouve le centre (cy) de la projection et y ajoute la moitié de h
		de part et d'autre.*/
		double w = getViewport().width() / p.width();
		double h = getViewport().height() / w / 2.0;
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
	Matrix4 viewMatrix(mLat, mUp, -mLookVector);
	Matrix4 t(-getPos());
	mViewMatrix = viewMatrix.transpose() * t;
}

//-----------------------------------------------------------------------------
const Vector3& Camera::getLookVector() const
{
	return mLookVector;
}

//-----------------------------------------------------------------------------
const Matrix4& Camera::getViewMatrix() const
{
	return mViewMatrix;
}

//-----------------------------------------------------------------------------
const Matrix4& Camera::getProjectionMatrix() const
{
	return mProjectionMatrix;
}

//-----------------------------------------------------------------------------
/*retourne la largeur visible en unité GL*/
double Camera::getVisibleHeight() const
{
	return mProjection.height() * 1.0 / getZoom();
}

//-----------------------------------------------------------------------------
double Camera::getVisibleWidth() const
{
	return mProjection.width() * 1.0 / getZoom();
}

//-----------------------------------------------------------------------------
bool Camera::isProjectionProportionalToViewport() const
{ return mIsProjectionProportionalToViewport; }

//-----------------------------------------------------------------------------
void Camera::print() const
{
	using namespace std;
	cout << "\n\nPos: " << mPos.x() << " " << mPos.y() << " " << mPos.z();
	cout << "\nLook: " << mLook.x() << " " << mLook.y() << " " << mLook.z();
	cout << "\nLat: " << mLat.x() << " " << mLat.y() << " " << mLat.z();
	cout << "\nUp: " << mUp.x() << " " << mUp.y() << " " << mUp.z();
}

//-----------------------------------------------------------------------------
void Camera::rotate(double iRad, Vector3 iAxe,
	Vector3 iAxisPos /*= Vector3()*/)
{
	Vector3 eye = getPos(), center = getLook();
	Vector3 up = getUp();

	Matrix4 r(iRad, iAxe);
	Matrix4 t(iAxisPos);
	r = t * r * t.inverse();

	eye = (r * Vector4(eye, 1)).xyz();
	center = (r * Vector4(center, 1)).xyz();
	up = (r * Vector4(up, 0)).xyz();
	set(eye, center, up);
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
	const Vector3& iCenter,
	const Vector3& iUp)
{
	mPos = iEye;
	mLook = iCenter;
	mUp = iUp;

	//on commence par calculer le vecteur laterale parce que le vecteur up
	//final est dependant du vecteur lateral
	mLookVector = getLook() - getPos();
	mLookVector.normalize();

	mLat = mLookVector ^ mUp;
	mLat.normalize();

	mUp = mLat ^ mLookVector;
	mUp.normalize();

	computeViewMatrix();
}

//-----------------------------------------------------------------------------
void Camera::setProjection(const Projection& iP, bool iProportional)
{
	mProjection = iP;
    mIsProjectionProportionalToViewport = iProportional;
    computeProjection();
}

////-----------------------------------------------------------------------------
//void Camera::setViewportSize(int iWidth, int iHeight)
//{
//	//early out
//	if (mViewport.getWidth() == iWidth && mViewport.getHeight() == iHeight)
//	{ return; }
//
//	if (iWidth >= iHeight)
//		mViewport.mOrientation = Viewport::oHorizontal;
//	else
//		mViewport.mOrientation = Viewport::oVertical;
//
//	//le coté le plus long montre la valeur mVisibleGLUnit unité GL
//	/*la valeur minimale pour les cote est de 1, on evite que la taille soit 0*/
//	mViewport.mShortSide = qMax(qMin(iWidth, iHeight), 1);
//	mViewport.mLongSide = qMax(qMax(iWidth, iHeight), 1);
//
//	computeProjection();
//}
////-----------------------------------------------------------------------------
//void Camera::setViewportSize(Vector2i iS)
//{
//	setViewportSize(iS.x(), iS.y());
//}

//-----------------------------------------------------------------------------
void Camera::setViewport( const Viewport& iV)
{
    mViewport = iV;
    computeProjection();
}

//-----------------------------------------------------------------------------
void Camera::setZoom(double iZoom)
{
	//mProjection.mZoomFactor = iZoom;
	computeProjection();
}
//-----------------------------------------------------------------------------
void Camera::translate(const Vector3& iV)
{
	Vector3 d = iV - getPos(); translate(d);
}
//-----------------------------------------------------------------------------
void Camera::translateTo(const Vector3& iV)
{
	set(mPos + iV, mLook + iV, getUp());
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