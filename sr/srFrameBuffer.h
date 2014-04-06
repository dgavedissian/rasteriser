// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef _SRFRAMEBUFFER_H_
#define _SRFRAMEBUFFER_H_

// Create frame buffer
void srCreateFrameBuffer(unsigned int width, unsigned int height);

// Destroy the frame buffer
void srDestroyFrameBuffer();

// Clear the frame buffer to a specified colour
void srClear(uint32_t colour);

// Draw a pixel
void srDrawPixel(unsigned int x, unsigned int y, uint32_t colour);

// Mark the end of a frame
void srPresent();

// Internal: Access pixels
uint32_t* _srGetPixels();

// Internal: Get frame buffer dimensions
unsigned int _srGetWidth();
unsigned int _srGetHeight();

#endif