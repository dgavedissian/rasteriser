// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "sr_common.h"
#include "sr_context.h"

// Console Implementation

// Current context
struct context_t
{
	char *pixels;
	int bytes;
	int width;
	int height;
} _cxt;

char _convertPixel(int colour)
{
	// Convert rgb into 4 brightness levels
	int intensity = (GET_R(colour) + GET_G(colour) + GET_B(colour)) / 3;
	int level = (int)((intensity / 255.0) * 4.0);

	// Pixel output
	char pixelOut = ' ';
	switch (level)
	{
	case 1:
		pixelOut = '-';
		break;
		
	case 2:
		pixelOut = '+';
		break;
		
	case 3:
		pixelOut = '#';
		break;

	case 4:
		pixelOut = '@';
		break;

	default:
		break;
	}

	// Return character
	return pixelOut;
}

void createContext(int width, int height)
{
	assert(width > 0);
	assert(height > 0);

	_cxt.bytes = sizeof(char) * width * height;
	_cxt.pixels = (char*)malloc(_cxt.bytes);
	_cxt.width = width;
	_cxt.height = height;

	clear(0);
}

void clear(int colour)
{
	memset(_cxt.pixels, _convertPixel(colour), _cxt.bytes);
}

void point(int x, int y, int colour)
{
	assert(x >= 0);
	assert(x < _cxt.width);
	assert(y >= 0);
	assert(y < _cxt.height);

	_cxt.pixels[y * _cxt.width + x] = _convertPixel(colour);
}

void present()
{
	for (int y = 0; y < _cxt.height; ++y)
	{
		for (int x = 0; x < _cxt.width; ++x)
			putchar(_cxt.pixels[y * _cxt.width + x]);
		putchar('\n');
	}
}