// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "srCommon.h"
#include "srContext.h"
#include "srFrameBuffer.h"
#include <unistd.h> // for usleep

struct
{
	uint32_t* pixels;
	unsigned int width;
	unsigned int height;
} _fb;

void srCreateFrameBuffer(unsigned int width, unsigned int height)
{
	_fb.pixels = (uint32_t*)malloc(sizeof(uint32_t) * width * height);
	_fb.width = width;
	_fb.height = height;

	// Initialise frame buffer
	srClear(0);

	// Set up rendering context
	_srCreateContext(width, height);
}

void srClear(uint32_t colour)
{
	memset(_fb.pixels, colour, sizeof(uint32_t) * _fb.width * _fb.height);
}

void srDrawPixel(unsigned int x, unsigned int y, uint32_t colour)
{
	if (x >= _fb.width || y >= _fb.height)
		return;

	_fb.pixels[y * _fb.width + x] = colour;
}

void srPresent()
{
	_srPresent();
	usleep(25 * 1000);
}

uint32_t* _srGetPixels()
{
	return _fb.pixels;
}

unsigned int _srGetWidth()
{
	return _fb.width;
}

unsigned int _srGetHeight()
{
	return _fb.height;
}