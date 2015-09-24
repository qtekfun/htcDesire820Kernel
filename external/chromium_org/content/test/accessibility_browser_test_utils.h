// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_ACCESSIBILITY_BROWSER_TEST_UTILS_H_
#define CONTENT_TEST_ACCESSIBILITY_BROWSER_TEST_UTILS_H_

#include "base/memory/weak_ptr.h"
#include "content/common/accessibility_node_data.h"
#include "content/common/view_message_enums.h"

namespace content {

class MessageLoopRunner;
class RenderViewHostImpl;
class Shell;

class AccessibilityNotificationWaiter {
 public:
  AccessibilityNotificationWaiter(
      Shell* shell,
      AccessibilityMode accessibility_mode,
      blink::WebAXEvent event);
  ~AccessibilityNotificationWaiter();

  
  
  
  void WaitForNotification();

  
  
  const AccessibilityNodeDataTreeNode& GetAccessibilityNodeDataTree() const;

 private:
  
  void OnAccessibilityEvent(blink::WebAXEvent event);

  
  
  
  bool IsAboutBlank();

  Shell* shell_;
  RenderViewHostImpl* view_host_;
  blink::WebAXEvent event_to_wait_for_;
  scoped_refptr<MessageLoopRunner> loop_runner_;
  base::WeakPtrFactory<AccessibilityNotificationWaiter> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(AccessibilityNotificationWaiter);
};

}  

#endif  
