/*---------------------------------------------------------------------
*
* Copyright © 2015  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#include "Light.h"


Light::Light()
{
	InitDefaultLight();
}


Light::~Light()
{
}

void Light::InitDefaultLight()
{
	//set default position and colour for a light
	SetLightColour(1.0, 1.0, 1.0);
	SetLightPosition(0.0, 20.0, 0.0);
}

void Light::SetLightColour(double r, double g, double b)
{
	mColour.SetVector(r, g, b);
}

void Light::SetLightPosition(double x, double y, double z)
{
	mPosition.SetVector(x, y, z);
}
