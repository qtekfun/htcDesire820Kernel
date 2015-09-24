// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_GENERATED_CREDIT_CARD_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_GENERATED_CREDIT_CARD_VIEW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/ui/autofill/generated_credit_card_bubble_controller.h"
#include "chrome/browser/ui/views/location_bar/location_bar_decoration_view.h"
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"
#include "ui/views/controls/image_view.h"

namespace autofill {
class GeneratedCreditCardBubbleController;
}

class GeneratedCreditCardView : public LocationBarDecorationView {
 public:
  explicit GeneratedCreditCardView(LocationBarView::Delegate* delegate);
  virtual ~GeneratedCreditCardView();

  void Update();

 protected:
  
  virtual bool CanHandleClick() const OVERRIDE;
  virtual void OnClick() OVERRIDE;

 private:
  
  
  autofill::GeneratedCreditCardBubbleController* GetController() const;

  LocationBarView::Delegate* delegate_;  

  DISALLOW_COPY_AND_ASSIGN(GeneratedCreditCardView);
};

#endif  
