// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef _SR_3D_H_
#define _SR_3D_H_

typedef struct
{
	kmVec3 p;
} srVertex;

#define SR_WIREFRAME 0
#define SR_STATE_COUNT 1

// Initialise 3D
void srInit3D();

// State
void setState(int state, int value);

// Drawing meshes
void srBegin();
void srAddVertex(float x, float y, float z);
void srEnd();

// Draw all
void srDrawAll();
void srDrawTriangle(srVertex *a, srVertex *b, srVertex *c);

#endif