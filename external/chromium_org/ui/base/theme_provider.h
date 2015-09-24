// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_THEME_PROVIDER_H_
#define UI_BASE_THEME_PROVIDER_H_

#include "base/basictypes.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/base/layout.h"
#include "ui/base/ui_export.h"

#if defined(OS_MACOSX)
#ifdef __OBJC__
@class NSColor;
@class NSGradient;
@class NSImage;
#else
class NSColor;
class NSGradient;
class NSImage;
#endif  
#elif !defined(OS_WIN)
typedef struct _GdkColor GdkColor;
typedef struct _GdkPixbuf GdkPixbuf;
#endif  

class SkBitmap;

namespace base {
class RefCountedMemory;
}

namespace gfx {
class ImageSkia;
}

namespace ui {


class UI_EXPORT ThemeProvider {
 public:
  virtual ~ThemeProvider();

  
  
  virtual gfx::ImageSkia* GetImageSkiaNamed(int id) const = 0;

  
  virtual SkColor GetColor(int id) const = 0;

  
  
  virtual int GetDisplayProperty(int id) const = 0;

  
  
  virtual bool ShouldUseNativeFrame() const = 0;

  
  
  virtual bool HasCustomImage(int id) const = 0;

  
  
  
  virtual base::RefCountedMemory* GetRawData(
      int id,
      ui::ScaleFactor scale_factor) const = 0;

#if defined(OS_MACOSX) && !defined(TOOLKIT_VIEWS)
  
  virtual NSImage* GetNSImageNamed(int id) const = 0;

  
  
  virtual NSColor* GetNSImageColorNamed(int id) const = 0;

  
  virtual NSColor* GetNSColor(int id) const = 0;

  
  virtual NSColor* GetNSColorTint(int id) const = 0;

  
  virtual NSGradient* GetNSGradient(int id) const = 0;
#elif defined(OS_POSIX) && !defined(TOOLKIT_VIEWS) && !defined(OS_ANDROID)
  
  
  
  
  
  
  
  

  
  virtual GdkPixbuf* GetRTLEnabledPixbufNamed(int id) const = 0;
#endif
};

}  

#endif  
