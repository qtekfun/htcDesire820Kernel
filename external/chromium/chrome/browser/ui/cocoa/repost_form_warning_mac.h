// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_REPOST_FORM_WARNING_MAC_H_
#define CHROME_BROWSER_UI_COCOA_REPOST_FORM_WARNING_MAC_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/cocoa/constrained_window_mac.h"

class RepostFormWarningController;

class RepostFormWarningMac : public ConstrainedDialogDelegate {
 public:
  
  
  static RepostFormWarningMac* Create(NSWindow* parent,
                                     TabContents* tab_contents);

  RepostFormWarningMac(NSWindow* parent,
                       RepostFormWarningController* controller);

  
  virtual void DeleteDelegate();

 private:
  virtual ~RepostFormWarningMac();

  scoped_ptr<RepostFormWarningController> controller_;

  DISALLOW_COPY_AND_ASSIGN(RepostFormWarningMac);
};

#endif  
