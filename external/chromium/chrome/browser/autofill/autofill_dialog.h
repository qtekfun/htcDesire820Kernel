// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_AUTOFILL_DIALOG_H_
#define CHROME_BROWSER_AUTOFILL_AUTOFILL_DIALOG_H_
#pragma once

#include <vector>

#include "chrome/browser/autofill/autofill_profile.h"
#include "chrome/browser/autofill/credit_card.h"

#ifdef ANDROID
namespace gfx {
typedef void* NativeView;
}
#else
#include "ui/gfx/native_widget_types.h"
#endif

class Profile;

class AutoFillDialogObserver {
 public:
  
  
  
  
  virtual void OnAutoFillDialogApply(
      std::vector<AutoFillProfile>* profiles,
      std::vector<CreditCard>* credit_cards) = 0;

 protected:
  virtual ~AutoFillDialogObserver() {}
};

void ShowAutoFillDialog(gfx::NativeView parent,
                        AutoFillDialogObserver* observer,
                        Profile* profile);

#endif  
