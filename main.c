// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "sr/sr.h"

#define WIDTH 40
#define HEIGHT 24

int main(int argc, char** argv)
{
	srCreateFrameBuffer(WIDTH, HEIGHT);

	uint32_t colour = 0xffffffff;
	float angle = 0.0f;
	while (1)
	{
		srClear(0);

		// Change position
		angle += 0.05f;
		int radius = 3;
		int centreX = WIDTH / 2 + (int)(sin(angle) * radius * 1.5f);
		int centreY = HEIGHT / 2 + (int)(cos(angle) * radius * 1.5f);

		// Draw a quad
		srBegin();
			srAddVertex(centreX - radius, centreY - radius, 0.0f, colour);
			srAddVertex(centreX + radius, centreY - radius, 0.0f, colour);
			srAddVertex(centreX - radius, centreY + radius, 0.0f, colour);
			srAddVertex(centreX + radius, centreY + radius, 0.0f, colour);
		srEnd();

		// Present frame
		srPresent();
	}

	// Clean-up
	srDestroyFrameBuffer();
	return 0;
}