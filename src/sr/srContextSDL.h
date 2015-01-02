// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef SRCONTEXTSDL_H
#define SRCONTEXTSDL_H

// SDL Implementation of the context
void _srSDLCtxRequest(uint* width, uint* height);
void _srSDLCtxDestroy();
int _srSDLCtxActive();
void _srSDLCtxClear(uint32_t colour);
void _srSDLCtxBegin();
void _srSDLCtxPutPixel(uint x, uint y, uint32_t colour);
void _srSDLCtxEnd();

#endif /* SRCONTEXTSDL_H */
