// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_NOTIFICATION_CONTAINER_H_
#define CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_NOTIFICATION_CONTAINER_H_

#import <Cocoa/Cocoa.h>

#include <vector>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/autofill/autofill_layout.h"

@class AutofillArrowView;

namespace autofill {
  class DialogNotification;
  typedef std::vector<DialogNotification> DialogNotifications;
  class AutofillDialogViewDelegate;
}

@interface AutofillNotificationContainer : NSViewController<AutofillLayout> {
 @private
  
  base::scoped_nsobject<NSMutableArray> notificationControllers_;

  
  NSView* anchorView_;

  
  autofill::AutofillDialogViewDelegate* delegate_;
}

- (id)initWithDelegate:(autofill::AutofillDialogViewDelegate*)delegate;

- (NSSize)preferredSizeForWidth:(CGFloat)width;

- (void)setNotifications:(const autofill::DialogNotifications&) notifications;

- (void)setAnchorView:(NSView*)anchorView;

@end

#endif 
