// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "srCommon.h"
#include "srContext.h"
#include "srShapes.h"
#include "sr3D.h"

#define START_MESH_COUNT 10
#define START_VERTEX_COUNT 10

typedef struct
{
	srVertex *vertices;
	int size;
	int maxSize;
} Mesh;

// TODO: merge with sr_context.c's _cxt object
struct Context
{
	// Meshes
	Mesh *meshes;
	int count;
	int maxCount;

	// Rendering State
	int states[SR_STATE_COUNT];
} _cxt;

void srInit3D()
{
	_cxt.meshes = NULL;
	_cxt.count = 0;
	_cxt.maxCount = 0;

	// Default states
	_cxt.states[SR_WIREFRAME] = SR_TRUE;
}

void setState(int state, int value)
{
	assert(state >= 0);
	assert(state < SR_STATE_COUNT);
	_cxt.states[state] = value;
}

void srBegin()
{
	// Expand size of meshes array
	if (_cxt.count == _cxt.maxCount)
	{
		if (_cxt.meshes == NULL)
		{
			// Allocate initial list
			_cxt.maxCount = START_MESH_COUNT;
			_cxt.meshes = (Mesh*)malloc(sizeof(Mesh) * _cxt.maxCount);
		}
		else
		{
			// Double size of existing one
			_cxt.maxCount *= 2;
			Mesh *newList = (Mesh*)malloc(sizeof(Mesh) * _cxt.maxCount);
			memcpy(newList, _cxt.meshes, sizeof(Mesh) * _cxt.count);
			free(_cxt.meshes);
			_cxt.meshes = newList;
		}
	}

	// Fill mesh data
	Mesh *mesh = &_cxt.meshes[_cxt.count];
	mesh->maxSize = START_VERTEX_COUNT;
	mesh->vertices = (srVertex*)malloc(sizeof(srVertex) * mesh->maxSize);
	mesh->size = 0;
}

void srEnd()
{
	// Increment size
	_cxt.count++;
}

void srAddVertex(float x, float y, float z)
{
	Mesh *mesh = &_cxt.meshes[_cxt.count];

	// Expand size of vertices array
	if (mesh->size == mesh->maxSize)
	{
		mesh->maxSize *= 2;
		srVertex *newVertices = (srVertex*)malloc(sizeof(srVertex) * mesh->maxSize);
		memcpy(newVertices, mesh->vertices, sizeof(srVertex) * mesh->size);
		free(mesh->vertices);
		mesh->vertices = newVertices;
	}

	// Add vertex
	kmVec3 *p = &mesh->vertices[mesh->size].p;
	p->x = x;
	p->y = y;
	p->z = z;

	// Increment vertex counter
	mesh->size++;
}

void srDrawAll()
{
	// For each mesh
	for (int m = 0; m < _cxt.count; ++m)
	{
		Mesh *mesh = &_cxt.meshes[m];

		// Draw a single vertex
		if (mesh->size == 1)
			srPoint(mesh->vertices[0].p.x, mesh->vertices[0].p.y, 0xffffffff);

		// Draw a line
		if (mesh->size == 2)
			srLine(mesh->vertices[0].p.x, mesh->vertices[0].p.y, mesh->vertices[1].p.x, mesh->vertices[1].p.y, 0xffffffff);

		// Draw a triangle strip
		if (mesh->size > 2)
		{
			// Take first two vertices
			srVertex *v0 = &mesh->vertices[0];
			srVertex *v1 = &mesh->vertices[1];

			// Cycle through remaining vertices
			for (int v = 2; v < mesh->size; ++v)
			{
				srVertex *vc = &mesh->vertices[v];
				srDrawTriangle(v0, v1, vc);

				// Advance first two vertices
				v0 = v1;
				v1 = vc;
			}
		}
	}

	// Clear meshes
	_cxt.count = 0;
}

void srDrawTriangle(srVertex *a, srVertex *b, srVertex *c)
{
	// Draw wireframe
	if (_cxt.states[SR_WIREFRAME] == SR_TRUE)
	{
		srLine(a->p.x, a->p.y, b->p.x, b->p.y, 0xffffffff);
		srLine(b->p.x, b->p.y, c->p.x, c->p.y, 0xffffffff);
		srLine(c->p.x, c->p.y, a->p.x, a->p.y, 0xffffffff);
	}
	else // Draw filled
	{
	}
}