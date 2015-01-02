// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "srCommon.h"
#include "srFrameBuffer.h"
#include "srRasteriser.h"

#define INITIAL_MAX_VERTEX_COUNT 10
#define MAX_LIGHT_COUNT 8

struct
{
  srSize width, height;
  srEnum states[SR_RENDER_STATE_COUNT];
  srVertexShaderFunc vs;
  srFragmentShaderFunc fs;
} _r;

void _srCreateRasteriser(uint width, uint height)
{
  _r.width = width;
  _r.height = height;
  _r.states[SR_WIREFRAME] = SR_FALSE;
}

void _srDestroyRasteriser()
{
}

void srSetRenderState(unsigned int state, unsigned int value)
{
  assert(state < SR_RENDER_STATE_COUNT);
  _r.states[state] = value;
}

void srCreateVertexArray(
    srVertexArray* out,
    srVertexAttribute inLayout[],
    srSize inLayoutCount,
    srVertexAttribute outLayout[],
    srSize outLayoutCount,
    float* vertexData,
    srSize vertexCount)
{
  out->inLayoutCount = inLayoutCount;
  out->inVertexSize = 0;
  out->outLayoutCount = outLayoutCount;
  out->outVertexSize = 0;
  out->vertexCount = vertexCount;
  
  // Input layout
  srSize inLayoutSize = inLayoutCount * sizeof(srVertexAttribute);
  out->inLayout = (srVertexAttribute*)malloc(inLayoutSize);
  memcpy(out->inLayout, inLayout, inLayoutSize);
  for (int i = 0; i < inLayoutCount; ++i)
    out->inVertexSize += inLayout[i].size;

  // Output layout
  srSize outLayoutSize = outLayoutCount * sizeof(srVertexAttribute);
  out->outLayout = (srVertexAttribute*)malloc(outLayoutSize);
  memcpy(out->outLayout, outLayout, outLayoutSize);
  for (int i = 0; i < outLayoutCount; ++i)
    out->outVertexSize += outLayout[i].size;
  
  // Allocate data
  srSize dataSize = sizeof(float) * out->inVertexSize * vertexCount;
  out->vertexData = (float*)malloc(dataSize);
  memcpy(out->vertexData, vertexData, dataSize);
}

void srSetShader(srVertexShaderFunc vs, srFragmentShaderFunc fs)
{
  _r.vs = vs;
  _r.fs = fs;
}

static void putPixel(srSize posOffset, float* vertex)
{
  // Run the fragment shader 
  float colour[4];
  _r.fs(vertex, colour);

  // Convert colour to hex
  int r = (int)(colour[0] * 255.0f);
  int g = (int)(colour[1] * 255.0f);
  int b = (int)(colour[2] * 255.0f);
  int a = (int)(colour[3] * 255.0f);
  srPutPixel((uint)vertex[posOffset], (uint)vertex[posOffset + 1],
      SR_HEX_RGBA(r, g, b, a));
}

static void interpolateVertexData(
    float* out,
    srSize posOffset,
    srSize vertexSize,
    float* a,
    float* b,
    float t)
{
  for (int i = 0; i < vertexSize; ++i)
  {
    if (i == posOffset || i == posOffset + 1)
      continue;
    out[i] = SR_LERP(a[i], b[i], t);
  }
}

