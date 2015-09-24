// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_TEST_GENERATED_CREDIT_CARD_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_AUTOFILL_TEST_GENERATED_CREDIT_CARD_BUBBLE_VIEW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/ui/autofill/generated_credit_card_bubble_view.h"

namespace autofill {

class TestGeneratedCreditCardBubbleView : public GeneratedCreditCardBubbleView {
 public:
  
  static base::WeakPtr<TestGeneratedCreditCardBubbleView> Create(
      const base::WeakPtr<GeneratedCreditCardBubbleController>& controller);

  virtual ~TestGeneratedCreditCardBubbleView();

  
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual bool IsHiding() const OVERRIDE;

  base::WeakPtr<TestGeneratedCreditCardBubbleView> GetWeakPtr();

  bool showing() const { return showing_; }

 private:
  explicit TestGeneratedCreditCardBubbleView(
      const base::WeakPtr<GeneratedCreditCardBubbleController>& controller);

  
  base::WeakPtr<GeneratedCreditCardBubbleController> controller_;

  
  bool showing_;

  base::WeakPtrFactory<TestGeneratedCreditCardBubbleView> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(TestGeneratedCreditCardBubbleView);
};

}  

#endif  
