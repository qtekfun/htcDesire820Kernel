// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_TEST_CHROME_TAB_MOCKS_H_
#define CHROME_FRAME_TEST_CHROME_TAB_MOCKS_H_

#include "chrome_frame/chrome_tab.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace testing {

class IChromeFramePrivilegedMockImpl : public IChromeFramePrivileged {
 public:
  
#include "mock_ichromeframeprivileged.gen"  
};

class MockIChromeFramePrivileged
  : public CComObjectRootEx<CComSingleThreadModel>,
    public testing::StrictMock<IChromeFramePrivilegedMockImpl> {
 public:
  DECLARE_NOT_AGGREGATABLE(MockIChromeFramePrivileged)
  BEGIN_COM_MAP(MockIChromeFramePrivileged)
    COM_INTERFACE_ENTRY(IChromeFramePrivileged)
  END_COM_MAP()
  DECLARE_PROTECT_FINAL_CONSTRUCT()

  HRESULT Initialize(MockIChromeFramePrivileged** cfp) {
    *cfp = this;
    return S_OK;
  }
};

}  

#endif  
