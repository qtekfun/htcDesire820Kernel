// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_INFOBARS_CONFIRM_INFOBAR_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_INFOBARS_CONFIRM_INFOBAR_CONTROLLER_H_

#import "chrome/browser/ui/cocoa/infobars/infobar_controller.h"

@interface ConfirmInfoBarController : InfoBarController
- (IBAction)ok:(id)sender;
- (IBAction)cancel:(id)sender;
- (void)linkClicked;
@end

#endif  
