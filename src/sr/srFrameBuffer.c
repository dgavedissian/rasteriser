// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "srCommon.h"
#include "srContext.h"
#include "srFrameBuffer.h"

#include <unistd.h> // for usleep 
#include <sys/time.h> // for gettimeofday

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

void srSetMaxFPS(uint fps)
{
  _fb.frametime = 1000000 / fps;
}

int srContextActive()
{
  return _srCtxActive();
}

void srBeginFrame(uint32_t colour)
{
  _fb.framestart = getMicroseconds();
  _srCtxBegin();
  _srCtxClear(colour);
}

void srPutPixel(uint x, uint y, uint32_t colour)
{
  _srCtxPutPixel(x, y, colour);
}

void srEndFrame()
{
  _srCtxEnd();

  // Calculate frametime
  uint64_t diff = getMicroseconds() - _fb.framestart;
  if (diff < _fb.frametime)
    usleep(_fb.frametime - diff);
}

void _srFBCreate(uint* width, uint* height)
{
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
