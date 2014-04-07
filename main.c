// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "sr/sr.h"

#define WIDTH 40
#define HEIGHT 24

int main(int argc, char** argv)
{
    srCreateFrameBuffer(WIDTH, HEIGHT);
    srSetRenderState(SR_WIREFRAME, SR_TRUE);

    uint32_t colour = 0xffffffff;
    float r = 0.0f;
    while (1)
    {
        srClear(0);

        // Calculate coordinates for triangle
        const float size = 10.0f;
        float x1 = (WIDTH / 2) + cosf(r - M_PI / 6.0) * size;
        float y1 = (HEIGHT / 2) + sinf(r - M_PI / 6.0) * size;
        float x2 = (WIDTH / 2) + cosf(r + M_PI / 2.0) * size;
        float y2 = (HEIGHT / 2) + sinf(r + M_PI / 2.0) * size;
        float x3 = (WIDTH / 2) + cosf(r + M_PI + M_PI / 6.0) * size;
        float y3 = (HEIGHT / 2) + sinf(r + M_PI + M_PI / 6.0) * size;

        // Draw it
        srBegin();
        srAddVertex(x1, y1, 0.0f, SR_RGB(255, 0, 0));
        srAddVertex(x2, y2, 0.0f, SR_RGB(0, 255, 0));
        srAddVertex(x3, y3, 0.0f, SR_RGB(0, 0, 255));
        srEnd();

        // Present frame
        srPresent();

        // Update rotation
        r += M_PI / 2.0f * 0.025f;
    }

    // Clean-up
    srDestroyFrameBuffer();
    return 0;
}