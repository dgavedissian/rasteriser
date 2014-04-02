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

		// Draw a quad
		srBegin();
			srVertex(10, 10, 0);
			srVertex(20, 10, 0);
			srVertex(10, 20, 0);
			srVertex(20, 20, 0);
		srEnd();

		// Present frame
		srDrawAll();
		srPresent();

		// Wait a bit
		sleep(1);
	}

	return 0;
}