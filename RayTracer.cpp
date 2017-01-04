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
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#if defined(WIN32) || defined(_WINDOWS)
#include <Windows.h>
#include <gl/GL.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#endif

#include "RayTracer.h"
#include "Ray.h"
#include "Scene.h"
#include "Camera.h"
#include "perlin.h"

RayTracer::RayTracer()
{
	m_buffHeight = m_buffWidth = 0.0;
	m_renderCount = 0;
	SetTraceLevel(5);
	m_traceflag = (TraceFlags)(TRACE_AMBIENT | TRACE_DIFFUSE_AND_SPEC |
		TRACE_SHADOW | TRACE_REFLECTION | TRACE_REFRACTION);

}

RayTracer::RayTracer(int Width, int Height)
{
	m_buffWidth = Width;
	m_buffHeight = Height;
	m_renderCount = 0;
	SetTraceLevel(5);

	m_framebuffer = new Framebuffer(Width, Height);

	//default set default trace flag, i.e. no lighting, non-recursive
	m_traceflag = (TraceFlags)(TRACE_AMBIENT);
}

RayTracer::~RayTracer()
{
	delete m_framebuffer;
}

void RayTracer::DoRayTrace(Scene* pScene)
{
	Camera* cam = pScene->GetSceneCamera();

	Vector3 camRightVector = cam->GetRightVector();
	Vector3 camUpVector = cam->GetUpVector();
	Vector3 camViewVector = cam->GetViewVector();
	Vector3 centre = cam->GetViewCentre();
	Vector3 camPosition = cam->GetPosition();

	double sceneWidth = pScene->GetSceneWidth();
	double sceneHeight = pScene->GetSceneHeight();

	double pixelDX = sceneWidth / m_buffWidth;
	double pixelDY = sceneHeight / m_buffHeight;

	int total = m_buffHeight*m_buffWidth;
	int done_count = 0;

	Vector3 start;

	start[0] = centre[0] - ((sceneWidth * camRightVector[0])
		+ (sceneHeight * camUpVector[0])) / 2.0;
	start[1] = centre[1] - ((sceneWidth * camRightVector[1])
		+ (sceneHeight * camUpVector[1])) / 2.0;
	start[2] = centre[2] - ((sceneWidth * camRightVector[2])
		+ (sceneHeight * camUpVector[2])) / 2.0;

	Colour scenebg = pScene->GetBackgroundColour();

	if (m_renderCount == 0)
	{
		fprintf(stdout, "Trace start.\n");

		Colour colour;
		//TinyRay on multiprocessors using OpenMP!!!
#pragma omp parallel for schedule (dynamic, 1) private(colour)
		for (int i = 0; i < m_buffHeight; i += 1) {
			for (int j = 0; j < m_buffWidth; j += 1) {

				//calculate the metric size of a pixel in the view plane (e.g. framebuffer)
				Vector3 pixel;

				pixel[0] = start[0] + (i + 0.5) * camUpVector[0] * pixelDY
					+ (j + 0.5) * camRightVector[0] * pixelDX;
				pixel[1] = start[1] + (i + 0.5) * camUpVector[1] * pixelDY
					+ (j + 0.5) * camRightVector[1] * pixelDX;
				pixel[2] = start[2] + (i + 0.5) * camUpVector[2] * pixelDY
					+ (j + 0.5) * camRightVector[2] * pixelDX;

				/*
				* setup first generation view ray
				* In perspective projection, each view ray originates from the eye (camera) position
				* and pierces through a pixel in the view plane
				*/
				Ray viewray;
				viewray.SetRay(camPosition, (pixel - camPosition).Normalise());

				double u = (double)j / (double)m_buffWidth;
				double v = (double)i / (double)m_buffHeight;

				scenebg = pScene->GetBackgroundColour();

				//trace the scene using the view ray
				//default colour is the background colour, unless something is hit along the way
				colour = this->TraceScene(pScene, viewray, scenebg, m_traceLevel);

				/*
				* Draw the pixel as a coloured rectangle
				*/
				m_framebuffer->WriteRGBToFramebuffer(colour, j, i);
			}
		}

		fprintf(stdout, "Done!!!\n");
		m_renderCount++;
	}
}

