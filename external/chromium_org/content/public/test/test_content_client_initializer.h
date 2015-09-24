// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_TEST_TEST_CONTENT_CLIENT_INITIALIZER_
#define CONTENT_PUBLIC_TEST_TEST_CONTENT_CLIENT_INITIALIZER_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace content {

class ContentClient;
class MockRenderProcessHostFactory;
class NotificationServiceImpl;
class TestContentBrowserClient;
class TestRenderViewHostFactory;

class TestContentClientInitializer {
 public:
  TestContentClientInitializer();
  ~TestContentClientInitializer();

  
  
  
  void CreateTestRenderViewHosts();

 private:
  scoped_ptr<NotificationServiceImpl> notification_service_;
  scoped_ptr<ContentClient> content_client_;
  scoped_ptr<TestContentBrowserClient> content_browser_client_;
  scoped_ptr<MockRenderProcessHostFactory> rph_factory_;
  scoped_ptr<TestRenderViewHostFactory> test_render_view_host_factory_;

  DISALLOW_COPY_AND_ASSIGN(TestContentClientInitializer);
};

}  

#endif  
