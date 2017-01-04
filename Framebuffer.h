/*---------------------------------------------------------------------
*
* Copyright © 2016  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#pragma once

#include "Material.h"


//This class represent a RGBA colour framebuffer
class Framebuffer
{
private:
	int	mWidth;					//the width of framebuffer
	int mHeight;				//the height of framebuffer
	Colour *mColourBuffer;	//Storage for RGBA pixels as a linear array

	//Method for initialise the framebuffer
	//input:	int width --- width of the buffer to be created
	//			int height --- height of the buffer to be created
	void InitFramebuffer(int width, int height);

	Framebuffer();

public:
	Framebuffer(int width, int height);
	~Framebuffer();

	inline int GetWidth() { return mWidth; }
	inline int GetHeight() { return mHeight; }

	inline Colour *GetBuffer() const 
	{ 
		return mColourBuffer; 
	}

	void WriteRGBToFramebuffer(const Colour &colour, int x, int y);
};

