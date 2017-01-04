/*---------------------------------------------------------------------
*
* Copyright © 2015  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#include <math.h>
#include "Plane.h"


Plane::Plane()
{
	//The default plane is the xz plane with normal pointing towards positive y
	// the offset is zero,
	m_normal.SetVector(0.0, 1.0, 0.0);
	m_offset = 0.0;
	m_primtype = PRIMTYPE_Plane;
}


Plane::~Plane()
{
}

RayHitResult Plane::IntersectByRay(Ray& ray)
{
	RayHitResult result = Ray::s_defaultHitResult;
	double t = FARFAR_AWAY;

	double d = m_offset;

	Vector3 r = ray.GetRay();
	double ndotr = m_normal.DotProduct(r);
	double sdotn = ray.GetRayStart().DotProduct(m_normal);
	if (fabs(ndotr) < 1e-5)
		return result;


	t = -(sdotn + d) / ndotr;
	
	result.normal = m_normal;
	result.t = t;
	result.data = this;
	Vector3 intersection_point = ray.GetRayStart() + r*result.t;
	result.point = intersection_point;

	return result;
}

void Plane::SetPlane(const Vector3& normal, double offset)
{
	m_normal = normal;
	m_offset = -offset;
}
