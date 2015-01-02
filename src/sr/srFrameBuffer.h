// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef SRFRAMEBUFFER_H
#define SRFRAMEBUFFER_H

/// Set the target frame rate
///
/// @param fps Target framerate in FPS
void srSetMaxFPS(uint fps);

/// Returns a non-zero value if the context is active
int srContextActive();

/// Begins a new frame and clears the backbuffer to the specified colour
///
/// @param colour The colour to clear the frame buffer with
void srBegin(uint32_t clearColour);

/// Sets a pixel in the frame buffer to a certain colour.
/// Pre: x < width && y < height
///
/// @param x The horizontal position of the pixel
/// @param y The vertical position of the pixel
/// @param colour The colour to set the pixel to
void srPutPixel(uint x, uint y, uint32_t colour);

/// Ends the frame and presents the backbuffer to the output context
void srEnd();

/// Internal: Creates the output frame buffer.
///
/// @param width The width of the frame buffer in pixels
/// @param height The height of the frame buffer in pixels
void _srFBCreate(uint* width, uint* height);

/// Internal: Destroys the output frame buffer and frees memory used by the pixels
void _srFBDestroy();

#endif /* SRFRAMEBUFFER_H */
