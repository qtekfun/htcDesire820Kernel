// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_BROWSER_PLUGIN_TEST_BROWSER_PLUGIN_GUEST_MANAGER_H_
#define CONTENT_BROWSER_BROWSER_PLUGIN_TEST_BROWSER_PLUGIN_GUEST_MANAGER_H_

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "content/browser/browser_plugin/browser_plugin_guest_manager.h"
#include "content/public/test/test_utils.h"

FORWARD_DECLARE_TEST(BrowserPluginHostTest, ReloadEmbedder);

namespace content {

class WebContentsImpl;

class TestBrowserPluginGuestManager : public BrowserPluginGuestManager {
 public:
  typedef BrowserPluginGuestManager::GuestInstanceMap GuestInstanceMap;

  TestBrowserPluginGuestManager();
  virtual ~TestBrowserPluginGuestManager();

  const GuestInstanceMap& guest_web_contents_for_testing() const {
    return guest_web_contents_by_instance_id_;
  }

  
  void WaitForGuestAdded();

 private:
  
  FRIEND_TEST_ALL_PREFIXES(BrowserPluginHostTest, ReloadEmbedder);

  
  virtual void AddGuest(int instance_id,
                        WebContentsImpl* guest_web_contents) OVERRIDE;

  scoped_refptr<MessageLoopRunner> message_loop_runner_;
  DISALLOW_COPY_AND_ASSIGN(TestBrowserPluginGuestManager);
};

} 

#endif  
