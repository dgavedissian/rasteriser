// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "srCommon.h"
#include "srFrameBuffer.h"
#include "srContext.h"

// Console data
struct
{
    unsigned int width, height, lineCount;
} _console;

// Convert a colour into a character
#define LEVEL(N, C) case N: charOut = C; break
#define LEVEL_DEFAULT(C) default: charOut = C; break
char colourToChar(int colour)
{
    // Convert rgb into brightness levels
    int intensity = (SR_GET_R(colour) + SR_GET_G(colour) + SR_GET_B(colour)) / 3;
    int level = (int)((intensity / 255.0f) * 16.0f);

    // Map brightness levels to characters
    char charOut;
    switch (level)
    {
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
        LEVEL_DEFAULT(' ');
    }
    return charOut;
}

void _srCreateContext(unsigned int width, unsigned int height)
{
    _console.width = width;
    _console.height = height;
    _console.lineCount = 0;
}

void _srDestroyContext()
{
}

void _srPresent()
{
    // Move cursor to the correct position
    printf("\033[%dA", _console.lineCount);
    _console.lineCount = 0;

    // Print everything
    for (int y = 0; y < _console.height; ++y)
    {
        for (int x = 0; x < _console.width; ++x)
        {
            // Char needs to be printed twice so the pixel aspect ratio
            // is more square
            char c = colourToChar(_srGetPixels()[y * _srGetWidth() + x]);
            putchar(c); putchar(c);
        }
        putchar('\n');
        _console.lineCount++;
    }
}