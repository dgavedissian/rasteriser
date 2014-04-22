// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "srCommon.h"
#include "srFrameBuffer.h"
#include "srContext.h"

// Terminal data
struct
{
    unsigned int width, height, lineCount;
} _term;

// Convert a colour into a character
#define LEVEL(N, C) if (level == N) return C
char intensityToChar(int intensity)
{
    // Map brightness levels to characters
    int level = (int)((intensity / 255.0f) * 16.0f);
    LEVEL(1, '.');
    LEVEL(2, ',');
    LEVEL(3, ':');
    LEVEL(4, '~');
    LEVEL(5, '=');
    LEVEL(6, '+');
    LEVEL(7, '?');
    LEVEL(8, 'I');
    LEVEL(9, '7');
    LEVEL(10, 'Z');
    LEVEL(11, 'O');
    LEVEL(12, '8');
    LEVEL(13, 'N');
    LEVEL(14, 'M');
    LEVEL(15, '#');
    LEVEL(16, '@');
    return ' ';
}

void _srCreateContext(unsigned int width, unsigned int height)
{
    _term.width = width;
    _term.height = height;
    _term.lineCount = 0;
}

void _srDestroyContext()
{
}

void _srWriteFBToContext()
{
    // Move cursor to the correct position
    printf("\033[%dA", _term.lineCount);
    _term.lineCount = 0;

    // Print everything
    for (int y = 0; y < _term.height; ++y)
    {
        for (int x = 0; x < _term.width; ++x)
        {
            // Figure out which character to use from the colour
            uint32_t colour = _srGetPixels()[y * _srGetWidth() + x];
            char c = intensityToChar((SR_HEX_GETR(colour) + SR_HEX_GETG(colour) + SR_HEX_GETB(colour)) / 3);

            // Calculate colour
            // TODO

            // Print twice to achieve a more square pixel aspect ratio
            putchar(c); putchar(c);
        }
        putchar('\n');
        _term.lineCount++;
    }
}