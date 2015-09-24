// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_LOCATION_BAR_GENERATED_CREDIT_CARD_DECORATION_H_
#define CHROME_BROWSER_UI_COCOA_LOCATION_BAR_GENERATED_CREDIT_CARD_DECORATION_H_

#import <Cocoa/Cocoa.h>

#include "base/basictypes.h"
#include "chrome/browser/ui/cocoa/location_bar/image_decoration.h"

class LocationBarViewMac;

namespace autofill {
class GeneratedCreditCardBubbleController;
}

class GeneratedCreditCardDecoration : public ImageDecoration {
 public:
  explicit GeneratedCreditCardDecoration(LocationBarViewMac* owner);
  virtual ~GeneratedCreditCardDecoration();

  
  void Update();

  
  
  NSPoint GetBubblePointInFrame(NSRect frame);

  
  
  
  virtual bool AcceptsMousePress() OVERRIDE;
  virtual bool OnMousePressed(NSRect frame) OVERRIDE;

 private:
  
  
  autofill::GeneratedCreditCardBubbleController* GetController() const;

  
  LocationBarViewMac* owner_;

  DISALLOW_COPY_AND_ASSIGN(GeneratedCreditCardDecoration);
};

#endif  

