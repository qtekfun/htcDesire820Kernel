// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_IO_SURFACE_SUPPORT_MAC_H_
#define UI_GL_IO_SURFACE_SUPPORT_MAC_H_

#include <CoreFoundation/CoreFoundation.h>
#include <CoreVideo/CoreVideo.h>
#include <mach/mach.h>
#include <OpenGL/OpenGL.h>

#include "base/basictypes.h"
#include "ui/gl/gl_export.h"



class GL_EXPORT IOSurfaceSupport {
 public:
  
  
  
  static IOSurfaceSupport* Initialize();

  virtual CFStringRef GetKIOSurfaceWidth() = 0;
  virtual CFStringRef GetKIOSurfaceHeight() = 0;
  virtual CFStringRef GetKIOSurfaceBytesPerElement() = 0;
  virtual CFStringRef GetKIOSurfacePixelFormat() = 0;
  virtual CFStringRef GetKIOSurfaceIsGlobal() = 0;

  virtual CFTypeRef IOSurfaceCreate(CFDictionaryRef properties) = 0;

  
  
  virtual uint32 IOSurfaceGetID(CFTypeRef io_surface) = 0;
  virtual CFTypeRef IOSurfaceLookup(uint32 io_surface_id) = 0;

  
  
  
  
  virtual mach_port_t IOSurfaceCreateMachPort(CFTypeRef io_surface) = 0;
  virtual CFTypeRef IOSurfaceLookupFromMachPort(mach_port_t port) = 0;

  virtual size_t IOSurfaceGetWidth(CFTypeRef io_surface) = 0;
  virtual size_t IOSurfaceGetHeight(CFTypeRef io_surface) = 0;
  virtual size_t IOSurfaceGetBytesPerRow(CFTypeRef io_surface) = 0;
  virtual void* IOSurfaceGetBaseAddress(CFTypeRef io_surface) = 0;

  virtual IOReturn IOSurfaceLock(CFTypeRef io_surface,
                                 uint32 options,
                                 uint32* seed) = 0;
  virtual IOReturn IOSurfaceUnlock(CFTypeRef io_surface,
                                   uint32 options,
                                   uint32* seed) = 0;

  virtual CGLError CGLTexImageIOSurface2D(CGLContextObj ctx,
                                          GLenum target,
                                          GLenum internal_format,
                                          GLsizei width,
                                          GLsizei height,
                                          GLenum format,
                                          GLenum type,
                                          CFTypeRef io_surface,
                                          GLuint plane) = 0;

  virtual CFTypeRef CVPixelBufferGetIOSurface(
      CVPixelBufferRef pixel_buffer) = 0;

 protected:
  IOSurfaceSupport();
  virtual ~IOSurfaceSupport();

  DISALLOW_COPY_AND_ASSIGN(IOSurfaceSupport);
};

#endif  
