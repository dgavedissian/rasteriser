// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef SRFRAMEBUFFER_H
#define SRFRAMEBUFFER_H

/// Creates the output frame buffer.
///
/// @param width The width of the frame buffer in pixels
/// @param height The height of the frame buffer in pixels
void srCreateFrameBuffer(uint width, uint height);

/// Destroys the output frame buffer and frees memory used by the pixels
void srDestroyFrameBuffer();

/// Set the target frame rate
///
/// @param fps Target FPS count
void srSetMaxFPS(uint fps);

/// Sets the value of each pixel in the frame buffer to the specified colour.
///
/// @param colour The colour to clear the frame buffer to
void srClear(uint32_t colour);

/// Sets a pixel in the frame buffer to a certain colour.
/// Pre: x < _srGetWidth() && y < _srGetHeight()
///
/// @param x The horizontal position of the pixel
/// @param y The vertical position of the pixel
/// @param colour The colour to set the pixel to
void srDrawPixel(uint x, uint y, uint32_t colour);

/// Copies the contents of the output frame buffer to the current context.
void srPresent();

/// Internal: Access the internal array of pixels
///
/// @return Pointer to the first pixel
uint32_t* _srGetPixels();

/// Internal: Get frame buffer width
///
/// @return The current width of the frame buffer
uint _srGetWidth();

/// Internal: Get the frame buffer height
///
/// @return The current height of the frame buffer
uint _srGetHeight();

#endif /* SRFRAMEBUFFER_H */
