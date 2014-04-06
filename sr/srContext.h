// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef _SRCONTEXT_H_
#define _SRCONTEXT_H_

// Internal: Create context
void _srCreateContext(unsigned int width, unsigned int height);

// Internal: Destroy context
void _srDestroyContext();

// Internal: Copy frame buffer to the screen - depending on context
void _srPresent();

#endif