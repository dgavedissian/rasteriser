// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef _SRCONTEXT_H_
#define _SRCONTEXT_H_

/// Returns non-zero if the context is still active
int srContextActive();

/// Internal: Sets up the current context so that it is ready for rendering
///
/// @param width The width of this context, which may not be what was requested
/// @param height The height of this context, which may not be what was requested
void _srRequestContext(uint* width, uint* height);

/// Internal: Frees memory allocated by the current context and cleans up
/// anything created by the context.
void _srDestroyContext();

/// Internal: Write the contexts of the output frame buffer to the context
void _srCopyFramebuffer();

#endif
