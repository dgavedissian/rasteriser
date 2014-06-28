// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "srCommon.h"
#include "srContext.h"
#include "srFrameBuffer.h"
#include "srRasteriser.h"

#include <unistd.h> // for usleep 
#include <sys/time.h> // for gettimeofday

// Frame buffer data
struct
{
  uint32_t* pixels;
  uint width, height;
  uint32_t frametime;
} _fb;

void srCreateFrameBuffer(uint width, uint height)
{
  _fb.pixels = (uint32_t*)malloc(sizeof(uint32_t) * width * height);
  _fb.width = width;
  _fb.height = height;
  _fb.frametime = 0;
  srClear(0);

  _srCreateContext(width, height);
  _srCreateRasteriser();
}

void srDestroyFrameBuffer()
{
  _srDestroyRasteriser();
  _srDestroyContext();

  free(_fb.pixels);
}

void srSetMaxFPS(uint fps)
{
  _fb.frametime = 1000000 / fps;
}

void srClear(uint32_t colour)
{
  memset(_fb.pixels, colour, sizeof(uint32_t) * _fb.width * _fb.height);
}

void srDrawPixel(uint x, uint y, uint32_t colour)
{
  assert(x < _fb.width && y < _fb.height);
  _fb.pixels[y * _fb.width + x] = colour;
}

uint64_t getMicroseconds()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec) * 1000000 + (tv.tv_usec);
}

void srPresent()
{
  uint64_t frameStart = getMicroseconds();
  _srCopyFramebuffer();
  uint64_t diff = getMicroseconds() - frameStart;
  if (diff < _fb.frametime)
  {
    usleep(_fb.frametime - diff);
  }
}

uint32_t* _srGetPixels()
{
  return _fb.pixels;
}

uint _srGetWidth()
{
  return _fb.width;
}

uint _srGetHeight()
{
  return _fb.height;
}
