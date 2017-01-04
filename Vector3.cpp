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
#include "Vector3.h"

Vector3::Vector3()
{
	m_element[0] = 0.0f;
	m_element[1] = 0.0f;
	m_element[2] = 0.0f;
}

Vector3::Vector3(const Vector3& rhs)
{
	m_element[0] = rhs[0];
	m_element[1] = rhs[1];
	m_element[2] = rhs[2];
}

Vector3::Vector3(float x, float y, float z)
{
	m_element[0] = x; m_element[1] = y; m_element[2] = z;
}

float Vector3::operator [] (const int i) const
{
	return m_element[i];
}

float& Vector3::operator [] (const int i)
{
	return m_element[i];
}

Vector3 Vector3::operator + (const Vector3& rhs) const
{
	return Vector3(
		(*this)[0] + rhs[0],
		(*this)[1] + rhs[1],
		(*this)[2] + rhs[2]);
}

Vector3 Vector3::operator - (const Vector3& rhs) const
{
	return Vector3(
		(*this)[0] - rhs[0],
		(*this)[1] - rhs[1],
		(*this)[2] - rhs[2]);
}

Vector3 Vector3::operator = (const Vector3& rhs)
{
	m_element[0] = rhs[0];
	m_element[1] = rhs[1];
	m_element[2] = rhs[2];
	return *this;
}

Vector3 Vector3::operator * (const Vector3& rhs) const
{
	return Vector3(
		m_element[0] * rhs[0],
		m_element[1] * rhs[1],
		m_element[2] * rhs[2]);
}

Vector3 Vector3::operator * (float scale) const
{
	return Vector3(
		m_element[0] * scale,
		m_element[1] * scale,
		m_element[2] * scale
		);
}

float Vector3::Norm() const
{
	return sqrt(m_element[0] * m_element[0] + m_element[1] * m_element[1] + m_element[2] * m_element[2]);
}

float Vector3::Norm_Sqr() const
{
	return m_element[0] * m_element[0] + m_element[1] * m_element[1] + m_element[2] * m_element[2];
}

float Vector3::DotProduct(const Vector3& rhs) const
{
	return m_element[0] * rhs[0] + m_element[1] * rhs[1] + m_element[2] * rhs[2];
}

Vector3 Vector3::Normalise()
{
	float length = this->Norm();

	if (length > 1.0e-8f)
	{
		float invLen = 1.0f / length;

		m_element[0] *= invLen;
		m_element[1] *= invLen;
		m_element[2] *= invLen;
	}

	return *this;
}

Vector3 Vector3::CrossProduct(const Vector3& rhs) const
{
	return Vector3(
		(m_element[1] * rhs[2] - m_element[2] * rhs[1]),
		(m_element[2] * rhs[0] - m_element[0] * rhs[2]),
		(m_element[0] * rhs[1] - m_element[1] * rhs[0])
		);
}

Vector3 Vector3::Reflect(const Vector3 & n) const
{
	Vector3 result;
	
	float IndotN = -2.0*this->DotProduct(n);

	result = *this + n*IndotN;

	return result;
}

Vector3 Vector3::Refract(const Vector3 & n, float r_index) const
{
	Vector3 result;
	result.SetZero();
	float IndotN = this->DotProduct(n);

	if (IndotN > 0.0)
	{
		Vector3 nn = n*(-1.0);
		IndotN = -this->DotProduct(nn);
	}
	else
		IndotN = -IndotN;

	float k = 1.0 - r_index*r_index*(1.0 - IndotN*IndotN);

	if (k >= 0.0)
		result = (*this)*r_index + n*(r_index*IndotN - sqrt(k));
	else
		result.SetZero();

	return result;
}

void Vector3::SetZero()
{
	m_element[0] = m_element[1] = m_element[2] = 0.0f;
}
