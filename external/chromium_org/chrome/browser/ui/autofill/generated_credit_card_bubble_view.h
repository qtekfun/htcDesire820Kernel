// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_GENERATED_CREDIT_CARD_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_AUTOFILL_GENERATED_CREDIT_CARD_BUBBLE_VIEW_H_

#include "base/memory/weak_ptr.h"

namespace autofill {

class GeneratedCreditCardBubbleController;

class GeneratedCreditCardBubbleView {
 public:
  
  static const int kContentsWidth;

  virtual ~GeneratedCreditCardBubbleView();

  
  virtual void Show() = 0;

  
  virtual void Hide() = 0;

  
  virtual bool IsHiding() const = 0;

  
  
  static base::WeakPtr<GeneratedCreditCardBubbleView> Create(
      const base::WeakPtr<GeneratedCreditCardBubbleController>& controller);
};

}  

#endif  
