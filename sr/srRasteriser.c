// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "srCommon.h"
#include "srFrameBuffer.h"
#include "srRasteriser.h"

#define INITIAL_MAX_MESH_COUNT 10
#define INITIAL_MAX_VERTEX_COUNT 10

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
    _r.states[SR_WIREFRAME] = SR_FALSE;
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

void srSetRenderState(unsigned int state, unsigned int value)
{
    assert(state < SR_RENDER_STATE_COUNT);
    _r.states[state] = value;
}

void srBegin()
{
    // Expand mesh count if need be
    if (_r.count == _r.maxCount)
    {
        if (_r.meshes == NULL)
        {
            // Allocate initial list
            _r.maxCount = INITIAL_MAX_MESH_COUNT;
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
    mesh->maxSize = INITIAL_MAX_VERTEX_COUNT;
    mesh->vertices = (srVertex*)malloc(sizeof(srVertex) * mesh->maxSize);
    mesh->size = 0;
}

void srEnd()
{
    // Increase mesh count
    _r.count++;
}

void srAddVertex(float x, float y, float z, int colour)
{
    srMesh *mesh = &_r.meshes[_r.count];

    // Expand size of vertices array if we need more
    if (mesh->size == mesh->maxSize)
    {
        mesh->maxSize *= 2;
        srVertex* newVertices = (srVertex*)malloc(sizeof(srVertex) * mesh->maxSize);
        memcpy(newVertices, mesh->vertices, sizeof(srVertex) * mesh->size);
        free(mesh->vertices);
        mesh->vertices = newVertices;
    }

    // Set vertex data
    srVertex* v = &mesh->vertices[mesh->size];
    v->p.x = x;
    v->p.y = y;
    v->p.z = z;
    v->c = colour;
    mesh->size++;
}

uint32_t lerpColour(uint32_t a, uint32_t b, float x)
{
    int ra = SR_GET_R(a), ga = SR_GET_G(a), ba = SR_GET_B(a), aa = SR_GET_A(a);
    int rb = SR_GET_R(b), gb = SR_GET_G(b), bb = SR_GET_B(b), ab = SR_GET_A(b);
    return SR_RGBA(
        (int)(ra + (rb - ra) * x),
        (int)(ga + (gb - ga) * x),
        (int)(ba + (bb - ba) * x),
        (int)(aa + (ab - aa) * x)
        );
}

// Pre: Vertices are assumed to be homogeneous coordinates
void srDrawLine(srVertex* a, srVertex* b)
{
    float x1 = a->p.x, y1 = a->p.y;
    float x2 = b->p.x, y2 = b->p.y;
    uint32_t c1 = a->c, c2 = b->c;

    float dx = x2 - x1;
    float dy = y2 - y1;

    // TODO: fp comparison, global calamity ensues
    if (dx == 0.0f && dy == 0.0f)
    {
        srDrawPixel(x1, y1, a->c);
        return;
    }

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
            uint32_t colour = lerpColour(c1, c2, (x - x1) / dx);
            srDrawPixel(x, y, colour);
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
            uint32_t colour = lerpColour(c1, c2, (y - y1) / dy);
            srDrawPixel(x, y, colour);
        }
    }
}

void srDrawTriangle(srVertex *a, srVertex *b, srVertex *c)
{
    // Edges: a-b, b-c, c-a
    if (_r.states[SR_WIREFRAME] == SR_TRUE)
    {
        srDrawLine(a, b);
        srDrawLine(b, c);
        srDrawLine(c, a);
    }
    else
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
            srDrawLine(&mesh->vertices[0], &mesh->vertices[1]);

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