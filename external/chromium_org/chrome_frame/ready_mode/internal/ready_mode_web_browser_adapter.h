// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_READY_MODE_INTERNAL_READY_MODE_WEB_BROWSER_ADAPTER_H_
#define CHROME_FRAME_READY_MODE_INTERNAL_READY_MODE_WEB_BROWSER_ADAPTER_H_

#include <atlbase.h>
#include <atlcom.h>
#include <exdisp.h>
#include <exdispid.h>

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/win/scoped_comptr.h"

class ATL_NO_VTABLE ReadyModeWebBrowserAdapter
    : public CComObjectRootEx<CComSingleThreadModel>,
      public IDispEventSimpleImpl<0, ReadyModeWebBrowserAdapter,
                                  &DIID_DWebBrowserEvents2> {
 public:
  
  
  class Observer {
   public:
    virtual ~Observer() {}

    
    virtual void OnNavigateTo(const std::wstring& url) = 0;

    
    
    virtual void OnRenderInChromeFrame(const std::wstring& url) = 0;

    
    
    virtual void OnRenderInHost(const std::wstring& url) = 0;
  };  

  ReadyModeWebBrowserAdapter();

  
  
  
  
  bool Initialize(IWebBrowser2* web_browser_, Observer* observer);

  
  void Uninitialize();

DECLARE_NOT_AGGREGATABLE(ReadyModeWebBrowserAdapter)

BEGIN_COM_MAP(ReadyModeWebBrowserAdapter)
END_COM_MAP()

BEGIN_SINK_MAP(ReadyModeWebBrowserAdapter)
  SINK_ENTRY_INFO(0, DIID_DWebBrowserEvents2, DISPID_BEFORENAVIGATE2,
                  BeforeNavigate2, &kBeforeNavigate2Info)
  SINK_ENTRY_INFO(0, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE,
                  DocumentComplete, &kDocumentCompleteInfo)
  SINK_ENTRY_INFO(0, DIID_DWebBrowserEvents2, DISPID_ONQUIT,
                  OnQuit, &kOnQuitInfo)
END_SINK_MAP()

 private:
  
  STDMETHOD(BeforeNavigate2)(IDispatch* dispatch, VARIANT* url, VARIANT* flags,
      VARIANT* target_frame_name, VARIANT* post_data, VARIANT* headers,
      VARIANT_BOOL* cancel);
  STDMETHOD_(void, DocumentComplete)(IDispatch* dispatch, VARIANT* url);
  STDMETHOD_(void, OnQuit)();

  scoped_ptr<Observer> observer_;
  base::win::ScopedComPtr<IWebBrowser2> web_browser_;

  static _ATL_FUNC_INFO kBeforeNavigate2Info;
  static _ATL_FUNC_INFO kDocumentCompleteInfo;
  static _ATL_FUNC_INFO kOnQuitInfo;

  DISALLOW_COPY_AND_ASSIGN(ReadyModeWebBrowserAdapter);
};  

#endif  
