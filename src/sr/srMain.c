// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "srCommon.h"
#include "srFrameBuffer.h"
#include "srRasteriser.h"
#include "srMain.h"

struct
{
    srSize width, height;
} _sr;

void srInit(srInitParams* params)
{
    _sr.width = params->width;
    _sr.height = params->height;
    _srFBCreate(&_sr.width, &_sr.height, params->outputContext);
    _srCreateRasteriser(_sr.width, _sr.height);
}

void srShutdown()
{
    _srDestroyRasteriser();
    _srFBDestroy();
}

srSize srGetWidth()
{
    return _sr.width;
}

srSize srGetHeight()
{
    return _sr.height;
}
