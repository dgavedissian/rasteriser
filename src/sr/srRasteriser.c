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
  kmMat4 modelView, proj;
  srVertexShaderFunc vs;
  srFragmentShaderFunc fs;
} _r;

// Immediate mode data
// TODO: eventually we can use some VBO here once thats implemented
struct
{
  srVertex* vertices;
  unsigned int size, maxSize;
  unsigned int primitive;
} _im;

void _srCreateRasteriser(uint width, uint height)
{
  _r.width = width;
  _r.height = height;

  // Default render states
  _r.states[SR_WIREFRAME] = SR_FALSE;
  _r.states[SR_LIGHTING] = SR_FALSE;

  // Default matrices
  kmMat4Identity(&_r.modelView);
  kmMat4Identity(&_r.proj);

  // Initialise immediate mode data
  _im.maxSize = INITIAL_MAX_VERTEX_COUNT;
  _im.vertices = (srVertex*)malloc(sizeof(srVertex) * _im.maxSize);
  _im.size = 0;
}

void _srDestroyRasteriser()
{
  // Free immediate mode data
  free(_im.vertices);
}

void srSetRenderState(unsigned int state, unsigned int value)
{
  assert(state < SR_RENDER_STATE_COUNT);
  _r.states[state] = value;
}

void srSetModelViewMatrix(kmMat4* matrix)
{
  kmMat4Assign(&_r.modelView, matrix);
}

kmMat4* srGetModelViewMatrix()
{
  return &_r.modelView;
}

void srSetProjectionMatrix(kmMat4* matrix)
{
  kmMat4Assign(&_r.proj, matrix);
}

kmMat4* srGetProjectionMatrix()
{
  return &_r.proj;
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

// Pre: Vertices are assumed to be homogeneous coordinates
static void drawLine(srSize posOffset, srSize vertexSize, float* a, float* b)
{
  float x1 = a[posOffset], y1 = a[posOffset + 1];
  float x2 = b[posOffset], y2 = b[posOffset + 1];

  float dx = x2 - x1;
  float dy = y2 - y1;

  // TODO: fp comparison
  if (dx == 0.0f && dy == 0.0f)
  {
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
      if (x > 0 && x < _r.width && y > 0 && y < _r.height)
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
      if (x > 0 && x < _r.width && y > 0 && y < _r.height)
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
/*
typedef struct
{
  srVertex* v1;
  srVertex* v2;
} srEdge;

typedef struct
{
  float x1, x2;
  srColour* c1;
  srColour* c2;
} srSpan;

void buildEdge(srEdge* e, srVertex* v1, srVertex* v2)
{
  if (v1->p.y < v2->p.y)
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

void buildSpan(srSpan* s, float x1, float x2, srColour* c1, srColour* c2)
{
  if (x1 < x2)
  {
    s->x1 = x1;
    s->x2 = x2;
    s->c1 = c1;
    s->c2 = c2;
  }
  else
  {
    s->x1 = x2;
    s->x2 = x1;
    s->c1 = c2;
    s->c2 = c1;
  }
}

void drawSpan(srSpan* span, int y)
{
  int xdiff = span->x2 - span->x1;
  if (xdiff == 0)
    return;

  float factor = 0.0f;
  float factorStep = 1.0f / (float)xdiff;

  // Draw each pixel in the span
  for (int x = span->x1; x < span->x2; ++x)
  {
    if (x > 0 && x < _r.width && y > 0 && y < _r.height)
    {
      srColour colour;
      srColourMix(&colour, span->c1, span->c2, factor);
      srPutPixel((uint)x, (uint)y, srColourToHex(&colour));
    }
    factor += factorStep;
  }
}

void drawSpansBetweenEdges(srEdge* e1, srEdge* e2)
{
  // Calculate difference between the y coordinates
  // of the first edge and return if 0
  float e1ydiff = (float)(e1->v2->p.y - e1->v1->p.y);
  if (e1ydiff == 0.0f)
    return;

  // Calculate difference between the y coordinates
  // of the second edge and return if 0
  float e2ydiff = (float)(e2->v2->p.y - e2->v1->p.y);
  if(e2ydiff == 0.0f)
    return;

  // Calculate differences between the x coordinates
  float e1xdiff = (float)(e1->v2->p.x - e1->v1->p.x);
  float e2xdiff = (float)(e2->v2->p.x - e2->v1->p.x);

  // Calculate factors to use for interpolation
  // with the edges and the step values to increase
  // them by after drawing each span
  float factor1 = (float)(e2->v1->p.y - e1->v1->p.y) / e1ydiff;
  float factorStep1 = 1.0f / e1ydiff;
  float factor2 = 0.0f;
  float factorStep2 = 1.0f / e2ydiff;

  // Loop through the lines between the edges and draw spans
  for (int y = e2->v1->p.y; y < e2->v2->p.y; ++y)
  {
    // Draw span
    srSpan span;
    srColour c1, c2;
    srColourMix(&c1, &e1->v1->c, &e1->v2->c, factor1);
    srColourMix(&c2, &e2->v1->c, &e2->v2->c, factor2);
    buildSpan(&span,
      e1->v1->p.x + (int)(e1xdiff * factor1),
      e2->v1->p.x + (int)(e2xdiff * factor2),
      &c1, &c2);
    drawSpan(&span, y);

    // Increase factors
    factor1 += factorStep1;
    factor2 += factorStep2;
  }
}

void srDrawTriangle(srVertex *a, srVertex *b, srVertex *c)
{
  // Store the edges
  srEdge edges[3];
  buildEdge(&edges[0], a, b);
  buildEdge(&edges[1], b, c);
  buildEdge(&edges[2], c, a);

  if (_r.states[SR_WIREFRAME] == SR_TRUE)
  {
    // Just draw the edges
    for (int i = 0; i < 3; ++i)
      srDrawLine(edges[i].v1, edges[i].v2);
  }
  else
  {
    int maxLength = 0;
    int longEdge = 0;

    // Find the edge with the greatest length in the y axis
    for (int i = 0; i < 3; ++i)
    {
      int length = edges[i].v2->p.y - edges[i].v1->p.y;
      if (length > maxLength)
      {
        maxLength = length;
        longEdge = i;
      }
    }

    int shortEdge1 = (longEdge + 1) % 3;
    int shortEdge2 = (longEdge + 2) % 3;

    // Draw spans between edges
    drawSpansBetweenEdges(&edges[longEdge], &edges[shortEdge1]);
    drawSpansBetweenEdges(&edges[longEdge], &edges[shortEdge2]);
  }
}
*/

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
    drawLine(outPositionOffset, vao->outVertexSize, v0, v1);
    drawLine(outPositionOffset, vao->outVertexSize, v1, v2);
    drawLine(outPositionOffset, vao->outVertexSize, v0, v2);
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

