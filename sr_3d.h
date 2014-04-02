// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef _SR_3D_H_
#define _SR_3D_H_

struct vertex_t
{
	vec3_t p;
};

#define SR_WIREFRAME 0
#define SR_STATE_COUNT 1

// Initialise 3D
void srInit3D();

// State
void setState(int state, int value);

// Drawing meshes
void srBegin();
void srVertex(float x, float y, float z);
void srEnd();

// Draw all
void srDrawAll();
void srDrawTriangle(vertex_t *a, vertex_t *b, vertex_t *c);

#endif