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

void _srCtxRequest(uint* width, uint* height)
{
  // Create the window
  SDL_Init(SDL_INIT_EVERYTHING);
  _sdl.width = *width;
  _sdl.height = *height;
  _sdl.surface = SDL_SetVideoMode(_sdl.width, _sdl.height, 32, SDL_SWSURFACE);
  _sdl.active = 1;

  // Set the window caption
  SDL_WM_SetCaption("Rasteriser", NULL);
}

void _srCtxDestroy()
{
  SDL_FreeSurface(_sdl.surface);
  SDL_Quit();
  _sdl.surface = 0;
  _sdl.active = 0;
}

int _srCtxActive()
{
  return _sdl.active;
}

void _srCtxClear(uint32_t colour)
{
  int pixel = SDL_MapRGBA(_sdl.surface->format,
      SR_HEX_GETR(colour),
      SR_HEX_GETG(colour),
      SR_HEX_GETB(colour),
      SR_HEX_GETA(colour));
  SDL_FillRect(_sdl.surface, NULL, pixel);
}

void _srCtxBegin()
{
  assert(_sdl.surface);
  
  // Perform message pump
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT)
      _sdl.active = 0;
  }

  // Lock the output surface
  if (SDL_MUSTLOCK(_sdl.surface))
    SDL_LockSurface(_sdl.surface);
}

void _srCtxPutPixel(uint x, uint y, uint32_t colour)
{
  assert(_sdl.surface);
  assert(x < _sdl.width && y < _sdl.height);

  SDL_Surface* surface = _sdl.surface;
  int bpp = surface->format->BytesPerPixel;
  int pixel = SDL_MapRGBA(surface->format,
      SR_HEX_GETR(colour),
      SR_HEX_GETG(colour),
      SR_HEX_GETB(colour),
      SR_HEX_GETA(colour));

  // Here p is the address to the pixel we want to set
  uint8_t* p = (uint8_t*)surface->pixels + y * surface->pitch + x * bpp;
  switch (bpp)
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
      if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
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

    default:
      break;
  }
}

void _srCtxEnd()
{
  assert(_sdl.surface);

  // Unlock and display
  if (SDL_MUSTLOCK(_sdl.surface))
    SDL_UnlockSurface(_sdl.surface);
  SDL_Flip(_sdl.surface);
}
