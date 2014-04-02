// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "sr_common.h"
#include "sr_math.h"
#include "sr_context.h"
#include "sr_2d.h"
#include "sr_3d.h"

#define START_MESH_COUNT 10
#define START_VERTEX_COUNT 10

struct mesh_t
{
	vertex_t *vertices;
	int size;
	int maxSize;
};

// TODO: merge with sr_context.c's _cxt object
struct objects_t
{
	// Meshes
	mesh_t *meshes;
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
			_cxt.meshes = (mesh_t*)malloc(sizeof(mesh_t) * _cxt.maxCount);
		}
		else
		{
			// Double size of existing one
			_cxt.maxCount *= 2;
			mesh_t *newList = (mesh_t*)malloc(sizeof(mesh_t) * _cxt.maxCount);
			memcpy(newList, _cxt.meshes, _cxt.count);
			free(_cxt.meshes);
			_cxt.meshes = newList;
		}
	}

	// Fill mesh data
	mesh_t *mesh = &_cxt.meshes[_cxt.count];
	mesh->maxSize = START_VERTEX_COUNT;
	mesh->vertices = (vertex_t*)malloc(sizeof(vertex_t) * mesh->maxSize);
	mesh->size = 0;
}

void srEnd()
{
	// Increment size
	_cxt.count++;
}

void srVertex(float x, float y, float z)
{
	mesh_t *mesh = &_cxt.meshes[_cxt.count];

	// Expand size of vertices array
	if (mesh->size == mesh->maxSize)
	{
		mesh->maxSize *= 2;
		vertex_t *newVertices = (vertex_t*)malloc(sizeof(vertex_t) * mesh->maxSize);
		memcpy(newVertices, mesh->vertices, mesh->size);
		free(mesh->vertices);
		mesh->vertices = newVertices;
	}

	// Add vertex
	vec3_t *p = &mesh->vertices[mesh->size].p;
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
		mesh_t *mesh = &_cxt.meshes[m];

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
			vertex_t *v0 = &mesh->vertices[0];
			vertex_t *v1 = &mesh->vertices[1];

			// Cycle through remaining vertices
			for (int v = 2; v < mesh->size; ++v)
			{
				vertex_t *vc = &mesh->vertices[v];
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

void srDrawTriangle(vertex_t *a, vertex_t *b, vertex_t *c)
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