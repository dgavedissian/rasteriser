// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef SRCONTEXT_H
#define SRCONTEXT_H

/// Internal: Sets up the current context so that it is ready for rendering
///
/// @param width The width of this context, which may not be what was requested
/// @param height The height of this context, which may not be what was requested
void _srCtxRequest(uint* width, uint* height);

/// Internal: Frees memory allocated by the current context and cleans up
/// anything created by the context.
void _srCtxDestroy();

/// Internal: Returns non-zero if the context is still active
int _srCtxActive();

/// Internal: Clear the frame buffer
void _srCtxClear(uint32_t colour);

/// Internal: Begin a frame
void _srCtxBegin();

/// Internal: Write a pixel
void _srCtxPutPixel(uint x, uint y, uint32_t colour);

/// Internal: End a frame
void _srCtxEnd();

#endif /* SRCONTEXT_H */