static void drawLine(srSize posOffset, srSize vertexSize, float* a, float* b)
{
  float x1 = a[posOffset], y1 = a[posOffset + 1];
  float x2 = b[posOffset], y2 = b[posOffset + 1];

  float dx = x2 - x1;
  float dy = y2 - y1;

  // TODO: fp comparison
  if (dx == 0.0f && dy == 0.0f)
  {
    if (x1 >= 0.0f && x1 < _r.width && y1 >= 0.0f && y1 < _r.height)
      putPixel(posOffset, a);
    return;
  }

  // Temporary vertex used as the output of interpolation
  float* interpVertex = (float*)malloc(sizeof(float) * vertexSize);
  if (fabs(dx) > fabs(dy))
  {
    float xmin, xmax;
    if (x1 < x2)
    {
      xmin = x1;
      xmax = x2;
    }
    else
    {
      xmin = x2;
      xmax = x1;
    }

    // Draw line in terms of y slope
    float slope = dy / dx;
    for (float x = xmin; x <= xmax; x += 1.0f)
    {
      float y = y1 + ((x - x1) * slope);
      if (x >= 0 && x < _r.width && y >= 0 && y < _r.height)
      {
        float t = (x - x1) / dx; // TODO: is this correct?
        interpVertex[posOffset] = x;
        interpVertex[posOffset + 1] = y;
        interpolateVertexData(interpVertex, posOffset, vertexSize, a, b, t); 
        putPixel(posOffset, interpVertex);
      }
    }
  }
  else
  {
    float ymin, ymax;
    if (y1 < y2)
    {
      ymin = y1;
      ymax = y2;
    }
    else
    {
      ymin = y2;
      ymax = y1;
    }

    // Draw line in terms of x slope
    float slope = dx / dy;
    for (float y = ymin; y <= ymax; y += 1.0f)
    {
      float x = x1 + ((y - y1) * slope);
      if (x >= 0 && x < _r.width && y >= 0 && y < _r.height)
      {
        float t = (y - y1) / dy; // TODO: is this correct?
        interpVertex[posOffset] = x;
        interpVertex[posOffset + 1] = y;
        interpolateVertexData(interpVertex, posOffset, vertexSize, a, b, t); 
        putPixel(posOffset, interpVertex);
      }
    }
  }
}

typedef struct
{
  float* v1;
  float* v2;
} Edge;

static void buildEdge(Edge* e, srSize posOffset, float* v1, float* v2)
{
  if (v1[posOffset + 1] < v2[posOffset + 1])
  {
    e->v1 = v1;
    e->v2 = v2;
  }
  else
  {
    e->v1 = v2;
    e->v2 = v1;
  }
}

static void drawSpansBetweenEdges(srSize posOffset, srSize vertexSize, Edge* e1, Edge* e2)
{
  // Calculate difference between the y coordinates of the first edge and
  // return if 0
  float e1ydiff = (float)(e1->v2[posOffset + 1] - e1->v1[posOffset + 1]);
  if (e1ydiff == 0.0f)
    return;

  // Calculate difference between the y coordinates of the second edge and
  // return if 0
  float e2ydiff = (float)(e2->v2[posOffset + 1] - e2->v1[posOffset + 1]);
  if (e2ydiff == 0.0f)
    return;

  // Calculate differences between the x coordinates
  float e1xdiff = (float)(e1->v2[posOffset] - e1->v1[posOffset]);
  float e2xdiff = (float)(e2->v2[posOffset] - e2->v1[posOffset]);

  // Calculate factors to use for interpolation with the edges and the step
  // values to increase them by after drawing each span
  float factor1 = (float)(e2->v1[posOffset + 1] - e1->v1[posOffset + 1]) / e1ydiff;
  float factorStep1 = 1.0f / e1ydiff;
  float factor2 = 0.0f;
  float factorStep2 = 1.0f / e2ydiff;

  // Loop through the lines between the edges and draw spans
  for (int y = e2->v1[posOffset + 1]; y < e2->v2[posOffset + 1]; ++y)
  {
    // Generate two temp vertices
    float* interpVert0 = (float*)malloc(sizeof(float) * vertexSize * 2);
    float* interpVert1 = &interpVert0[vertexSize];

    // Draw span
    interpVert0[posOffset] = e1->v1[posOffset] + (int)(e1xdiff * factor1);
    interpVert0[posOffset + 1] = y;
    interpolateVertexData(interpVert0, posOffset, vertexSize, e1->v1, e1->v2, factor1);
    interpVert1[posOffset] = e2->v1[posOffset] + (int)(e2xdiff * factor2);
    interpVert1[posOffset + 1] = y;
    interpolateVertexData(interpVert1, posOffset, vertexSize, e2->v1, e2->v2, factor2);
    drawLine(posOffset, vertexSize, interpVert0, interpVert1);
    
    // Increase factors
    factor1 += factorStep1;
    factor2 += factorStep2;
  }
}

