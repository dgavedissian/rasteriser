// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "sr/sr.h"

#define WIDTH 40
#define HEIGHT 24

void draw2DTriangle(float r)
{
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
}

void init3D(float fov, float zNear, float zFar)
{
    // Calculate projection matrix
    float aspect = (float)WIDTH / (float)HEIGHT;
    kmMat4 proj;
    srSetProjectionMatrix(kmMat4PerspectiveProjection(&proj, fov, aspect, zNear, zFar));
}

void draw3DTriangle(float r)
{
    // Calculate view matrix
    kmMat4 v;
    kmVec3 eye;
    kmVec3 centre;
    kmVec3 up;
    kmVec3Fill(&eye, 0.0f, 0.0f, 20.0f);
    kmVec3Fill(&centre, 0.0f, 0.0f, 0.0f);
    kmVec3Fill(&up, 0.0f, 1.0f, 0.0f);
    kmMat4LookAt(&v, &eye, &centre, &up);

    // Calculate model matrix
    kmMat4 m;
    kmMat4RotationY(&m, r);

    // Calculate model-view matrix
    kmMat4 mv;
    srSetModelViewMatrix(kmMat4Multiply(&mv, &v, &m));

    // Calculate coordinates for triangle
    float size = 20.0f;
    float height = sqrt(size * size * 0.75f);

    // Draw it
    srBegin();
    srAddVertex(0.0f, height * 2.0f / 3.0f, 0.0f, SR_RGB(255, 0, 0));
    srAddVertex(-size * 0.5f, -height / 3.0f, 0.0f, SR_RGB(0, 255, 0));
    srAddVertex(size * 0.5f, -height / 3.0f, 0.0f, SR_RGB(0, 0, 255));
    srEnd();
}

int main(int argc, char** argv)
{
    srCreateFrameBuffer(WIDTH, HEIGHT);
    srSetRenderState(SR_WIREFRAME, SR_TRUE);

    init3D(60.0f, 1.0f, 1000.0f);

    uint32_t colour = 0xffffffff;
    float r = 0.0f;
    while (1)
    {
        srClear(0);
        draw3DTriangle(r);
        srPresent();

        // Update rotation
        r += M_PI / 2.0f * 0.025f;
    }

    // Clean-up
    srDestroyFrameBuffer();
    return 0;
}