// Software Rasteriser
// Copyright (c) David Avedissian 2014
#ifndef _SR_3D_H_
#define _SR_3D_H_

void srInit3D();

void srBegin();
void srEnd();

void srVertex(float x, float y, float z);

void srDrawAll();
void srDrawTriangle(vec3_t *a, vec3_t *b, vec3_t *c);

#endif