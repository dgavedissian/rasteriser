// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "sr/sr.h"

#define WINDOW_WIDTH 40
#define WINDOW_HEIGHT 24

kmMat4 view;

void initScene(float fov, float zNear, float zFar)
{
    // Calculate projection matrix
    float aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    kmMat4 proj;
    srSetProjectionMatrix(kmMat4PerspectiveProjection(&proj, fov, aspect, zNear, zFar));
}

/*
void drawCube()
{
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
}
*/

void drawTriangle()
{
    // Calculate coordinates for triangle
    float size = 10.0f;
    float height = sqrt(size * size * 0.75f);

    // Draw
    srBegin();
    srAddVertex(0.0f, height * 2.0f / 3.0f, 0.0f, SR_RGB(255, 0, 0));
    srAddVertex(size * 0.5f, -height / 3.0f, 0.0f, SR_RGB(0, 255, 0));
    srAddVertex(-size * 0.5f, -height / 3.0f, 0.0f, SR_RGB(0, 0, 255));
    srEnd();
}

void drawScene(float r)
{
    // Calculate model matrix
    kmMat4 m;
    kmMat4RotationY(&m, r);

    // Calculate model-view matrix
    kmMat4 mv;
    srSetModelViewMatrix(kmMat4Multiply(&mv, &view, &m));

    // Draw
    drawTriangle();
}

int main(int argc, char** argv)
{
    srCreateFrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

    initScene(60.0f, 1.0f, 1000.0f);

    uint32_t colour = 0xffffffff;
    float r = 0.0f;
    while (1)
    {
        // Calculate view matrix
        kmVec3 eye;
        kmVec3 centre;
        kmVec3 up;
        kmMat4LookAt(&view,
            kmVec3Fill(&eye, 0.0f, 0.0f, 10.0f),
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
