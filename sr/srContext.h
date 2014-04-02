// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef _SRCONTEXT_H_
#define _SRCONTEXT_H_

// Create context
void srCreateContext(int width, int height);

// Drawing
void srClear(int colour);
void srPoint(int x, int y, int colour);

// Copy contents of buffer to screen 
void srPresent();

#endif