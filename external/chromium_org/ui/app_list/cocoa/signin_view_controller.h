// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_COCOA_SIGNIN_VIEW_CONTROLLER_H_
#define UI_APP_LIST_COCOA_SIGNIN_VIEW_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "ui/app_list/app_list_export.h"

namespace app_list {
class SigninDelegate;
}

APP_LIST_EXPORT
@interface SigninViewController : NSViewController {
 @private
  app_list::SigninDelegate* delegate_; 
}

- (id)initWithFrame:(NSRect)frame
       cornerRadius:(CGFloat)cornerRadius
           delegate:(app_list::SigninDelegate*)delegate;

@end

#endif  
