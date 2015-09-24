// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_IMAGE_CAIRO_CACHED_SURFACE_H_
#define UI_GFX_IMAGE_CAIRO_CACHED_SURFACE_H_

#include <vector>

#include "ui/gfx/gfx_export.h"

typedef struct _GdkDisplay GdkDisplay;
typedef struct _GdkPixbuf GdkPixbuf;
typedef struct _GtkWidget GtkWidget;
typedef struct _cairo cairo_t;
typedef struct _cairo_surface cairo_surface_t;

namespace gfx {

class GFX_EXPORT CairoCachedSurface {
 public:
  CairoCachedSurface();
  ~CairoCachedSurface();

  
  bool valid() const {
    return pixbuf_;
  }

  
  void Reset();

  
  int Width() const;
  int Height() const;

  
  
  
  
  void UsePixbuf(GdkPixbuf* pixbuf);

  
  
  void SetSource(cairo_t* cr, GtkWidget* widget, int x, int y) const;
  void SetSource(cairo_t* cr, GdkDisplay* display, int x, int y) const;

  
  void MaskSource(cairo_t* cr, GtkWidget* widget, int x, int y) const;
  void MaskSource(cairo_t* cr, GdkDisplay* display, int x, int y) const;

  
  
  GdkPixbuf* pixbuf() { return pixbuf_; }

 private:
  typedef std::vector<std::pair<GdkDisplay*, cairo_surface_t*> > SurfaceVector;

  
  
  cairo_surface_t* GetSurfaceFor(cairo_t* cr, GdkDisplay* display) const;

  
  GdkPixbuf* pixbuf_;

  
  
  
  mutable SurfaceVector surface_map_;
};

}  

#endif  
