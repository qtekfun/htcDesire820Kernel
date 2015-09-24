// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_TEST_IE_EVENT_SINK_H_
#define CHROME_FRAME_TEST_IE_EVENT_SINK_H_

#include <atlbase.h>
#include <atlwin.h>
#include <exdispid.h>
#include <string>

#include "base/win/scoped_comptr.h"
#include "chrome_frame/chrome_tab.h"
#include "chrome_frame/test/simulate_input.h"
#include "chrome_frame/test_utils.h"

namespace chrome_frame_test {

class IEEventListener {
 public:
  virtual ~IEEventListener() {}

  
  virtual void OnNavigateError(IDispatch* dispatch, VARIANT* url,
                               VARIANT* frame_name, VARIANT* status_code,
                               VARIANT* cancel) {}
  
  
  virtual void OnBeforeNavigate2(IDispatch* dispatch, VARIANT* url,
                                 VARIANT* flags, VARIANT* target_frame_name,
                                 VARIANT* post_data, VARIANT* headers,
                                 VARIANT_BOOL* cancel) {}
  virtual void OnDownloadBegin() {}
  virtual void OnNavigateComplete2(IDispatch* dispatch, VARIANT* url) {}
  virtual void OnNewWindow2(IDispatch** dispatch, VARIANT_BOOL* cancel) {}
  virtual void OnNewWindow3(IDispatch** dispatch, VARIANT_BOOL* cancel,
                            DWORD flags, BSTR url_context, BSTR url) {}
  
