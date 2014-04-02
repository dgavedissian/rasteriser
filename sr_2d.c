// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "sr_common.h"
#include "sr_context.h"
#include "sr_2d.h"

void line(int x1, int y1, int x2, int y2, int colour)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int dxabs = abs(dx);
	int dyabs = abs(dy);
	int sdx = SGN(dx);
	int sdy = SGN(dy);
	int x = dyabs >> 1;
	int y = dxabs >> 1;
	int px = x1;
	int py = y1;

	point(px, py, colour);
	if (dxabs >= dyabs) // the line is more horizontal than vertical
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
			point(px, py, colour);
		}
	}
	else // the line is more vertical than horizontal
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
			point(px, py, colour);
		}
	}
}