// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "sr/sr.h"

int main(int argc, char** argv)
{
	int width = 40, height = 24;
	srCreateFrameBuffer(width, height);
	srInit3D();

	float angle = 0.0f;
	while (1)
	{
		srClear(0);

		// Change position
		angle += 0.06125f;
		int radius = 3;
		int centreX = 20 + (int)(sin(angle) * radius * 1.5f);
		int centreY = 12 + (int)(cos(angle) * radius * 1.5f);

		// Draw a quad
		srBegin();
			srAddVertex(centreX - radius, centreY - radius, 0);
			srAddVertex(centreX + radius, centreY - radius, 0);
			srAddVertex(centreX - radius, centreY + radius, 0);
			srAddVertex(centreX + radius, centreY + radius, 0);
		srEnd();

		// Present frame
		srDrawAll();
		srPresent();
	}

	// Clean-up
	/*
	srDestroy3D();
	srDestroyContext();
	*/

	return 0;
}