  virtual void OnDocumentComplete(IDispatch* dispatch, VARIANT* url_variant) {}
  virtual void OnFileDownload(VARIANT_BOOL active_doc, VARIANT_BOOL* cancel) {}
  virtual void OnQuit() {}

  
  virtual void OnLoad(const wchar_t* url) {}
  virtual void OnLoadError(const wchar_t* url) {}
  virtual void OnMessage(const wchar_t* message, const wchar_t* origin,
                         const wchar_t* source) {}
  virtual void OnNewBrowserWindow(IDispatch* new_window, const wchar_t* url) {}
};

class PropertyNotifySinkListener {
 public:
  virtual ~PropertyNotifySinkListener() {}
  virtual void OnChanged(DISPID dispid) {}
  virtual void OnRequestEdit(DISPID dispid) {}
};

class IEEventSink
    : public CComObjectRootEx<CComSingleThreadModel>,
      public IDispEventSimpleImpl<0, IEEventSink,
                                  &DIID_DWebBrowserEvents2>,
      public IUnknown {
 public:
  typedef IDispEventSimpleImpl<0, IEEventSink,
                               &DIID_DWebBrowserEvents2> DispEventsImpl;
  IEEventSink();
  ~IEEventSink();

  
  
  HRESULT LaunchIEAndNavigate(const std::wstring& navigate_url,
                              IEEventListener* listener);

  
  HRESULT Navigate(const std::wstring& navigate_url);

  
  
  void Attach(IDispatch* browser_disp);

  
  HRESULT Attach(IWebBrowser2* browser);

  
  
  void Uninitialize();

  
  
  HRESULT CloseWebBrowser();

  
  void PostMessageToCF(const std::wstring& message, const std::wstring& target);

    
  void SetFocusToRenderer();

  
  
  void SendKeys(const char* input_string);

  
  
  void SendMouseClick(int x, int y, simulate_input::MouseButton button);

  
  
  HWND GetBrowserWindow();

  
  
  HWND GetRendererWindow();

  
  
  HWND GetRendererWindowSafe();

  
  bool IsCFRendering();

  
  void ExpectRendererWindowHasFocus();

  
  void ExpectAddressBarUrl(const std::wstring& url);

  
  
  void GoBack() {
    web_browser2_->GoBack();
  }

  void GoForward() {
    web_browser2_->GoForward();
  }

  void Refresh();

  void Exec(const GUID* cmd_group_guid, DWORD command_id,
            DWORD cmd_exec_opt, VARIANT* in_args, VARIANT* out_args);

  
  void set_listener(IEEventListener* listener) { listener_ = listener; }

  IWebBrowser2* web_browser2() { return web_browser2_.get(); }

  
  bool reference_count() { return m_dwRef; }

  static void SetAbnormalShutdown(bool abnormal_shutdown);

 private:
  void ConnectToChromeFrame();
  void DisconnectFromChromeFrame();
  void FindIEProcessId();

  
BEGIN_COM_MAP(IEEventSink)
  COM_INTERFACE_ENTRY(IUnknown)
END_COM_MAP()

BEGIN_SINK_MAP(IEEventSink)
  SINK_ENTRY_INFO(0, DIID_DWebBrowserEvents2, DISPID_BEFORENAVIGATE2,
                  OnBeforeNavigate2, &kBeforeNavigate2Info)
  SINK_ENTRY_INFO(0, DIID_DWebBrowserEvents2, DISPID_DOWNLOADBEGIN,
                  OnDownloadBegin, &kVoidMethodInfo)
  SINK_ENTRY_INFO(0, DIID_DWebBrowserEvents2, DISPID_NAVIGATECOMPLETE2,
                  OnNavigateComplete2, &kNavigateComplete2Info)
  SINK_ENTRY_INFO(0, DIID_DWebBrowserEvents2, DISPID_NAVIGATEERROR,
                  OnNavigateError, &kNavigateErrorInfo)
  SINK_ENTRY_INFO(0, DIID_DWebBrowserEvents2, DISPID_NEWWINDOW2,
                  OnNewWindow2, &kNewWindow2Info)
  SINK_ENTRY_INFO(0, DIID_DWebBrowserEvents2, DISPID_NEWWINDOW3,
                  OnNewWindow3, &kNewWindow3Info)
  SINK_ENTRY_INFO(0, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE,
                  OnDocumentComplete, &kDocumentCompleteInfo)
  SINK_ENTRY_INFO(0, DIID_DWebBrowserEvents2, DISPID_FILEDOWNLOAD,
                  OnFileDownload, &kFileDownloadInfo)
  SINK_ENTRY_INFO(0, DIID_DWebBrowserEvents2, DISPID_ONQUIT,
                  OnQuit, &kVoidMethodInfo)
END_SINK_MAP()

  STDMETHOD_(void, OnNavigateError)(IDispatch* dispatch, VARIANT* url,
                                    VARIANT* frame_name, VARIANT* status_code,
                                    VARIANT* cancel);
  STDMETHOD(OnBeforeNavigate2)(IDispatch* dispatch, VARIANT* url,
                               VARIANT* flags, VARIANT* target_frame_name,
                               VARIANT* post_data, VARIANT* headers,
                               VARIANT_BOOL* cancel);
  STDMETHOD_(void, OnDownloadBegin)();
  STDMETHOD_(void, OnNavigateComplete2)(IDispatch* dispatch, VARIANT* url);
  STDMETHOD_(void, OnNewWindow2)(IDispatch** dispatch, VARIANT_BOOL* cancel);
  STDMETHOD_(void, OnNewWindow3)(IDispatch** dispatch, VARIANT_BOOL* cancel,
                                 DWORD flags, BSTR url_context, BSTR url);
  STDMETHOD_(void, OnDocumentComplete)(IDispatch* dispatch,
                                       VARIANT* url_variant);
  STDMETHOD_(void, OnFileDownload)(VARIANT_BOOL active_doc,
                                   VARIANT_BOOL* cancel);
  STDMETHOD_(void, OnQuit)();

  STDMETHOD(Invoke)(DISPID dispid,
                    REFIID riid, LCID lcid,
                    WORD flags,
                    DISPPARAMS* params,
                    VARIANT* result,
                    EXCEPINFO* except_info,
                    UINT* arg_error);

  
  HRESULT OnLoad(const VARIANT* param);
  HRESULT OnLoadError(const VARIANT* param);
  HRESULT OnMessage(const VARIANT* param);

  base::win::ScopedComPtr<IWebBrowser2> web_browser2_;
  base::win::ScopedComPtr<IChromeFrame> chrome_frame_;
  DispCallback<IEEventSink> onmessage_;
  DispCallback<IEEventSink> onloaderror_;
  DispCallback<IEEventSink> onload_;
  IEEventListener* listener_;
  base::ProcessId ie_process_id_;
  bool did_receive_on_quit_;
  static bool abnormal_shutdown_;

  static _ATL_FUNC_INFO kBeforeNavigate2Info;
  static _ATL_FUNC_INFO kNavigateComplete2Info;
  static _ATL_FUNC_INFO kNavigateErrorInfo;
  static _ATL_FUNC_INFO kNewWindow2Info;
  static _ATL_FUNC_INFO kNewWindow3Info;
  static _ATL_FUNC_INFO kVoidMethodInfo;
  static _ATL_FUNC_INFO kDocumentCompleteInfo;
  static _ATL_FUNC_INFO kFileDownloadInfo;
};

class PropertyNotifySinkImpl
    : public CComObjectRootEx<CComSingleThreadModel>,
      public IPropertyNotifySink {
 public:
  PropertyNotifySinkImpl() : listener_(NULL) {
  }

BEGIN_COM_MAP(PropertyNotifySinkImpl)
  COM_INTERFACE_ENTRY(IPropertyNotifySink)
END_COM_MAP()

  STDMETHOD(OnChanged)(DISPID dispid) {
    if (listener_)
      listener_->OnChanged(dispid);
    return S_OK;
  }

  STDMETHOD(OnRequestEdit)(DISPID dispid) {
    if (listener_)
      listener_->OnRequestEdit(dispid);
    return S_OK;
  }

  void set_listener(PropertyNotifySinkListener* listener) {
    DCHECK(listener_ == NULL || listener == NULL);
    listener_ = listener;
  }

 protected:
  PropertyNotifySinkListener* listener_;
};

}  

#endif  
