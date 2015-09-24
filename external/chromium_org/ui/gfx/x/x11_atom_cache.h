// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_X_X11_ATOM_CACHE_H_
#define UI_GFX_X_X11_ATOM_CACHE_H_

#include "base/basictypes.h"
#include "ui/gfx/gfx_export.h"

#include <map>
#include <string>

#include <X11/Xlib.h>

#undef RootWindow

namespace ui {

class GFX_EXPORT X11AtomCache {
 public:
  
  X11AtomCache(Display* xdisplay, const char** to_cache);
  ~X11AtomCache();

  
  ::Atom GetAtom(const char*) const;

  
  
  void allow_uncached_atoms() { uncached_atoms_allowed_ = true; }

 private:
  Display* xdisplay_;

  bool uncached_atoms_allowed_;

  mutable std::map<std::string, ::Atom> cached_atoms_;

  DISALLOW_COPY_AND_ASSIGN(X11AtomCache);
};

}  

#endif  
