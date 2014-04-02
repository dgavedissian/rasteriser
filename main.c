// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "sr/sr.h"
#include <unistd.h> // for sleep

int main(int argc, char** argv)
{
	int width = 60, height = 20;
	srCreateContext(width, height);
	srInit3D();

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

	// Clean-up
	/*
	srDestroy3D();
	srDestroyContext();
	*/

	return 0;
}