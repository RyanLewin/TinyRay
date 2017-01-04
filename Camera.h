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

class Camera
{
	private:
		Vector3				m_position;			//aka eye.
		Vector3				m_upVector;
		Vector3				m_viewVector;
		Vector3				m_rightVector;
		Vector3				m_viewCentre;		//centre of the near view plane
		double				m_focalLength;		//you can think of this as the distance between the eye and the image plane (framebuffer)

	public:
		
		Camera();
		~Camera();

		void InitDefaultCamera();

		//Setup the camera properties based on its position and lookat point
		//Param:	const Vector3& pos, this position of the camera
		//			const Vector3& lookat, the location at which the camera is pointing
		void SetPositionAndLookAt( const Vector3& pos, const Vector3& lookat);
		
		inline Vector3&		GetPosition() 
		{
			return m_position;
		}

		inline Vector3&		GetUpVector() 
		{
			return m_upVector;
		}

		inline Vector3&		GetRightVector() 
		{
			return m_rightVector;
		}

		inline Vector3&		GetViewVector() 
		{
			return m_viewVector;
		}

		inline Vector3&		GetViewCentre() 
		{
			return m_viewCentre;
		}

		inline double		GetFocalLength() 
		{
			return m_focalLength;
		}
};

