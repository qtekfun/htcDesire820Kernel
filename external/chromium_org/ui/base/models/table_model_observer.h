// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_MODELS_TABLE_MODEL_OBSERVER_H_
#define UI_BASE_MODELS_TABLE_MODEL_OBSERVER_H_

#include "ui/base/ui_export.h"

namespace ui {

class UI_EXPORT TableModelObserver {
 public:
  
  virtual void OnModelChanged() = 0;

  
  virtual void OnItemsChanged(int start, int length) = 0;

  
  virtual void OnItemsAdded(int start, int length) = 0;

  
  virtual void OnItemsRemoved(int start, int length) = 0;

 protected:
  virtual ~TableModelObserver() {}
};

}  

#endif  
