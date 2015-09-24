// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_MODELS_COMBOBOX_MODEL_H_
#define UI_BASE_MODELS_COMBOBOX_MODEL_H_

#include "base/strings/string16.h"
#include "ui/base/ui_export.h"

namespace ui {

class ComboboxModelObserver;

class UI_EXPORT ComboboxModel {
 public:
  
  virtual int GetItemCount() const = 0;

  
  virtual base::string16 GetItemAt(int index) = 0;

  
  
  virtual bool IsItemSeparatorAt(int index);

  
  
  virtual int GetDefaultIndex() const;

  
  virtual bool IsItemEnabledAt(int index) const;

  
  virtual void AddObserver(ComboboxModelObserver* observer) {}
  virtual void RemoveObserver(ComboboxModelObserver* observer) {}

 protected:
  virtual ~ComboboxModel() {}
};

}  

#endif  
