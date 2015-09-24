// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_MODELS_DIALOG_MODEL_H_
#define UI_BASE_MODELS_DIALOG_MODEL_H_

#include "base/strings/string16.h"
#include "ui/base/ui_base_types.h"
#include "ui/base/ui_export.h"

namespace ui {

class UI_EXPORT DialogModel {
 public:
  virtual ~DialogModel();

  
  virtual base::string16 GetDialogLabel() const = 0;

  
  virtual base::string16 GetDialogTitle() const = 0;

  
  
  virtual int GetDialogButtons() const = 0;

  
  
  
  
  
  
  virtual int GetDefaultDialogButton() const = 0;

  
  
  virtual bool ShouldDefaultButtonBeBlue() const = 0;

  
  virtual base::string16 GetDialogButtonLabel(DialogButton button) const = 0;

  
  virtual bool IsDialogButtonEnabled(DialogButton button) const = 0;
};

}  

#endif  
