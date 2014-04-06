// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "srCommon.h"
#include "srFrameBuffer.h"
#include "srContext.h"

//==================================
// Console Implementation
//==================================

struct
{
	char *pixels;
	unsigned int width;
	unsigned int height;
	unsigned int lineCount;
} _console_cxt;

#define LEVEL(N, C) case N: pixelOut = C; break
#define LEVEL_DEFAULT(C) default: pixelOut = C; break

char _convertPixel(int colour)
{
	// Convert rgb into brightness levels
	int intensity = (SR_GET_R(colour) + SR_GET_G(colour) + SR_GET_B(colour)) / 3;
	int level = (int)((intensity / 255.0f) * 16.0f);

	// Pixel output
	char pixelOut;
	switch (level)
	{
	LEVEL(1, '.');
	LEVEL(2, ',');
	LEVEL(3, ':');
	LEVEL(4, '~');
	LEVEL(5, '=');
	LEVEL(6, '+');
	LEVEL(7, '?');
	LEVEL(8, 'I');
	LEVEL(9, '7');
	LEVEL(10, 'Z');
	LEVEL(11, 'O');
	LEVEL(12, '8');
	LEVEL(13, 'N');
	LEVEL(14, 'M');
	LEVEL(15, '#');
	LEVEL(16, '@');
	LEVEL_DEFAULT(' ');
	}

	// Return character
	return pixelOut;
}

void _srCreateContext(unsigned int width, unsigned int height)
{
	_console_cxt.pixels = (char*)malloc(sizeof(char) * width * height);
	_console_cxt.width = width;
	_console_cxt.height = height;
	_console_cxt.lineCount = 0;
}

void _srPresent()
{
	// Move cursor to the correct position
	printf("\033[%dA", _console_cxt.lineCount);
	_console_cxt.lineCount = 0;

	// Print everything
	for (int y = 0; y < _console_cxt.height; ++y)
	{
		for (int x = 0; x < _console_cxt.width; ++x)
		{
			// Char needs to be printed twice so the pixel aspect ratio
			// is more square
			char c = _convertPixel(_srGetPixels()[y * _srGetWidth() + x]);
			putchar(c); putchar(c);
		}
		putchar('\n');
		_console_cxt.lineCount++;
	}
}