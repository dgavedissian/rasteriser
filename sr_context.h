// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef _SR_CONTEXT_H_
#define _SR_CONTEXT_H_

// Create context
void srCreateContext(int width, int height);

// Drawing
void srClear(int colour);
void srPoint(int x, int y, int colour);

// Copy contents of buffer to screen 
void srPresent();

#endif