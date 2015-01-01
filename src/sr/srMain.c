// Software Rasteriser
// Copyright (c) David Avedissian 2014
#include "srCommon.h"
#include "srFrameBuffer.h"
#include "srMain.h"

void srInit(srInitParams* params)
{
  srCreateFrameBuffer(params->width, params->height);
}

void srShutdown()
{
  srDestroyFrameBuffer();
}
