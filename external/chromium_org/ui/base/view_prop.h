// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_VIEW_PROP_H_
#define UI_BASE_VIEW_PROP_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "ui/base/ui_export.h"
#include "ui/gfx/native_widget_types.h"

#if defined(OS_MACOSX) || (defined(OS_LINUX) && !defined(USE_AURA))
#error view_prop.h is only for windows and aura builds.
#endif

namespace ui {

class UI_EXPORT ViewProp {
 public:
  
  
  
  
  
  ViewProp(gfx::AcceleratedWidget view, const char* key, void* data);
  ~ViewProp();

  
  
  static void* GetValue(gfx::AcceleratedWidget view, const char* key);

  
  const char* Key() const;

 private:
  class Data;

  
  scoped_refptr<Data> data_;

  DISALLOW_COPY_AND_ASSIGN(ViewProp);
};

}  

#endif  
