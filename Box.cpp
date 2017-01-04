#include "Box.h"


Box::Box()
{
	SetBox(Vector3(0.0, 0.0, 0.0), 1, 1, 1);
	m_primtype = Primitive::PRIMTYPE_Box;
}


Box::~Box()
{
}

Box::Box(Vector3 position, double width, double height, double depth)
{
	SetBox(position, width, height, depth);
	m_primtype = Primitive::PRIMTYPE_Box;
}

void Box::SetBox(Vector3 position, double width, double height, double depth)
{
	double halfwidth = width*0.5;
	double halfheight = height*0.5;
	double halfdepth = depth*0.5;
	
	Vector3 tempVerts[8];

	tempVerts[0].SetVector(-halfwidth + position[0], -halfheight + position[1], halfdepth + position[2]);
	tempVerts[1].SetVector(halfwidth + position[0], -halfheight + position[1], halfdepth + position[2]);
	tempVerts[2].SetVector(halfwidth + position[0], halfheight + position[1], halfdepth + position[2]);
	tempVerts[3].SetVector(-halfwidth + position[0], halfheight + position[1], halfdepth + position[2]);
	
	tempVerts[4].SetVector(-halfwidth + position[0], -halfheight + position[1], -halfdepth + position[2]);
	tempVerts[5].SetVector(halfwidth + position[0], -halfheight + position[1], -halfdepth + position[2]);
	tempVerts[6].SetVector(halfwidth + position[0], halfheight + position[1], -halfdepth + position[2]);
	tempVerts[7].SetVector(-halfwidth + position[0], halfheight + position[1], -halfdepth + position[2]);

	m_triangles[0].SetVertices(tempVerts[0], tempVerts[1], tempVerts[2]);
	
	m_triangles[1].SetVertices(tempVerts[0], tempVerts[2], tempVerts[3]);
	
	m_triangles[2].SetVertices(tempVerts[1], tempVerts[6], tempVerts[2]);
	
	m_triangles[3].SetVertices(tempVerts[1], tempVerts[5], tempVerts[6]);
	
	m_triangles[4].SetVertices(tempVerts[0], tempVerts[3], tempVerts[7]);
	
	m_triangles[5].SetVertices(tempVerts[0], tempVerts[7], tempVerts[4]);
	
	m_triangles[6].SetVertices(tempVerts[4], tempVerts[7], tempVerts[6]);
	
	m_triangles[7].SetVertices(tempVerts[4], tempVerts[6], tempVerts[5]);
	
	m_triangles[8].SetVertices(tempVerts[3], tempVerts[6], tempVerts[7]);
	
	m_triangles[9].SetVertices(tempVerts[3], tempVerts[2], tempVerts[6]);
	
	m_triangles[10].SetVertices(tempVerts[0], tempVerts[4], tempVerts[5]);
	
	m_triangles[11].SetVertices(tempVerts[0], tempVerts[5], tempVerts[1]);
	
}

RayHitResult Box::IntersectByRay(Ray& ray)
{
	RayHitResult result = Ray::s_defaultHitResult;

	for (int i = 0; i < 12; i++)
	{
		Primitive* prim = static_cast<Primitive*>(&m_triangles[i]);

		RayHitResult tempresult = prim->IntersectByRay(ray);
		
		if (tempresult.t < result.t)
		{
			Vector3 v01 = m_triangles[i].m_vertices[1].m_position - m_triangles[i].m_vertices[0].m_position;
			Vector3 v02 = m_triangles[i].m_vertices[2].m_position - m_triangles[i].m_vertices[0].m_position;

			tempresult.normal = v01.CrossProduct(v02).Normalise();
			tempresult.data = this;
			result = tempresult;
		}

	}

	return result;
}
