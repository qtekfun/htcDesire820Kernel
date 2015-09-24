// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_SCOPED_GOBJECT_H_
#define UI_GFX_SCOPED_GOBJECT_H_

#include <glib-object.h>

#include "base/memory/scoped_ptr.h"

namespace ui {

template<class T>
struct ScopedGObject {
  
  
  template<class U>
  struct GObjectUnrefer {
    void operator()(U* ptr) const {
      if (ptr)
        g_object_unref(ptr);
    }
  };

  typedef scoped_ptr_malloc<T, GObjectUnrefer<T> > Type;
};

}  

#endif  
