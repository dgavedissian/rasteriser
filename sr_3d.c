// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "sr_common.h"
#include "sr_math.h"
#include "sr_3d.h"

#define START_MESH_COUNT 10
#define START_VERTEX_COUNT 10

struct vertex_t
{
	vec3_t p;
};

struct mesh_t
{
	vertex_t *vertices;
	int size;
	int maxSize;
}

struct objects_t
{
	mesh_t *meshes;
	int count;
	int maxCount;
} _objects;

void srInit3D()
{
	meshes = NULL;
	count = 0;
	maxCount = 0;
}

void srBegin()
{
	// Expand size of meshes array
	if (_objects.count == _objects.maxCount)
	{
		if (_objects.meshes == NULL)
		{
			// Allocate initial list
			_objects.maxCount = START_MESH_COUNT;
			_objects.meshes = (mesh_t*)malloc(sizeof(mesh_t) * _objects.maxCount);
		}
		else
		{
			// Double size of existing one
			_objects.maxCount *= 2;
			mesh_t *newList = (mesh_t*)malloc(sizeof(mesh_t) * _objects.maxCount);
			memcpy(newList, _objects.meshes, _objects.count);
			free(_objects.meshes);
			_objects.meshes = newList;
		}
	}

	// Fill mesh data
	mesh_t *mesh = &_objects.meshes[_objects.count];
	mesh->maxSize = START_VERTEX_COUNT;
	mesh->vertices = (vertex_t*)malloc(sizeof(vertex_t) * mesh->maxSize);
	mesh->size = 0;
}

void srEnd()
{
	// Increment size
	_objects.count++;
}

void srVertex(float x, float y, float z)
{
	mesh_t *mesh = &_objects.meshes[_objects.count];

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
	for (int m = 0; m < _objects.count; ++m)
	{
		mesh_t *mesh = &_objects[m];

		// Draw a single vertex
		if (mesh->size == 1)
		{
			srPoint(mesh->vertices[0].x, mesh->vertices[0].y, 0xffffffff);
		}

		// Draw a line
		if (mesh->size == 2)
		{
			srLine(mesh->vertices[0].x, mesh->vertices[0].y, mesh->vertices[1].x, mesh->vertices[1].y, 0xffffffff);
		}

		// Draw a triangle strip
		if (mesh->size > 2)
		{
			// Take first two vertices
		}
	}

	// Clear meshes
	_objects.count = 0;
}

void srDrawTriangle(vec3_t *a, vec3_t *b, vec3_t *c)
{

}