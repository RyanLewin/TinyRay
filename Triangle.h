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
#include "Primitive.h"
#include "Vector3.h"
#include "Ray.h"
#include <vector>

//Struct representing a 3D vertex
typedef struct
{
	Vector3 m_position;     //position of the vertex
	Vector3 m_normal;		//normal to the vertex
	Vector3 m_texcoords;	//texture coordinates for texture mapping
} Vertex;

class Triangle : public Primitive
{
	private:

	public:
		Vertex	m_vertices[3];

		Triangle();
		Triangle(Vector3 pos1, Vector3 pos2, Vector3 pos3);
		~Triangle();
	
		void SetVertices(Vector3& v0, Vector3& v1, Vector3& v2);
		void SetNormals(Vector3& n0, Vector3& n1, Vector3& n2);
		void SetTexCoords(Vector3& t0, Vector3& t1, Vector3& t2);

		//Method for calculating the Barycentric coordinates of a input point
		Vector3 GetBarycentricCoords(Vector3& point);

		RayHitResult IntersectByRay(Ray& ray);
};

