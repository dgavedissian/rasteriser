// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "srCommon.h"
#include "srContextTerm.h"

#include <sys/ioctl.h>
#include <unistd.h>

// Terminal data
struct
{
    uint width, height;
    uint32_t* pixels;
} _term;

// Convert a colour into a character
static char intensityToChar(uint intensity)
{
    assert(intensity <= 255);

    // Determine brightness level
    char levels[] = {' ', '.', ',', ':', '~', '=', '+', '?', 'I',
                     '7', 'Z', 'O', '8', 'N', 'M', '#', '@'};
    return levels[(uint)(intensity / 255.0f * 16.0f)];
}

static float fltSq(int i)
{
    return (float)(i * i);
}

static float getColourDistanceSq(uint32_t a, uint32_t b)
{
    int r1, g1, b1, r2, g2, b2;
    r1 = SR_HEX_GETR(a);
    g1 = SR_HEX_GETG(a);
    b1 = SR_HEX_GETB(a);
    r2 = SR_HEX_GETR(b);
    g2 = SR_HEX_GETG(b);
    b2 = SR_HEX_GETB(b);
    return fltSq(r1 - r2) + fltSq(g1 - g2) + fltSq(b1 - b2);
}

static void generateColourCode(char* buffer, uint32_t rgb)
{
    // ANSI RGB colours
    uint32_t ansirgb[] = {// Primary 3-bit (8 colours)
                          0x000000ff, 0x800000ff, 0x008000ff, 0x808000ff,
                          0x000080ff, 0x800080ff, 0x008080ff, 0xc0c0c0ff,

                          // Equivalent "bright" versions of the original 8 colours
                          0x808080ff, 0xff0000ff, 0x00ff00ff, 0xffff00ff,
                          0x0000ffff, 0xff00ffff, 0x00ffffff, 0xffffffff};

    // Search for the closest colour
    float minDist = 10000000000;
    int minIndex;
    for (int i = 0; i < 16; ++i)
    {
        int dist = getColourDistanceSq(rgb, ansirgb[i]);
        if (dist < minDist)
        {
            minDist = dist;
            minIndex = i;
        }
    }

    // Fill out the buffer
    snprintf(buffer, 5, "%d;3%d", minIndex / 8, minIndex % 8);
}

void _srTermCtxRequest(uint* width, uint* height)
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

    // Allocate backbuffer
    srSize backbufferSize = sizeof(uint32_t) * _term.width * _term.height;
    _term.pixels = (uint32_t*)malloc(backbufferSize);
    memset(_term.pixels, 0, backbufferSize);
}

void _srTermCtxDestroy()
{
    free(_term.pixels);
}

int _srTermCtxActive()
{
    return 1;
}

void _srTermCtxClear(uint32_t colour)
{
    memset(_term.pixels, colour, sizeof(uint32_t) * _term.width * _term.height);
}

void _srTermCtxBegin()
{
}

void _srTermCtxPutPixel(uint x, uint y, uint32_t colour)
{
    assert(x < _term.width && y < _term.height);
    _term.pixels[y * _term.width + x] = colour;
}

void _srTermCtxEnd()
{
    // Move cursor to the beginning
    printf("\033[%dA", _term.height);

    // Print each pixel
    for (int y = 0; y < _term.height; ++y)
    {
        for (int x = 0; x < _term.width; ++x)
        {
            // Figure out which character to use from the colour
            uint32_t colour = _term.pixels[y * _term.width + x];
            char repr = intensityToChar(
                (SR_HEX_GETR(colour) + SR_HEX_GETG(colour) + SR_HEX_GETB(colour)) / 3);

            // Draw the pixel
            char colourCode[5];
            generateColourCode(colourCode, colour);
            printf("\033[%sm%c%c", colourCode, repr, repr);
        }
        printf("\n");
    }
}
