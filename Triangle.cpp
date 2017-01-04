#include "Triangle.h"

Triangle::Triangle()
{
	m_vertices[0].m_position = Vector3(-1.0, 0.0, -5.0);
	m_vertices[1].m_position = Vector3(0.0, 1.0, -5.0);
	m_vertices[2].m_position = Vector3(1.0, 0.0, -5.0);
	m_vertices[0].m_normal = Vector3(0.0, 0.0, 1.0);
	m_primtype = PRIMTYPE_Triangle;
}

Triangle::Triangle(Vector3 pos1, Vector3 pos2, Vector3 pos3)
{
	SetVertices(pos1, pos2, pos3);

	m_primtype = PRIMTYPE_Triangle;
}


Triangle::~Triangle()
{
}

/*void Triangle::SetTriangle(Vector3 v0, Vector3 v1, Vector3 v2)
{
	m_vertices[0] = v0;
	m_vertices[1] = v1;
	m_vertices[2] = v2;

	//Calculate Normal
	Vector3 NormalA = m_vertices[1] - m_vertices[0];
	Vector3 NormalB = m_vertices[2] - m_vertices[0];
	Vector3 Norm = NormalA.CrossProduct(NormalB);
	Norm.Length();
	m_normal = Norm;
} */

void Triangle::SetVertices(Vector3& v0, Vector3& v1, Vector3& v2)
{
	m_vertices[0].m_position = v0;
	m_vertices[1].m_position = v1;
	m_vertices[2].m_position = v2;
}

void Triangle::SetNormals(Vector3& n0, Vector3& n1, Vector3& n2)
{
	m_vertices[0].m_normal = n0;
	m_vertices[1].m_normal = n1;
	m_vertices[2].m_normal = n2;
}

void Triangle::SetTexCoords(Vector3& t0, Vector3& t1, Vector3& t2)
{
	m_vertices[0].m_texcoords = t0;
	m_vertices[1].m_texcoords = t1;
	m_vertices[2].m_texcoords = t2;
}

Vector3 Triangle::GetBarycentricCoords(Vector3& point)
{
	Vector3 barycoord;

	return barycoord;
}

RayHitResult Triangle::IntersectByRay(Ray& ray)
{
	RayHitResult result = Ray::s_defaultHitResult;
	double t = FARFAR_AWAY;

	Vector3 e1, e2;  //Edge1, Edge2
	Vector3 P, Q, T;
	float det, inv_det, u, v;

	//Find vectors for two edges sharing m_vertices[0]
	e1 = m_vertices[1].m_position - m_vertices[0].m_position;
	e2 = m_vertices[2].m_position - m_vertices[0].m_position;
	//Begin calculating determinant - also used to calculate u parameter
	P = ray.GetRay().CrossProduct(e2);
	//if determinant is near zero, ray lies in plane of triangle
	det = e1.DotProduct(P);
	//NOT CULLING
	inv_det = 1.f / det;

	//calculate distance from m_vertices[0] to ray origin
	T = ray.GetRayStart() - m_vertices[0].m_position;

	//Calculate u parameter and test bound
	u = T.DotProduct(P) * inv_det;
	//The intersection lies outside of the triangle
	if (u < 0.f || u > 1.f) return result;

	//Prepare to test v parameter
	Q = T.CrossProduct(e1);

	//Calculate V parameter and test bound
	v = ray.GetRay().DotProduct(Q) * inv_det;
	//The intersection lies outside of the triangle
	if (v < 0.f || u + v  > 1.f) return result;

	t = e2.DotProduct(Q) * inv_det;
	
	if (t > 0) { //ray intersection

		result.t = t;
		
		result.point = ray.GetRayStart() + ray.GetRay()*t;

		Vector3 texcoords;
		Vector3 normal;
		Vector3 bc_coords = GetBarycentricCoords(result.point);

		texcoords =
			m_vertices[0].m_texcoords*bc_coords[0] +
			m_vertices[1].m_texcoords*bc_coords[1] +
			m_vertices[2].m_texcoords*bc_coords[2];

		normal =
			m_vertices[0].m_normal*bc_coords[0] +
			m_vertices[1].m_normal*bc_coords[1] +
			m_vertices[2].m_normal*bc_coords[2];
		
		normal.Normalise();

		result.normal = normal;
 		result.data = this;
		return result;
	}


	return result;
}
