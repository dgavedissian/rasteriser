// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "srCommon.h"
#include "srFrameBuffer.h"
#include "srRasteriser.h"
#include "srMain.h"

struct
{
  uint width, height;
} _sr;

void srInit(srInitParams* params)
{
  _sr.width = params->width;
  _sr.height = params->height;
  _srFBCreate(&_sr.width, &_sr.height);
  _srCreateRasteriser(_sr.width, _sr.height);
}

void srShutdown()
{
  _srDestroyRasteriser();
  _srFBDestroy();
}

