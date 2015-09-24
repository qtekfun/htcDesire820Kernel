// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_AUTOFILL_GENERATED_CREDIT_CARD_BUBBLE_COCOA_H_
#define CHROME_BROWSER_UI_COCOA_AUTOFILL_GENERATED_CREDIT_CARD_BUBBLE_COCOA_H_

#include "base/mac/scoped_nsobject.h"
#include "chrome/browser/ui/autofill/generated_credit_card_bubble_view.h"

#ifdef __OBJC__
@class GeneratedCreditCardBubbleControllerCocoa;
#else
class GeneratedCreditCardBubbleControllerCocoa;
#endif

namespace autofill {

class GeneratedCreditCardBubbleCocoa : public GeneratedCreditCardBubbleView {
 public:
  virtual ~GeneratedCreditCardBubbleCocoa();

  
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual bool IsHiding() const OVERRIDE;

  
  void OnBubbleClosing();
  void OnLinkClicked();

  base::WeakPtr<GeneratedCreditCardBubbleController> controller() {
    return controller_;
  }

 private:
  explicit GeneratedCreditCardBubbleCocoa(
      const base::WeakPtr<GeneratedCreditCardBubbleController>& controller);
  friend base::WeakPtr<GeneratedCreditCardBubbleView>
      GeneratedCreditCardBubbleView::Create(
          const base::WeakPtr<GeneratedCreditCardBubbleController>& controller);

  
  GeneratedCreditCardBubbleControllerCocoa* bubbleController_;

  
  base::WeakPtr<GeneratedCreditCardBubbleController> controller_;

  
  base::WeakPtrFactory<GeneratedCreditCardBubbleCocoa> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(GeneratedCreditCardBubbleCocoa);
};

}  

#endif  

