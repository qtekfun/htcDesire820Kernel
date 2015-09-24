// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "base/bind.h"
#include "base/run_loop.h"
#include "gpu/GLES2/gl2extchromium.h"
#include "gpu/command_buffer/client/context_support.h"

namespace {

class SignalTest : public ContextTestBase {
 public:
  static void RunOnlyOnce(base::Closure cb, int* tmp) {
    CHECK_EQ(*tmp, 0);
    ++*tmp;
    cb.Run();
  }

  
  void TestSignalSyncPoint(unsigned sync_point) {
    base::RunLoop run_loop;
    context_support_->SignalSyncPoint(sync_point, run_loop.QuitClosure());
    run_loop.Run();
  }

  
  void TestSignalQuery(blink::WebGLId query) {
    base::RunLoop run_loop;
    context_support_->SignalQuery(
        query,
        base::Bind(
            &RunOnlyOnce, run_loop.QuitClosure(), base::Owned(new int(0))));
    run_loop.Run();
  }
};

CONTEXT_TEST_F(SignalTest, BasicSignalSyncPointTest) {
  if (!context_)
    return;

  TestSignalSyncPoint(context_->insertSyncPoint());
};

CONTEXT_TEST_F(SignalTest, InvalidSignalSyncPointTest) {
  if (!context_)
    return;

  
  
  TestSignalSyncPoint(1297824234);
};

CONTEXT_TEST_F(SignalTest, BasicSignalQueryTest) {
  if (!context_)
    return;

  unsigned query = context_->createQueryEXT();
  context_->beginQueryEXT(GL_COMMANDS_ISSUED_CHROMIUM, query);
  context_->finish();
  context_->endQueryEXT(GL_COMMANDS_ISSUED_CHROMIUM);
  TestSignalQuery(query);
  context_->deleteQueryEXT(query);
};

CONTEXT_TEST_F(SignalTest, SignalQueryUnboundTest) {
  if (!context_)
    return;

  blink::WebGLId query = context_->createQueryEXT();
  TestSignalQuery(query);
  context_->deleteQueryEXT(query);
};

CONTEXT_TEST_F(SignalTest, InvalidSignalQueryUnboundTest) {
  if (!context_)
    return;

  
  
  TestSignalQuery(928729087);
  TestSignalQuery(928729086);
  TestSignalQuery(928729085);
  TestSignalQuery(928729083);
  TestSignalQuery(928729082);
  TestSignalQuery(928729081);
};

};
