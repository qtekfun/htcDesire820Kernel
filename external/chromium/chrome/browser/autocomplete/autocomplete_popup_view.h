// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_POPUP_VIEW_H_
#define CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_POPUP_VIEW_H_
#pragma once

#include "build/build_config.h"

namespace gfx {
class Rect;
}

class AutocompletePopupView {
 public:
  virtual ~AutocompletePopupView() {}

  
  virtual bool IsOpen() const = 0;

  
  virtual void InvalidateLine(size_t line) = 0;

  
  
  virtual void UpdatePopupAppearance() = 0;

  
  
  
  virtual gfx::Rect GetTargetBounds() = 0;

  
  virtual void PaintUpdatesNow() = 0;

  
  
  
  
  virtual void OnDragCanceled() = 0;
};

#endif  
