// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_AUTOFILL_NEW_CREDIT_CARD_BUBBLE_COCOA_H_
#define CHROME_BROWSER_UI_COCOA_AUTOFILL_NEW_CREDIT_CARD_BUBBLE_COCOA_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/ui/autofill/new_credit_card_bubble_view.h"

#ifdef __OBJC__
@class InfoBubbleWindow;
@class NewCreditCardBubbleControllerCocoa;
@class NSWindow;
#else
class InfoBubbleWindow;
class NewCreditCardBubbleControllerCocoa;
class NSWindow;
}
#endif

namespace autofill {

class NewCreditCardBubbleController;

class NewCreditCardBubbleCocoa : public NewCreditCardBubbleView {
 public:
  virtual ~NewCreditCardBubbleCocoa();

  
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(NewCreditCardBubbleCocoaUnitTest, CloseDeletes);
  friend base::WeakPtr<NewCreditCardBubbleView> NewCreditCardBubbleView::Create(
      NewCreditCardBubbleController* controller);

  explicit NewCreditCardBubbleCocoa(NewCreditCardBubbleController*);

  
  void CreateCocoaController(NSWindow* parent);

  InfoBubbleWindow* GetInfoBubbleWindow();

  
  NewCreditCardBubbleControllerCocoa* bubbleController_;

  
  NewCreditCardBubbleController* controller_;

  base::WeakPtrFactory<NewCreditCardBubbleCocoa> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(NewCreditCardBubbleCocoa);
};

}  

#endif  
