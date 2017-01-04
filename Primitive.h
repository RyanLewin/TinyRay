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

#include "Ray.h"

class Material;


//An abstract class representing a basic primitive in TinyRay
class Primitive
{
	private:
		Material				*m_pMaterial;		//pointer to the material associated to the primitive
		
	public:
		//enum for primitive types
		enum PRIMTYPE
		{
			PRIMTYPE_Plane = 0,	//plane
			PRIMTYPE_Sphere, //sphere
			PRIMTYPE_Triangle, //generic triangle
			PRIMTYPE_Box //box
		};

		PRIMTYPE				m_primtype; //primitive type

								Primitive(){ m_pMaterial = nullptr; }
		virtual					~Primitive(){ ; }


		virtual RayHitResult	IntersectByRay(Ray& ray) = 0;  //An interface for computing intersection between a ray and this primitve

		inline void				SetMaterial(Material* pMat)
		{
			m_pMaterial = pMat;
		}

		inline Material*		GetMaterial()
		{
			return m_pMaterial;
		}
};
