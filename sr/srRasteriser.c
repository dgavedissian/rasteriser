// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "srCommon.h"
#include "srFrameBuffer.h"
#include "srRasteriser.h"

#define INITIAL_MAX_VERTEX_COUNT 10

// Rasteriser data
struct
{
    // Render states
    unsigned int states[SR_RENDER_STATE_COUNT];

    // Matrices
    kmMat4 modelView, proj;
} _r;

// Immediate mode data
// TODO: eventually we can use some VBO here once thats implemented
struct
{
    srVertex* vertices;
    unsigned int size, maxSize;
} _im;

void _srCreateRasteriser()
{
    // Default render states
    _r.states[SR_WIREFRAME] = SR_FALSE;

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

void srBegin()
{
    // TODO: set primitive type..?
}

void srAddVertex(float x, float y, float z, int colour)
{
    // Expand size of vertices array if we need more by
    // creating a larger vertex array then copy old data in
    if (_im.size == _im.maxSize)
    {
        _im.maxSize *= 2;
        srVertex* newVertices = (srVertex*)malloc(sizeof(srVertex) * _im.maxSize);
        memcpy(newVertices, _im.vertices, sizeof(srVertex) * _im.size);
        free(_im.vertices);
        _im.vertices = newVertices;
    }

    // Set vertex data
    srVertex* v = &_im.vertices[_im.size];
    v->p.x = x;
    v->p.y = y;
    v->p.z = z;
    v->c = colour;
    _im.size++;
}

void srEnd()
{
    // Render
    if (_im.size > 2)
    {
        // Build final transform matrix
        kmMat4 t;
        kmMat4Multiply(&t, &_r.proj, &_r.modelView);

        // =====================================
        // Stage 1: Transform vertices
        // =====================================

        // kmVec3TransformCoord both transforms the vertex and divides
        // each component by w
        for (int v = 0; v < _im.size; ++v)
            kmVec3TransformCoord(&_im.vertices[v].p, &_im.vertices[v].p, &t);

        // =====================================
        // Stage 2: Rasterise
        // =====================================

        // At the moment, we're rendering with the trangle strip
        // method

        // Take first two vertices
        srVertex *v0 = &_im.vertices[0];
        srVertex *v1 = &_im.vertices[1];

        // Cycle through remaining vertices
        for (int v = 2; v < _im.size; ++v)
        {
            srVertex *vc = &_im.vertices[v];
            srDrawTriangle(v0, v1, vc);

            // Advance first two vertices
            v0 = v1;
            v1 = vc;
        }
    }

    // Reset
    _im.size = 0;
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

float projXToScreen(float x)
{
    return (x + 1.0f) * 0.5f * _srGetWidth();
}

float projYToScreen(float y)
{
    return (-y + 1.0f) * 0.5f * _srGetHeight();
}

// Pre: Vertices are assumed to be homogeneous coordinates
void srDrawLine(srVertex* a, srVertex* b)
{
    float x1 = projXToScreen(a->p.x), y1 = projYToScreen(a->p.y);
    float x2 = projXToScreen(b->p.x), y2 = projYToScreen(b->p.y);
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
