// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "srCommon.h"
#include "srContext.h"
#include <unistd.h> // for sleep

//==================================
// Console Implementation
//==================================

struct srConsoleContext
{
	char *pixels;
	int bytes;
	int width;
	int height;
	int cursorPosition;
} _console_cxt;

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

void srCreateContext(int width, int height)
{
	assert(width > 0);
	assert(height > 0);

	_console_cxt.bytes = sizeof(char) * width * height;
	_console_cxt.pixels = (char*)malloc(_console_cxt.bytes);
	_console_cxt.width = width;
	_console_cxt.height = height;
	_console_cxt.cursorPosition = 0;

	srClear(0);
}

void srClear(int colour)
{
	memset(_console_cxt.pixels, _convertPixel(colour), _console_cxt.bytes);
}

void srPoint(int x, int y, int colour)
{
	assert(x >= 0);
	assert(x < _console_cxt.width);
	assert(y >= 0);
	assert(y < _console_cxt.height);

	_console_cxt.pixels[y * _console_cxt.width + x] = _convertPixel(colour);
}

void srPresent()
{
	// Move cursor to the correct position
	printf("\033[%dA", _console_cxt.cursorPosition);
	_console_cxt.cursorPosition = 0;

	// Print everything
	for (int y = 0; y < _console_cxt.height; ++y)
	{
		for (int x = 0; x < _console_cxt.width; ++x)
		{
			// Char needs to be printed twice so the pixel aspect ratio
			// is more square
			char c = _console_cxt.pixels[y * _console_cxt.width + x];
			putchar(c); putchar(c);
		}
		putchar('\n');
		_console_cxt.cursorPosition++;
	}

	// Wait a bit - so the console doesnt update thousands of times a second
	usleep(25 * 1000);
}