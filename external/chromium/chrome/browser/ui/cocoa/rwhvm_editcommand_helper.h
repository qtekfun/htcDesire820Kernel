// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_RWHVM_EDITCOMMAND_HELPER_H_
#define CHROME_BROWSER_UI_COCOA_RWHVM_EDITCOMMAND_HELPER_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/basictypes.h"
#include "base/hash_tables.h"
#include "base/gtest_prod_util.h"
#include "chrome/browser/renderer_host/render_widget_host_view_mac.h"

class RWHVMEditCommandHelper {
   FRIEND_TEST_ALL_PREFIXES(RWHVMEditCommandHelperTest,
                            TestAddEditingSelectorsToClass);
   FRIEND_TEST_ALL_PREFIXES(RWHVMEditCommandHelperTest,
                            TestEditingCommandDelivery);

 public:
  RWHVMEditCommandHelper();
  ~RWHVMEditCommandHelper();

  
  
  
  
  
  
  void AddEditingSelectorsToClass(Class klass);

  
  
  
  
  bool IsMenuItemEnabled(SEL item_action,
                         id<RenderWidgetHostViewMacOwner> owner);

  
  
  static NSString* CommandNameForSelector(SEL selector);

 protected:
  
  
  
  NSArray* GetEditSelectorNames();

 private:
  base::hash_set<std::string> edit_command_set_;
  DISALLOW_COPY_AND_ASSIGN(RWHVMEditCommandHelper);
};

#endif  
