// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_CONFIRM_BUBBLE_MODEL_H_
#define CHROME_BROWSER_UI_CONFIRM_BUBBLE_MODEL_H_

#include "base/basictypes.h"
#include "base/strings/string16.h"

namespace gfx {
class Image;
}

class ConfirmBubbleModel {
 public:
  enum BubbleButton {
    BUTTON_NONE   = 0,
    BUTTON_OK     = 1 << 0,
    BUTTON_CANCEL = 1 << 1,
  };

  ConfirmBubbleModel();
  virtual ~ConfirmBubbleModel();

  
  
  virtual base::string16 GetTitle() const = 0;
  virtual base::string16 GetMessageText() const = 0;

  
  
  
  virtual gfx::Image* GetIcon() const = 0;

  
  
  
  
  virtual int GetButtons() const;

  
  
  virtual base::string16 GetButtonLabel(BubbleButton button) const;

  
  virtual void Accept();

  
  virtual void Cancel();

  
  
  virtual base::string16 GetLinkText() const;

  
  virtual void LinkClicked();

 private:
  DISALLOW_COPY_AND_ASSIGN(ConfirmBubbleModel);
};

#endif  