Colour RayTracer::TraceScene(Scene* pScene, Ray& ray, Colour incolour, int tracelevel, bool shadowray)
{
	RayHitResult result;

	Colour outcolour = incolour; //the output colour based on the ray-primitive intersection

	std::vector<Light*> *light_list = pScene->GetLightList();
	Vector3 cameraPosition = pScene->GetSceneCamera()->GetPosition();

	//Intersect the ray with the scene
	//TODO: Scene::IntersectByRay needs to be implemented first
	result = pScene->IntersectByRay(ray);

	tracelevel--;


	if (result.data) //the ray has hit something
	{
		//TODO:
		//1. Non-recursive ray tracing:
		//	 When a ray intersect with an objects, determine the colour at the intersection point
		//	 using CalculateLighting
		outcolour = CalculateLighting(light_list, &cameraPosition, &result);
		Colour diffuseColour = ((Primitive*)result.data)->GetMaterial()->GetDiffuseColour();

		//TODO: 2. The following conditions are for implementing recursive ray tracing
		if (m_traceflag & TRACE_REFLECTION)
		{
			//If the hit objects spec colour is greater than zero
			if (((Primitive*)result.data)->GetMaterial()->GetSpecularColour().Norm() > 0)
			{
				//Causes the TraceScene method to be called until the tracelevel is 0
				if (tracelevel >= 1)
				{
					//TODO: trace the reflection ray from the intersection point
					//Calculate the reflection vector
					Vector3 reflection = ray.GetRay().Reflect(result.normal.Normalise());

					//Create a ray that goes from just before the hit point along the reflection vector
					Ray reflect;
					reflect.SetRay(result.point + reflection * 0.0001, reflection);
					//Trace the scene again to find the colour of the reflect ray and also mix with the hit objects colour
					Colour reflectColour = TraceScene(pScene, reflect, outcolour, tracelevel) *  (diffuseColour + outcolour);

					outcolour = reflectColour + outcolour;
				}
			}
		}

		if (m_traceflag & TRACE_REFRACTION)
		{
			//TODO: trace the refraction ray from the intersection point
			if (((Primitive*)result.data)->m_primtype != Primitive::PRIMTYPE_Plane)
			{
				//Causes the TraceScene method to be called until the tracelevel is 0
				if (tracelevel >= 1)
				{
					//Calculate the refraction ray
					Vector3 refraction;


					//Check if the ray is on the inside or outside of the object, if its on the
					//outside it need to do the reverse of the refraction
					if (tracelevel % 2)
						refraction = ray.GetRay().Refract(result.normal.Normalise(), (1.1));
					else
						refraction = ray.GetRay().Refract(result.normal.Normalise(), 1 / 1.1);

					//Create the ray that points from the hit through the refraction vector
					Ray refract;
					refract.SetRay(result.point + refraction * 0.0001, refraction);

					//Trace  the scene to find the refract ray and mix with the diffuse colour
					Colour refractColour = TraceScene(pScene, refract, outcolour, tracelevel);
					outcolour = refractColour + (diffuseColour + outcolour) * 0.15;
				}
			}
		}

		if (m_traceflag & TRACE_SHADOW)
		{
			for (int i = 0; i < light_list->size(); i++)
			{
				//Get the light direction
				Vector3 light = (*light_list)[i]->GetLightPosition() - result.point;
				Ray shadow;

				//Create a ray between the result and the light
				shadow.SetRay(result.point + light * 0.0001, light.Normalise());
				//Find distance between the hitpoint and the light source
				double dist = sqrt(pow(result.point[0] - (*light_list)[i]->GetLightPosition()[0], 2) + pow(result.point[1] - (*light_list)[i]->GetLightPosition()[0], 2) + pow(result.point[2] - (*light_list)[i]->GetLightPosition()[0], 2));

				//If the ray's intersection value is below the distance
				RayHitResult shadowRay = pScene->IntersectByRay(shadow);

				if (shadowRay.t < dist)
				{
					//Cast shadow if an object is hit on way back to light
					if (((Primitive*)shadowRay.data)->GetMaterial()->CastShadow() && ((Primitive*)shadowRay.data))
						outcolour = outcolour * 0.25;
				}
			}
		}
	}

	return outcolour;
}

