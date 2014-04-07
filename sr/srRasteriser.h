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

// =========================
// Render State
// =========================

// Render states
#define SR_WIREFRAME 0
#define SR_RENDER_STATE_COUNT 1

// Set a render state
void srSetRenderState(unsigned int state, unsigned int value);

// =========================
// Immediate rendering
// =========================

// Begin a new mesh
void srBegin();

// Add a vertex to the mesh
void srAddVertex(float x, float y, float z, int colour);

// Finish a mesh
void srEnd();

// =========================
// Drawing
// =========================

// Draw a line
void srDrawLine(srVertex* a, srVertex* b);

// Draw a triangle
void srDrawTriangle(srVertex *a, srVertex *b, srVertex *c);

// =========================
// Pipeline
// =========================

// Internal: Execute the pipeline
// Transform vertices -> Rasterise -> Blending -> Whatever
void _srRasteriseScene();

#endif