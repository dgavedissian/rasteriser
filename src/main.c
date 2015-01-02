// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "sr/sr.h"

// Horrible globals
kmMat4 view;
kmMat4 proj;

uint width = 800;
uint height = 600;

static float cube[36 * 3] = {
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

srVertexArray vao;

// Shader parameters
kmMat4 mvp;

// Shader
typedef struct
{
  kmVec3 position;
} VSIn;

typedef struct
{
  kmVec3 position;
} VSOut;

void vs(float* _in, float* _out)
{
  VSIn* in = (VSIn*)_in;
  VSOut* out = (VSOut*)_out;
  kmVec3TransformCoord(&out->position, &in->position, &mvp);
}

void fs(float* _in, float* _out)
{
  VSOut* in = (VSOut*)_in;
  srColour* out = (srColour*)_out;

  out->r = in->position.x / width;
  out->g = 0.0f;
  out->b = in->position.y / height;
  out->a = 1.0f;
}

void initScene()
{
  srVertexAttribute in[] = {
    {SR_VERT_POSITION, 3}
  };
  srVertexAttribute out[] = {
    {SR_VERT_POSITION, 3}
  };

  srCreateVertexArray(&vao, in, 1, out, 1, cube, 36);
}

void drawScene(float r)
{
  // Calculate model-view-projection matrix
  kmMat4RotationY(&mvp, r);
  kmMat4Multiply(&mvp, &view, &mvp);
  kmMat4Multiply(&mvp, &proj, &mvp);

  // Draw
  srSetShader(&vs, &fs);
  srDrawVertexArray(SR_TRIANGLE_LIST, &vao);
}

int main(int argc, char** argv)
{
  // Get width and height
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
  params.outputContext = SR_CTX_SDL;
  srInit(&params);
  width = srGetWidth();
  height = srGetHeight();
  srSetMaxFPS(60);
  //srSetRenderState(SR_WIREFRAME, SR_TRUE);
  
  // Set projectino matrix
  float aspect = (float)width / height;
  kmMat4PerspectiveProjection(&proj, 60.0f, aspect, 0.1f, 100.0f);

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
    srBegin(0);
    drawScene(angle);
    srEnd();

    // Update rotation
    angle += M_PI / 2.0f * 0.01f;
  }

  // Clean-up
  srShutdown();
  return 0;
}
