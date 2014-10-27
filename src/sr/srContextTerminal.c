// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "srCommon.h"
#include "srFrameBuffer.h"
#include "srContext.h"

#include <sys/ioctl.h>
#include <unistd.h>

// Terminal data
struct
{
  uint width, height;
} _term;

// Convert a colour into a character
static char intensityToChar(uint intensity)
{
  //assert(intensity < 255);

  // Determine brightness level
  char levels[] = {
    ' ','.',',',':','~','=','+','?','I','7','Z','O','8','N','M','#','@'
  };
  return levels[(uint)(intensity / 255.0f * 16.0f)];
}

int srContextActive()
{
  return 1;
}

void _srRequestContext(uint* width, uint* height)
{
  // Get terminal dimensions
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  // Division by 2 due to each pixel being 2 characters wide
  int tw = w.ws_col / 2, th = w.ws_row - 1;

  // Clamp width and height
  *width = *width > tw ? tw : *width;
  *height = *height > th ? th : *height;

  // Save width and height
  _term.width = *width;
  _term.height = *height;
}

void _srDestroyContext()
{
}

void _srCopyFramebuffer()
{
  // Move cursor to the correct position
  printf("\033[%dA", _term.height);

  // Print everything
  for (int y = 0; y < _term.height; ++y)
  {
    for (int x = 0; x < _term.width; ++x)
    {
      // Figure out which character to use from the colour
      uint32_t colour = _srGetPixels()[y * _srGetWidth() + x];
      char repr = intensityToChar(
        (SR_HEX_GETR(colour) + SR_HEX_GETG(colour) + SR_HEX_GETB(colour)) / 3);

      // Calculate colour
      // TODO

      // Print twice to achieve a more square pixel aspect ratio
      putchar(repr); putchar(repr);
    }
    putchar('\n');
  }
}
