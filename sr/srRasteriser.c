// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "srCommon.h"
#include "srFrameBuffer.h"
#include "srRasteriser.h"

#define START_MESH_COUNT 10
#define START_VERTEX_COUNT 10

// Mesh object
typedef struct
{
    srVertex* vertices;
    unsigned int size, maxSize;
} srMesh;

// Rasteriser data
struct
{
    // Meshes
    srMesh* meshes;
    unsigned int count, maxCount;

    // Render states
    unsigned int states[SR_RENDER_STATE_COUNT];
} _r;

void _srCreateRasteriser()
{
    _r.meshes = NULL;
    _r.count = 0;
    _r.maxCount = 0;

    // Default render states
    _r.states[SR_WIREFRAME] = SR_TRUE;
}

void _srDestroyRasteriser()
{
    if (_r.meshes != NULL)
    {
        for (int i = 0; i < _r.maxCount; ++i)
            free(_r.meshes[i].vertices);
        free(_r.meshes);
    }
}

void setRenderState(unsigned int state, unsigned int value)
{
    assert(state < SR_RENDER_STATE_COUNT);
    _r.states[state] = value;
}

void srBegin()
{
    // Expand size of meshes array
    if (_r.count == _r.maxCount)
    {
        if (_r.meshes == NULL)
        {
            // Allocate initial list
            _r.maxCount = START_MESH_COUNT;
            _r.meshes = (srMesh*)malloc(sizeof(srMesh) * _r.maxCount);
        }
        else
        {
            // Double size of existing one
            _r.maxCount *= 2;
            srMesh* newList = (srMesh*)malloc(sizeof(srMesh) * _r.maxCount);
            memcpy(newList, _r.meshes, sizeof(srMesh) * _r.count);
            free(_r.meshes);
            _r.meshes = newList;
        }
    }

    // Fill mesh data
    srMesh* mesh = &_r.meshes[_r.count];
    mesh->maxSize = START_VERTEX_COUNT;
    mesh->vertices = (srVertex*)malloc(sizeof(srVertex) * mesh->maxSize);
    mesh->size = 0;
}

void srEnd()
{
    // Increment size
    _r.count++;
}

void srAddVertex(float x, float y, float z, int colour)
{
    srMesh *mesh = &_r.meshes[_r.count];

    // Expand size of vertices array
    if (mesh->size == mesh->maxSize)
    {
        mesh->maxSize *= 2;
        srVertex* newVertices = (srVertex*)malloc(sizeof(srVertex) * mesh->maxSize);
        memcpy(newVertices, mesh->vertices, sizeof(srVertex) * mesh->size);
        free(mesh->vertices);
        mesh->vertices = newVertices;
    }

    // Add vertex
    kmVec3* p = &mesh->vertices[mesh->size].p;
    p->x = x;
    p->y = y;
    p->z = z;

    // Increment vertex counter
    mesh->size++;
}

void srDrawLine(int x1, int y1, int x2, int y2, int colour)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int dxabs = abs(dx);
    int dyabs = abs(dy);
    int sdx = SR_SGN(dx);
    int sdy = SR_SGN(dy);
    int x = dyabs >> 1;
    int y = dxabs >> 1;
    int px = x1;
    int py = y1;

    srDrawPixel(px, py, colour);
    if (dxabs >= dyabs) // the line is more horizontal than vertical
    {
        for (int i = 0; i < dxabs; ++i)
        {
            y += dyabs;
            if (y >= dxabs)
            {
                y -= dxabs;
                py += sdy;
            }
            px += sdx;
            srDrawPixel(px, py, colour);
        }
    }
    else // the line is more vertical than horizontal
    {
        for (int i = 0; i < dyabs; i++)
        {
            x += dxabs;
            if (x >= dyabs)
            {
                x -= dyabs;
                px += sdx;
            }
            py += sdy;
            srDrawPixel(px, py, colour);
        }
    }
}

void srDrawTriangle(srVertex *a, srVertex *b, srVertex *c)
{
    // Draw wireframe
    if (_r.states[SR_WIREFRAME] == SR_TRUE)
    {
        srDrawLine(a->p.x, a->p.y, b->p.x, b->p.y, 0xffffffff);
        srDrawLine(b->p.x, b->p.y, c->p.x, c->p.y, 0xffffffff);
        srDrawLine(c->p.x, c->p.y, a->p.x, a->p.y, 0xffffffff);
    }
    else // Draw filled
    {
    }
}

void _srRasteriseScene()
{
    // For each mesh
    for (int m = 0; m < _r.count; ++m)
    {
        srMesh *mesh = &_r.meshes[m];

        // Draw a single vertex
        if (mesh->size == 1)
            srDrawPixel(mesh->vertices[0].p.x, mesh->vertices[0].p.y, 0xffffffff);

        // Draw a line
        if (mesh->size == 2)
            srDrawLine(mesh->vertices[0].p.x, mesh->vertices[0].p.y, mesh->vertices[1].p.x, mesh->vertices[1].p.y, 0xffffffff);

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
    _r.count = 0;
}