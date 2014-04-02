// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "sr.h"
#include <unistd.h>

int main(int argc, char** argv)
{
	int width = 80, height = 25;
	srCreateContext(width, height);
	srInit3D();

	int tick = 0;
	int hw = width / 2, hh = height / 2;
	int noSteps = 60;
	while (1)
	{
		srClear(0);

		// Draw borders
		srLine(0, 0, width - 1, 0, 0x7f7f7fff);
		srLine(width - 1, 0, width - 1, height - 1, 0x7f7f7fff);
		srLine(width - 1, height - 1, 0, height - 1, 0x7f7f7fff);
		srLine(0, height - 1, 0, 0, 0x7f7f7fff);

		// Draw a line from the centre
		tick = (tick + 1) % noSteps;
		double angle = (double)tick * M_PI * 2.0f / noSteps;
		srLine(
			hw - 1, hh - 1,
			MAX(hw - 1 + sin(angle) * hw, 0), MAX(hh - 1 - cos(angle) * hh, 0),
			RGB(255, 127, 127));

		// Present frame
		srPresent();

		// Wait a bit
		usleep(50 * 1000);
	}

	return 0;
}