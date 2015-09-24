// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_GLES2_GLES2_H_
#define MOJO_PUBLIC_GLES2_GLES2_H_


#include <stdint.h>

#if defined(WIN32)

#if defined(MOJO_GLES2_IMPLEMENTATION)
#define MOJO_GLES2_EXPORT __declspec(dllexport)
#else
#define MOJO_GLES2_EXPORT __declspec(dllimport)
#endif

#else  

#if defined(MOJO_GLES2_IMPLEMENTATION)
#define MOJO_GLES2_EXPORT __attribute__((visibility("default")))
#else
#define MOJO_GLES2_EXPORT
#endif

#endif  

#ifdef __cplusplus
extern "C" {
#endif

MOJO_GLES2_EXPORT void MojoGLES2Initialize();
MOJO_GLES2_EXPORT void MojoGLES2Terminate();
MOJO_GLES2_EXPORT void MojoGLES2MakeCurrent(uint64_t encoded);
MOJO_GLES2_EXPORT void MojoGLES2SwapBuffers();

#ifdef __cplusplus
}  
#endif

#endif  
