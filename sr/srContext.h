// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef _SRCONTEXT_H_
#define _SRCONTEXT_H_

/// Internal: Sets up the current context so that it is ready for rendering
///
/// @param width The width of this context
/// @param height The height of this context
void _srCreateContext(unsigned int width, unsigned int height);

/// Internal: Frees memory allocated by the current context and cleans up
/// anything created by the context.
void _srDestroyContext();

/// Internal: Write the contexts of the output frame buffer to the context
void _srWriteFBToContext();

#endif