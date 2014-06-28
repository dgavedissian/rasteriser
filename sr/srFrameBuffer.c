// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "srCommon.h"
#include "srContext.h"
#include "srFrameBuffer.h"
#include "srRasteriser.h"
#include <unistd.h> // for usleep 

// Frame buffer data
struct
{
  uint32_t* pixels;
  uint width, height;
} _fb;

void srCreateFrameBuffer(uint width, uint height)
{
  _fb.pixels = (uint32_t*)malloc(sizeof(uint32_t) * width * height);
  _fb.width = width;
  _fb.height = height;
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

void srClear(uint32_t colour)
{
  memset(_fb.pixels, colour, sizeof(uint32_t) * _fb.width * _fb.height);
}

void srDrawPixel(uint x, uint y, uint32_t colour)
{
  if (x >= _fb.width || y >= _fb.height)
    return;

  _fb.pixels[y * _fb.width + x] = colour;
}

void srPresent()
{
  _srCopyFramebuffer();
  usleep(25 * 1000);
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
