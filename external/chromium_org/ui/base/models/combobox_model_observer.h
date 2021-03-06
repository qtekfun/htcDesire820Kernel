// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_MODELS_COMBOBOX_MODEL_OBSERVER_H_
#define UI_BASE_MODELS_COMBOBOX_MODEL_OBSERVER_H_

#include "ui/base/ui_export.h"

namespace ui {

class UI_EXPORT ComboboxModelObserver {
 public:
  
  
  virtual void OnModelChanged() = 0;

 protected:
  virtual ~ComboboxModelObserver() {}
};

}  

#endif  
