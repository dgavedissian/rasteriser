// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "sr/sr.h"

#define WINDOW_WIDTH 40
#define WINDOW_HEIGHT 24

kmMat4 view;

void initScene(float fov, float aspect, float zNear, float zFar)
{
    // Calculate projection matrix
    kmMat4 proj;
    srSetProjectionMatrix(kmMat4PerspectiveProjection(&proj, fov, aspect, zNear, zFar));
}

void drawScene(float r)
{
    // Calculate model matrix
    kmMat4 m;
    kmMat4RotationY(&m, r);

    // Calculate model-view matrix
    kmMat4 mv;
    srSetModelViewMatrix(kmMat4Multiply(&mv, &view, &m));

    // Vertices
    static const float vData[] = {
        -1.0f, -1.0f, -1.0f, // triangle 1 : begin
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f, -1.0f, // triangle 2 : begin
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f, // triangle 2 : end
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
    };

    // Draw
    int noVertices = 12 * 3;
    srBegin(SR_TRIANGLE_LIST);
    for (int i = 0; i < noVertices; ++i)
        srAddVertex(vData[i * 3], vData[i * 3 + 1], vData[i * 3 + 2], srRGB(1.0f, 1.0f, 1.0f));
    srEnd();
}

int main(int argc, char** argv)
{
    // Get width and height
    int width = WINDOW_WIDTH;
    int height = WINDOW_HEIGHT;
    if (argc > 1)
    {
        width = atoi(argv[1]);
        if (argc > 2)
            height = atoi(argv[2]);
    }

    // Initialise scene
    srCreateFrameBuffer(width, height);
    srSetRenderState(SR_WIREFRAME, SR_TRUE);
    initScene(60.0f, (float)width / (float)height, 0.1f, 100.0f);

    // Enter rendering loop
    float r = 0.0f;
    while (1)
    {
        // Calculate view matrix
        kmVec3 eye;
        kmVec3 centre;
        kmVec3 up;
        kmMat4LookAt(&view,
            kmVec3Fill(&eye, 0.0f, 0.0f, 3.0f),
            kmVec3Fill(&centre, 0.0f, 0.0f, 0.0f),
            kmVec3Fill(&up, 0.0f, 1.0f, 0.0f)
            );

        // Draw scene
        srClear(0);
        drawScene(r);
        srPresent();

        // Update rotation
        r += M_PI / 2.0f * 0.025f;
    }

    // Clean-up
    srDestroyFrameBuffer();
    return 0;
}
