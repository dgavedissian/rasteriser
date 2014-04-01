#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <assert.h>
#include <math.h>

// RGB <-> HEX
#define RGBA(r, g, b, a) (((r) & 0xFF) << 24) + (((g) & 0xFF) << 16) + (((b) & 0xFF) << 8) + ((a) & 0xFF)
#define RGB(r, g, b) RGBA(r, g, b, 0xFF)
#define GET_R(colour) (((colour) >> 24) & 0xFF)
#define GET_G(colour) (((colour) >> 16) & 0xFF)
#define GET_B(colour) (((colour) >> 8) & 0xFF)
#define GET_A(colour) ((colour) & 0xFF)

#define SGN(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

// Prototype for createContext
void clear(int);

// Implementation
struct Context
{
	char *pixels;
	int width;
	int height;
	int bytes;
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

void drawPixel(int x, int y, int colour)
{
	assert(x >= 0);
	assert(x < _cxt.width);
	assert(y >= 0);
	assert(y < _cxt.height);

	_cxt.pixels[y * _cxt.width + x] = _convertPixel(colour);
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

void renderFrame()
{
	for (int y = 0; y < _cxt.height; ++y)
	{
		for (int x = 0; x < _cxt.width; ++x)
			putchar(_cxt.pixels[y * _cxt.width + x]);
		putchar('\n');
	}
}

// Functions
void drawLine(int x1, int y1, int x2, int y2, int colour)
{
	int dx = x2 - x1;      /* the horizontal distance of the line */
	int dy = y2 - y1;      /* the vertical distance of the line */
	int dxabs = abs(dx);
	int dyabs = abs(dy);
	int sdx = SGN(dx);
	int sdy = SGN(dy);
	int x = dyabs >> 1;
	int y = dxabs >> 1;
	int px = x1;
	int py = y1;

	drawPixel(px, py, colour);
	if (dxabs >= dyabs) /* the line is more horizontal than vertical */
	{
		for (int i = 0; i < dxabs; ++i)
		{
			y += dyabs;
			if (y >= dxabs)
			{
				y -= dxabs;
				py += sdy;
			}
			px += sdx;
			drawPixel(px, py, colour);
		}
	}
	else /* the line is more vertical than horizontal */
	{
		for (int i = 0; i < dyabs; i++)
		{
			x += dxabs;
			if (x >= dyabs)
			{
				x -= dyabs;
				px += sdx;
			}
			py += sdy;
			drawPixel(px, py, colour);
		}
	}
}

void clear(int colour)
{
	memset(_cxt.pixels, _convertPixel(colour), _cxt.bytes);
}

// Test
int main(int argc, char** argv)
{
	int width = 80, height = 25;
	createContext(width, height);

	int tick = 0;
	int hw = width / 2, hh = height / 2;
	int noSteps = 60;
	while (1)
	{
		clear(0);
		{
			// Draw borders
			drawLine(0, 0, width - 1, 0, 0x7f7f7fff);
			drawLine(width - 1, 0, width - 1, height - 1, 0x7f7f7fff);
			drawLine(width - 1, height - 1, 0, height - 1, 0x7f7f7fff);
			drawLine(0, height - 1, 0, 0, 0x7f7f7fff);

			// Draw a line from the centre
			tick = (tick + 1) % noSteps;
			double angle = (double)tick * M_PI * 2.0f / noSteps;
			drawLine(
				hw - 1, hh - 1,
				MAX(hw - 1 + sin(angle) * hw, 0), MAX(hh - 1 - cos(angle) * hh, 0),
				RGB(255, 127, 127));
		}
		renderFrame();

		// Restrict to 20fps
		usleep(50 * 1000);
	}

	return 0;
}