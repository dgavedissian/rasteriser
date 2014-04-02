// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef _SR_CONTEXT_H_
#define _SR_CONTEXT_H_

// Create context
void createContext(int width, int height);

// Drawing
void clear(int colour);
void point(int x, int y, int colour);

// Copy contents of buffer to screen 
void present();

#endif