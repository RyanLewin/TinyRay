/*---------------------------------------------------------------------
*
* Copyright © 2015  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#pragma once

#include "Vector3.h"

#define FARFAR_AWAY  1000000.0			//let's hope this is reasonably large ;)

//A basic struct for recording a ray hit result
struct RayHitResult
{
	Vector3	normal;			// the surface normal at the intersection ( e.g. useful for lighting);
	Vector3 point;			// the exact position of the intersection point
	double t;				//the parametric value of the resulting intersections
	void* data;				//a pointer to misc. data, e.g. this could be material data for calculating lighting; or the hit object itself
};

class Ray
{

	private:
		Vector3				m_start;   //origin of the ray
		Vector3				m_ray;     //direct of the ray, this must be a unit vector

	public:
			static RayHitResult		s_defaultHitResult; //This is a constant for storing the default ray intersection result, i.e. nothing
			Ray();
			~Ray();

			inline void SetRay(Vector3 start, Vector3 ray)
			{
				m_start = start;
				m_ray = ray;
			}

			inline Vector3& GetRay()
			{
				return m_ray;
			}

			inline Vector3& GetRayStart()
			{
				return m_start;
			}
};

