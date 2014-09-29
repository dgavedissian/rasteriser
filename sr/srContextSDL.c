// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "srCommon.h"
#include "srFrameBuffer.h"
#include "srContext.h"

#include <SDL.h>

// SDL data
struct
{
  uint width, height;
  SDL_Surface* surface;
  int active;
} _sdl;

// Set the pixel at (x, y) to the given value
void putPixel(SDL_Surface *surface, uint x, uint y, uint32_t pixel)
{
  int bpp = surface->format->BytesPerPixel;

  // Here p is the address to the pixel we want to set
  uint8_t* p = (uint8_t*)surface->pixels + y * surface->pitch + x * bpp;
  switch(bpp)
  {
    case 1:
    {
      *p = pixel;
      break;
    }

    case 2:
    {
      *(uint16_t*)p = pixel;
      break;
    }

    case 3:
    {
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
      {
        p[0] = (pixel >> 16) & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = pixel & 0xff;
      }
      else
      {
        p[0] = pixel & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = (pixel >> 16) & 0xff;
      }
      break;
    }

    case 4:
    {
      *(uint32_t*)p = pixel;
      break;
    }
  }
}

int srContextActive()
{
  return _sdl.active;
}

void _srCreateContext(uint width, uint height)
{
  // Create the window
  SDL_Init(SDL_INIT_EVERYTHING);
  _sdl.width = width;
  _sdl.height = height;
  _sdl.surface = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);
  _sdl.active = 1;

  // Set the window caption
  SDL_WM_SetCaption("SR - SDL Context", NULL);
}

void _srDestroyContext()
{
  SDL_FreeSurface(_sdl.surface);
  SDL_Quit();
  _sdl.surface = 0;
  _sdl.active = 0;
}

void _srCopyFramebuffer()
{
  assert(_sdl.surface);

  // Handle all SDL events
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT)
    {
      _sdl.active = 0;
    }
  }

  // Lock the surface
  if (SDL_MUSTLOCK(_sdl.surface))
  {
    SDL_LockSurface(_sdl.surface);
  }

  // Copy the pixels to SDL
  uint32_t* pixels = _srGetPixels();
  for (uint y = 0; y < _sdl.height; ++y)
  {
    for (uint x = 0; x < _sdl.width; ++x)
    {
      putPixel(_sdl.surface, x, y, *(pixels + y * _sdl.width + x));
    }
  }

  // Unlock the surface
  if (SDL_MUSTLOCK(_sdl.surface))
  {
    SDL_UnlockSurface(_sdl.surface);
  }

  // Display to screen
  SDL_Flip(_sdl.surface);
}
