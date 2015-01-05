// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "srCommon.h"
#include "srFrameBuffer.h"

#include "srContextSDL.h"
#include "srContextTerm.h"

#include <unistd.h>      // for usleep
#include <sys/time.h>    // for gettimeofday

// Context function pointers
void (*_srCtxRequest)(uint* width, uint* height);
void (*_srCtxDestroy)();
int (*_srCtxActive)();
void (*_srCtxClear)(uint32_t colour);
void (*_srCtxBegin)();
void (*_srCtxPutPixel)(uint x, uint y, uint32_t colour);
void (*_srCtxEnd)();

struct
{
    uint width, height;
    uint64_t framestart;
    uint32_t frametime;
} _fb;

uint64_t getMicroseconds()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

void _srFBCreate(uint* width, uint* height, srEnum outputContext)
{
    // Set up context function pointers
    switch (outputContext)
    {
    case SR_CTX_SDL:
        _srCtxRequest = &_srSDLCtxRequest;
        _srCtxDestroy = &_srSDLCtxDestroy;
        _srCtxActive = &_srSDLCtxActive;
        _srCtxClear = &_srSDLCtxClear;
        _srCtxBegin = &_srSDLCtxBegin;
        _srCtxPutPixel = &_srSDLCtxPutPixel;
        _srCtxEnd = &_srSDLCtxEnd;
        break;

    case SR_CTX_TERM:
        _srCtxRequest = &_srTermCtxRequest;
        _srCtxDestroy = &_srTermCtxDestroy;
        _srCtxActive = &_srTermCtxActive;
        _srCtxClear = &_srTermCtxClear;
        _srCtxBegin = &_srTermCtxBegin;
        _srCtxPutPixel = &_srTermCtxPutPixel;
        _srCtxEnd = &_srTermCtxEnd;
        break;

    default:
        assert(0 && "Must enter a context type");
    }

    _srCtxRequest(width, height);
    _fb.width = *width;
    _fb.height = *height;
    _fb.framestart = 0;
    _fb.frametime = 0;
}

void _srFBDestroy()
{
    _srCtxDestroy();
}

void srSetMaxFPS(uint fps)
{
    _fb.frametime = 1000000 / fps;
}

int srContextActive()
{
    return _srCtxActive();
}

void srBegin(uint32_t colour)
{
    _fb.framestart = getMicroseconds();
    _srCtxBegin();
    _srCtxClear(colour);
}

void srPutPixel(uint x, uint y, uint32_t colour)
{
    _srCtxPutPixel(x, y, colour);
}

void srEnd()
{
    _srCtxEnd();

    // Calculate frametime
    uint64_t diff = getMicroseconds() - _fb.framestart;
    if (diff < _fb.frametime)
        usleep(_fb.frametime - diff);
}
