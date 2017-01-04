/*---------------------------------------------------------------------
*
* Copyright © 2016  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#include <stdlib.h>
#include "Framebuffer.h"

Framebuffer::Framebuffer()
{
	mWidth = 0;
	mHeight = 0;
	mColourBuffer = NULL;
}

Framebuffer::Framebuffer(int width, int height)
{
	InitFramebuffer(width, height);
}

Framebuffer::~Framebuffer()
{
	delete[] mColourBuffer;
}

void Framebuffer::WriteRGBToFramebuffer(const Colour & colour, int x, int y)
{
	int offset = y*mWidth + x;

	*(mColourBuffer + offset) = colour;
}

void Framebuffer::InitFramebuffer(int width, int height)
{
	int size = width*height;
	mWidth = width;
	mHeight = height;

	mColourBuffer = new Colour[size];

	//memset(mColourBuffer, 0, size*sizeof(PixelRGBA));
}
