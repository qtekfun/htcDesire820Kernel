// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_CAIRO_CACHED_SURFACE_H_
#define CHROME_BROWSER_UI_GTK_CAIRO_CACHED_SURFACE_H_
#pragma once

typedef struct _GdkPixbuf GdkPixbuf;
typedef struct _cairo cairo_t;
typedef struct _cairo_surface cairo_surface_t;

class CairoCachedSurface {
 public:
  CairoCachedSurface();
  ~CairoCachedSurface();

  
  bool valid() const {
    return pixbuf_;
  }

  
  int Width() const;
  int Height() const;

  
  
  
  
  void UsePixbuf(GdkPixbuf* pixbuf);

  
  
  void SetSource(cairo_t* cr, int x, int y);

  
  
  GdkPixbuf* pixbuf() { return pixbuf_; }

 private:
  
  GdkPixbuf* pixbuf_;

  
  
  cairo_surface_t* surface_;
};

#endif  
