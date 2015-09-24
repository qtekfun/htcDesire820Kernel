// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_TEST_TEST_VIEW_H_
#define PPAPI_TEST_TEST_VIEW_H_

#include "ppapi/cpp/view.h"
#include "ppapi/tests/test_case.h"

class TestView : public TestCase {
 public:
  TestView(TestingInstance* instance);

  virtual void DidChangeView(const pp::View& view);

  
  virtual bool Init();
  virtual void RunTests(const std::string& test_filter);

 private:
  
  
  
  
  
  
  
  bool WaitUntilViewChanged();

  void QuitMessageLoop(int32_t result);

  std::string TestCreatedVisible();
  std::string TestCreatedInvisible();
  std::string TestPageHideShow();
  std::string TestSizeChange();
  std::string TestClipChange();

  pp::View last_view_;

  
  
  std::vector<bool> page_visibility_log_;

  
  
  
  bool post_quit_on_view_changed_;
};

#endif  