Colour RayTracer::CalculateLighting(std::vector<Light*>* lights, Vector3* campos, RayHitResult* hitresult)
{
	Colour outcolour;
	std::vector<Light*>::iterator lit_iter = lights->begin();

	Primitive* prim = (Primitive*)hitresult->data;
	Material* mat = prim->GetMaterial();
	Colour colourDiffuse;

	colourDiffuse = mat->GetAmbientColour();
	if (m_traceflag & TRACE_DIFFUSE_AND_SPEC)
		colourDiffuse = mat->GetDiffuseColour();

	//Generate the grid pattern on the plane
	if (((Primitive*)hitresult->data)->m_primtype == Primitive::PRIMTYPE_Plane)
	{
		int dx = hitresult->point[0] / 2.0;
		int dy = hitresult->point[1] / 2.0;
		int dz = hitresult->point[2] / 2.0;

		if (dx % 2 || dy % 2 || dz % 2)
		{
			colourDiffuse = Vector3(0.1, 0.1, 0.1);
		}
		else
		{
			colourDiffuse = mat->GetDiffuseColour();
		}
	}

	////Go through all lights in the scene
	////Note the default scene only has one light source
	if (m_traceflag & TRACE_DIFFUSE_AND_SPEC)
	{
		//TODO: Calculate and apply the lighting of the intersected object using the illumination
		//model covered in the lecture
		//i.e. diffuse using Lambertian model, for specular, you can use either
		//Phong or Blinn-Phong model

		for (int i = 0; i < lights->size(); i++)
		{
			// A vector that holds the direction from the light to the hitpoint
			Vector3 lightDir = lit_iter[i]->GetLightPosition() - hitresult->point;

			//Find cos theta for the Diffuse colour
			float cosTheta = hitresult->normal.Normalise().DotProduct(lightDir.Normalise());

			//Get camera drection and the reflection so as to be able to calculate phi
			Vector3 camDir = campos->Normalise();
			Vector3 reflection = (lightDir * -1).Normalise().Reflect(hitresult->normal.Normalise());
			float phi = camDir.DotProduct(reflection);

			//Make sure phi is within the range of 0 and 1
			if (phi < 0)
				phi = 0;
			else if (phi > 1)
				phi = 1;

			float specPower = mat->GetSpecPower();

			//Calculate diffuse and spec colour seperately as refraction and reflection don't require the diffuse
			Colour diffuseCol = (colourDiffuse * lit_iter[i]->GetLightColour() * cosTheta);
			Colour specCol = (mat->GetSpecularColour() * lit_iter[i]->GetLightColour() * pow(phi, specPower));
			Colour colour;

			//If the object is specular then set the colour to the specular calculation else Calculate 
			//colour by the diffuse calculation + the specular calculation
			if (((m_traceflag & TRACE_REFLECTION) && mat->GetSpecularColour().Norm() > 0.0) || ((m_traceflag & TRACE_REFRACTION) && ((Primitive*)hitresult->data)->m_primtype != Primitive::PRIMTYPE_Plane))
				colour = specCol;
			else
				colour = diffuseCol + specCol;

			//Add the colours in case there are multiple lights
			outcolour = outcolour + colour;
		}
	}
	else
	{
		outcolour = colourDiffuse;
	}

	return outcolour;
}