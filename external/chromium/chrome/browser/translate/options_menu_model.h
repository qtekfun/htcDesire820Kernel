// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TRANSLATE_OPTIONS_MENU_MODEL_H_
#define CHROME_BROWSER_TRANSLATE_OPTIONS_MENU_MODEL_H_
#pragma once

#include "ui/base/models/simple_menu_model.h"

class TranslateInfoBarDelegate;

class OptionsMenuModel : public ui::SimpleMenuModel,
                         public ui::SimpleMenuModel::Delegate {
 public:
  explicit OptionsMenuModel(TranslateInfoBarDelegate* translate_delegate);
  virtual ~OptionsMenuModel();

  
  virtual bool IsCommandIdChecked(int command_id) const;
  virtual bool IsCommandIdEnabled(int command_id) const;
  virtual bool GetAcceleratorForCommandId(int command_id,
                                          ui::Accelerator* accelerator);
  virtual void ExecuteCommand(int command_id);

 private:
  TranslateInfoBarDelegate* translate_infobar_delegate_;

  DISALLOW_COPY_AND_ASSIGN(OptionsMenuModel);
};

#endif  