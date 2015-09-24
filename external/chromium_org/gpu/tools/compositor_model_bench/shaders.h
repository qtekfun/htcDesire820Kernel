// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef GPU_TOOLS_COMPOSITOR_MODEL_BENCH_SHADERS_H_
#define GPU_TOOLS_COMPOSITOR_MODEL_BENCH_SHADERS_H_

#include <string>

class CCNode;
class ContentLayerNode;

typedef unsigned int GLuint;

enum ShaderID {
  SHADER_UNRECOGNIZED = 0,
  VERTEX_SHADER_POS_TEX_YUV_STRETCH,
  VERTEX_SHADER_POS_TEX,
  VERTEX_SHADER_POS_TEX_TRANSFORM,
  FRAGMENT_SHADER_YUV_VIDEO,
  FRAGMENT_SHADER_RGBA_TEX_FLIP_ALPHA,
  FRAGMENT_SHADER_RGBA_TEX_ALPHA,
  SHADER_ID_MAX
};

ShaderID ShaderIDFromString(std::string name);
std::string ShaderNameFromID(ShaderID id);

void ConfigAndActivateShaderForNode(CCNode* n);

void ConfigAndActivateShaderForTiling(ContentLayerNode* n);

void InitBuffers();

void BeginFrame();

void DrawQuad(float width, float height);

void DrawTileQuad(GLuint texID, int x, int y);

#endif  
