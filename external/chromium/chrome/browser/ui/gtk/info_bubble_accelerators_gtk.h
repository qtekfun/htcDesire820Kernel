// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_INFO_BUBBLE_ACCELERATORS_GTK_H_
#define CHROME_BROWSER_UI_GTK_INFO_BUBBLE_ACCELERATORS_GTK_H_
#pragma once

#include <gdk/gdktypes.h>
#include <glib.h>
#include <vector>

#include "base/basictypes.h"

struct InfoBubbleAcceleratorGtk {
  guint keyval;
  GdkModifierType modifier_type;
};

typedef std::vector<struct InfoBubbleAcceleratorGtk>
    InfoBubbleAcceleratorGtkList;

class InfoBubbleAcceleratorsGtk {
 public:
  static InfoBubbleAcceleratorGtkList GetList();

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(InfoBubbleAcceleratorsGtk);
};

#endif  
