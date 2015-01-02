// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef SRCONTEXTTERM_H
#define SRCONTEXTTERM_H

// Terminal implementation of the context
void _srTermCtxRequest(uint* width, uint* height);
void _srTermCtxDestroy();
int _srTermCtxActive();
void _srTermCtxClear(uint32_t colour);
void _srTermCtxBegin();
void _srTermCtxPutPixel(uint x, uint y, uint32_t colour);
void _srTermCtxEnd();

#endif /* SRCONTEXTTERM_H */
