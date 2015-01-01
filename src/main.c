// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "sr/sr.h"

#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480

kmMat4 view;
  
static const float cubeVertices[] = {
  -1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f, 1.0f,
  -1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f,
  -1.0f, 1.0f, -1.0f,
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

void initScene()
{
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
  int noVertices = 12 * 3;
  srBegin(SR_TRIANGLE_LIST);
  for (int i = 0; i < noVertices; ++i)
    srAddVertex(
        cubeVertices[i * 3], cubeVertices[i * 3 + 1], cubeVertices[i * 3 + 2],
        srRGB(0.0f, 0.0f, 1.0f));
  srEnd();
}

int main(int argc, char** argv)
{
  // Get width and height
  int width = DEFAULT_WINDOW_WIDTH;
  int height = DEFAULT_WINDOW_HEIGHT;
  if (argc > 1)
  {
    width = atoi(argv[1]);
    if (argc > 2)
      height = atoi(argv[2]);
  }

  // Set up the rasteriser
  srInitParams params;
  params.width = width;
  params.height = height;
  srInit(&params);
  srSetMaxFPS(60);
  srSetRenderState(SR_WIREFRAME, SR_TRUE);
  
  // Set projectino matrix
  kmMat4 proj;
  float aspect = (float)width / height;
  srSetProjectionMatrix(
      kmMat4PerspectiveProjection(&proj, 60.0f, aspect, 0.1f, 100.0f));

  // Initialise the scene
  initScene();

  // Enter rendering loop
  kmVec3 eye;
  kmVec3 centre;
  kmVec3 up;
  float angle = 0.0f;
  while (srContextActive())
  {
    // Calculate view matrix
    kmMat4LookAt(&view,
      kmVec3Fill(&eye, 0.0f, 0.0f, 3.0f),
      kmVec3Fill(&centre, 0.0f, 0.0f, 0.0f),
      kmVec3Fill(&up, 0.0f, 1.0f, 0.0f)
      );

    // Draw scene
    srBeginFrame(0);
    drawScene(angle);
    srEndFrame();

    // Update rotation
    angle += M_PI / 2.0f * 0.01f;
  }

  // Clean-up
  srShutdown();
  return 0;
}
