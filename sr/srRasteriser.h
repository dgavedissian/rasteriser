// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef _SRRASTERISER_H_
#define _SRRASTERISER_H_

typedef struct
{
	kmVec3 p;
	uint32_t c;
} srVertex;

// Internal: Create the rasteriser
void _srCreateRasteriser();

// Internal: Destroy the rasteriser
void _srDestroyRasteriser();

// =====================================
// Render State
// =====================================

// Render states
#define SR_WIREFRAME 0
#define SR_RENDER_STATE_COUNT 1

// Set a render state
void srSetRenderState(unsigned int state, unsigned int value);

// Model-view matrix
void srSetModelViewMatrix(kmMat4* matrix);
kmMat4* srGetModelViewMatrix();

// Projection Matrix
void srSetProjectionMatrix(kmMat4* matrix);
kmMat4* srGetProjectionMatrix();

// =====================================
// Rendering
// =====================================

#define SR_POINT_LIST 0
#define SR_LINE_LIST 1
#define SR_LINE_STRIP 2
#define SR_TRIANGLE_LIST 3
#define SR_TRIANGLE_STRIP 4

// The interface for these functions is inspired by OpenGLs
// legacy immedate mode rendering functions which don't
// have much of a performance hit in a software rasteriser
// due to the GPU memory transfer bottleneck faced with
// OpenGL not existing in a CPU only renderer

// Begin a new primitive
void srBegin(unsigned int primitiveType);

// Add a vertex to the current primitive
void srAddVertex(float x, float y, float z, int colour);

// Finish and render a primitinve
void srEnd();

// =====================================
// Drawing
// =====================================

// Draw a line
void srDrawLine(srVertex* a, srVertex* b);

// Draw a triangle
void srDrawTriangle(srVertex *a, srVertex *b, srVertex *c);

#endif