// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_MAC_EDITCOMMAND_HELPER_H_
#define CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_MAC_EDITCOMMAND_HELPER_H_

#import <Cocoa/Cocoa.h>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/gtest_prod_util.h"
#include "content/browser/renderer_host/render_widget_host_view_mac.h"

namespace content {

class CONTENT_EXPORT RenderWidgetHostViewMacEditCommandHelper {
   FRIEND_TEST_ALL_PREFIXES(RenderWidgetHostViewMacEditCommandHelperTest,
                            TestAddEditingSelectorsToClass);
   FRIEND_TEST_ALL_PREFIXES(RenderWidgetHostViewMacEditCommandHelperTest,
                            TestEditingCommandDelivery);

 public:
  RenderWidgetHostViewMacEditCommandHelper();
  ~RenderWidgetHostViewMacEditCommandHelper();

  
  
  
  
  
  
  void AddEditingSelectorsToClass(Class klass);

  
  
  
  
  bool IsMenuItemEnabled(SEL item_action,
                         id<RenderWidgetHostViewMacOwner> owner);

  
  
  static NSString* CommandNameForSelector(SEL selector);

 protected:
  
  
  
  NSArray* GetEditSelectorNames();

 private:
  base::hash_set<std::string> edit_command_set_;
  DISALLOW_COPY_AND_ASSIGN(RenderWidgetHostViewMacEditCommandHelper);
};

}  

#endif  