static void drawTriangle(
    srSize posOffset, srSize vertexSize, float *a, float *b, float *c)
{
  // Build the edges
  Edge edges[3];
  buildEdge(&edges[0], posOffset, a, b);
  buildEdge(&edges[1], posOffset, b, c);
  buildEdge(&edges[2], posOffset, c, a);

  if (_r.states[SR_WIREFRAME] == SR_TRUE)
  {
    // Draw the edges
    for (int i = 0; i < 3; ++i)
      drawLine(posOffset, vertexSize, edges[i].v1, edges[i].v2);
  }
  else
  {
    int maxLength = 0;
    int longEdge = 0;

    // Find the edge with the greatest length in the y axis
    for (int i = 0; i < 3; ++i)
    {
      int length = edges[i].v2[posOffset + 1] - edges[i].v1[posOffset + 1];
      if (length > maxLength)
      {
        maxLength = length;
        longEdge = i;
      }
    }

    int shortEdge1 = (longEdge + 1) % 3;
    int shortEdge2 = (longEdge + 2) % 3;

    // Draw spans between edges
    drawSpansBetweenEdges(posOffset, vertexSize, &edges[longEdge], &edges[shortEdge1]);
    drawSpansBetweenEdges(posOffset, vertexSize, &edges[longEdge], &edges[shortEdge2]);
  }
}

void srDrawVertexArray(srEnum type, srVertexArray* vao)
{
  // ==========================================================================
  // Stage 1: Vertex Shader
  // ==========================================================================

  // Figure out where the vertices are in the input and output layouts
  srSize inPositionOffset = 0;
  srSize outPositionOffset = 0;
  for (int j = 0; j < vao->inLayoutCount; ++j)
  {
    if (vao->inLayout[j].type == SR_VERT_POSITION)
      break;
    inPositionOffset += vao->inLayout[j].type;
  }
  for (int j = 0; j < vao->outLayoutCount; ++j)
  {
    if (vao->outLayout[j].type == SR_VERT_POSITION)
      break;
    outPositionOffset += vao->outLayout[j].type;
  }

  // Create space for output data
  float* out = (float*)malloc(sizeof(float) * vao->outVertexSize * vao->vertexCount); 

  // Run the vertex shader on the data
  for (int i = 0; i < vao->vertexCount; ++i)
  {
    int inOffset = i * vao->inVertexSize;
    int outOffset = i * vao->outVertexSize;
    _r.vs(vao->vertexData + inOffset, out + outOffset);
    
    // Transform normalised device coordinates to viewport coordinates
    kmVec3* v = (kmVec3*)(out + outOffset + outPositionOffset);
    v->x = (v->x + 1.0f) * 0.5f * _r.width;
    v->y = (v->y + 1.0f) * 0.5f * _r.height;
  }

  // =====================================
  // Stage 2: Rasterise
  // =====================================
  
  // Triangle list for the time being
  for (int v = 0; v < vao->vertexCount; v += 3)
  {
    float* v0 = out + ((v) * vao->outVertexSize);
    float* v1 = out + ((v + 1) * vao->outVertexSize);
    float* v2 = out + ((v + 2) * vao->outVertexSize);
    drawTriangle(outPositionOffset, vao->outVertexSize, v0, v1, v2);
  }

  /*
  // Point List
  if (_im.primitive == SR_POINT_LIST)
  {
    for (int i = 0; i < _im.size; ++i)
      srPutPixel(v[i].p.x, v[i].p.y, srColourToHex(&v[i].c));
  }

  // Line List
  if (_im.primitive == SR_LINE_LIST && (_im.size & 2) == 0)
  {
    // Cycle through lines
    for (int l = 0; l < _im.size / 2; ++l)
      srDrawLine(&v[l * 2], &v[l * 2 + 1]);
  }

  // Line Strip
  if (_im.primitive == SR_LINE_STRIP && _im.size > 1)
  {
    // Take first vertex
    srVertex* vp = &v[0];

    // Cycle through remaining vertices
    for (int i = 1; i < _im.size; ++i)
    {
      srVertex* vc = &v[i];
      srDrawLine(vp, vc);
      vp = vc;
    }
  }

  // Triangle List
  if (_im.primitive == SR_TRIANGLE_LIST && (_im.size % 3) == 0)
  {
    // Cycle through triangles
    for (int t = 0; t < _im.size / 3; ++t)
      srDrawTriangle(&v[t * 3], &v[t * 3 + 1], &v[t * 3 + 2]);
  }

  // Triangle Strip
  if (_im.primitive == SR_TRIANGLE_STRIP && _im.size > 2)
  {
    // Take first two vertices
    srVertex* v0 = &v[0];
    srVertex* v1 = &v[1];

    // Cycle through remaining vertices
    for (int i = 2; i < _im.size; ++i)
    {
      srVertex* vc = &v[i];
      srDrawTriangle(v0, v1, vc);
      v0 = v1;
      v1 = vc;
    }
  }
  */
}

