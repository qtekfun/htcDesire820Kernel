// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_BUGGY_BHO_HANDLING_H_
#define CHROME_FRAME_BUGGY_BHO_HANDLING_H_

#include <atlbase.h>
#include <atlcom.h>
#include <exdisp.h>

#include <vector>

#include "base/threading/thread_local.h"
#include "base/win/scoped_comptr.h"

namespace buggy_bho {

typedef HRESULT (__stdcall* InvokeFunc)(IDispatch* me, DISPID dispid,
                                        REFIID riid, LCID lcid, WORD flags,
                                        DISPPARAMS* params, VARIANT* result,
                                        EXCEPINFO* ei, UINT* err);

class BuggyBhoTls {
 public:
  
  
  
  
  
  HRESULT PatchBuggyBHOs(IWebBrowser2* browser);

  
  static BuggyBhoTls* GetInstance();

  
  static void DestroyInstance();

  void set_web_browser(IWebBrowser2* web_browser2) {
    web_browser2_ = web_browser2;
  }

 protected:
  BuggyBhoTls();
  ~BuggyBhoTls();
  

  
  void AddBuggyObject(IDispatch* obj);

  
  
  
  
  
  bool ShouldSkipInvoke(IDispatch* obj) const;

  

  
  bool PatchIfBuggy(IUnknown* unk, const IID& diid);

  
  static HRESULT PatchInvokeMethod(PROC* invoke);

  
  
  
  static STDMETHODIMP BuggyBhoInvoke(InvokeFunc original, IDispatch* me,
      DISPID dispid, REFIID riid, LCID lcid, WORD flags, DISPPARAMS* params,
      VARIANT* result, EXCEPINFO* ei, UINT* err);

 protected:
  
  std::vector<IDispatch*> bad_objects_;
  
  static base::ThreadLocalPointer<BuggyBhoTls> s_bad_object_tls_;
  
  base::win::ScopedComPtr<IWebBrowser2> web_browser2_;
  
  bool patched_;
};

}  

#endif  
