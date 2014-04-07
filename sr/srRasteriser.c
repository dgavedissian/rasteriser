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
    unsigned int primitive;
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

void srBegin(unsigned int primitiveType)
{
    _im.primitive = primitiveType;
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
    if (_im.size == 0)
        return;

    srVertex* v = _im.vertices;

    // Build final transform matrix
    kmMat4 t;
    kmMat4Multiply(&t, &_r.proj, &_r.modelView);

    // =====================================
    // Stage 1: Transform vertices
    // =====================================

    // kmVec3TransformCoord both transforms the vertex and divides
    // each component by w
    for (int i = 0; i < _im.size; ++i)
    {
        // World space -> Normalised device coordinates
        kmVec3TransformCoord(&v[i].p, &v[i].p, &t);

        // Normalised device coordinates -> Viewport coordinates
        v[i].p.x = (v[i].p.x + 1.0f) * 0.5f * _srGetWidth();
        v[i].p.y = (-v[i].p.y + 1.0f) * 0.5f * _srGetHeight();
    }

    // =====================================
    // Stage 2: Rasterise
    // =====================================

    // Point List
    if (_im.primitive == SR_POINT_LIST)
    {
        for (int i = 0; i < _im.size; ++i)
            srDrawPixel(v[i].p.x, v[i].p.y, v[i].c);
    }

    // Line List
    if (_im.primitive == SR_LINE_LIST && (_im.size & 2) == 0)
    {
        // Cycle through lines
        for (int l = 0; l < _im.size / 2; ++l)
            srDrawLine(&v[l * 2], &v[l * 2 + 1]);
    }

    // Line Strip
    if (_im.primitive == SR_LINE_STRIP && _im.size > 1)
    {
        // Take first vertex
        srVertex* vp = &v[0];

        // Cycle through remaining vertices
        for (int i = 1; i < _im.size; ++i)
        {
            srVertex* vc = &v[i];
            srDrawLine(vp, vc);
            vp = vc;
        }
    }

    // Triangle List
    if (_im.primitive == SR_TRIANGLE_LIST && (_im.size % 3) == 0)
    {
        // Cycle through triangles
        for (int t = 0; t < _im.size / 3; ++t)
            srDrawTriangle(&v[t * 3], &v[t * 3 + 1], &v[t * 3 + 2]);
    }

    // Triangle Strip
    if (_im.primitive == SR_TRIANGLE_STRIP && _im.size > 2)
    {
        // Take first two vertices
        srVertex *v0 = &v[0];
        srVertex *v1 = &v[1];

        // Cycle through remaining vertices
        for (int i = 2; i < _im.size; ++i)
        {
            srVertex *vc = &v[i];
            srDrawTriangle(v0, v1, vc);
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

typedef struct
{
    srVertex* v1;
    srVertex* v2;
} srEdge;

typedef struct
{
    float x1, x2;
    uint32_t c1, c2;
} srSpan;

void buildEdge(srEdge* e, srVertex* v1, srVertex* v2)
{
    if (v1->p.y < v2->p.y)
    {
        e->v1 = v1;
        e->v2 = v2;
    }
    else
    {
        e->v1 = v2;
        e->v2 = v1;
    }
}

void buildSpan(srSpan* s, float x1, float x2, uint32_t c1, uint32_t c2)
{
    if (x1 < x2)
    {
        s->x1 = x1;
        s->x2 = x2;
        s->c1 = c1;
        s->c2 = c2;
    }
    else
    {
        s->x1 = x2;
        s->x2 = x1;
        s->c1 = c2;
        s->c2 = c1;
    }
}

void drawSpan(srSpan* span, int y)
{
    int xdiff = span->x2 - span->x1;
    if (xdiff == 0)
        return;

    float factor = 0.0f;
    float factorStep = 1.0f / (float)xdiff;

    // Draw each pixel in the span
    for (int x = span->x1; x < span->x2; ++x)
    {
        srDrawPixel(x, y, lerpColour(span->c1, span->c2, factor));
        factor += factorStep;
    }
}

void drawSpansBetweenEdges(srEdge* e1, srEdge* e2)
{
    // Calculate difference between the y coordinates
    // of the first edge and return if 0
    float e1ydiff = (float)(e1->v2->p.y - e1->v1->p.y);
    if (e1ydiff == 0.0f)
        return;

    // Calculate difference between the y coordinates
    // of the second edge and return if 0
    float e2ydiff = (float)(e2->v2->p.y - e2->v1->p.y);
    if(e2ydiff == 0.0f)
        return;

    // Calculate differences between the x coordinates
    float e1xdiff = (float)(e1->v2->p.x - e1->v1->p.x);
    float e2xdiff = (float)(e2->v2->p.x - e2->v1->p.x);

    // Calculate factors to use for interpolation
    // with the edges and the step values to increase
    // them by after drawing each span
    float factor1 = (float)(e2->v1->p.y - e1->v1->p.y) / e1ydiff;
    float factorStep1 = 1.0f / e1ydiff;
    float factor2 = 0.0f;
    float factorStep2 = 1.0f / e2ydiff;

    // Loop through the lines between the edges and draw spans
    for (int y = e2->v1->p.y; y < e2->v2->p.y; ++y)
    {
        // Draw span
        srSpan span;
        buildSpan(&span,
            e1->v1->p.x + (int)(e1xdiff * factor1),
            e2->v1->p.x + (int)(e2xdiff * factor2),
            lerpColour(e1->v1->c, e1->v2->c, factor1),
            lerpColour(e2->v1->c, e2->v2->c, factor2));
        drawSpan(&span, y);

        // Increase factors
        factor1 += factorStep1;
        factor2 += factorStep2;
    }
}

void srDrawTriangle(srVertex *a, srVertex *b, srVertex *c)
{
    // Store the edges
    srEdge edges[3];
    buildEdge(&edges[0], a, b);
    buildEdge(&edges[1], b, c);
    buildEdge(&edges[2], c, a);

    if (_r.states[SR_WIREFRAME] == SR_TRUE)
    {
        // Just draw the edges
        for (int i = 0; i < 3; ++i)
            srDrawLine(edges[i].v1, edges[i].v2);
    }
    else
    {
        int maxLength = 0;
        int longEdge = 0;

        // Find the edge with the greatest length in the y axis
        for (int i = 0; i < 3; ++i)
        {
            int length = edges[i].v2->p.y - edges[i].v1->p.y;
            if (length > maxLength)
            {
                maxLength = length;
                longEdge = i;
            }
        }

        int shortEdge1 = (longEdge + 1) % 3;
        int shortEdge2 = (longEdge + 2) % 3;

        // Draw spans between edges
        drawSpansBetweenEdges(&edges[longEdge], &edges[shortEdge1]);
        drawSpansBetweenEdges(&edges[longEdge], &edges[shortEdge2]);
    }
}
