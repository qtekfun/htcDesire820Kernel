// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_NEW_CREDIT_CARD_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_AUTOFILL_NEW_CREDIT_CARD_BUBBLE_VIEW_H_

#include "base/memory/weak_ptr.h"

namespace autofill {

class NewCreditCardBubbleController;

class NewCreditCardBubbleView {
 public:
  
  static const int kContentsWidth;

  virtual ~NewCreditCardBubbleView();

  
  virtual void Show() = 0;

  
  virtual void Hide() = 0;

  
  
  static base::WeakPtr<NewCreditCardBubbleView> Create(
      NewCreditCardBubbleController* controller);
};

}  

#endif  
