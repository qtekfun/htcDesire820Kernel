// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_TEST_MOCK_IE_EVENT_SINK_TEST_H_
#define CHROME_FRAME_TEST_MOCK_IE_EVENT_SINK_TEST_H_

#include <atlbase.h>
#include <atlcom.h>
#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "base/win/object_watcher.h"
#include "chrome_frame/test/chrome_frame_test_utils.h"
#include "chrome_frame/test/chrome_frame_ui_test_utils.h"
#include "chrome_frame/test/ie_event_sink.h"
#include "chrome_frame/test/test_server.h"
#include "chrome_frame/test/test_with_web_server.h"
#include "chrome_frame/test/win_event_receiver.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/xulrunner-sdk/win/include/accessibility/AccessibleEventId.h"

namespace chrome_frame_test {

enum LoadedInRenderer {
  IN_IE = 0,
  IN_CF
};

class MockIEEventSink : public IEEventListener {
 public:
  MockIEEventSink() {
    CComObject<IEEventSink>::CreateInstance(&event_sink_);
    event_sink_->AddRef();
  }

  ~MockIEEventSink() {
    Detach();
    int reference_count = event_sink_->reference_count();
    LOG_IF(ERROR, reference_count != 1)
        << "Event sink is still referenced externally: ref count = "
        << reference_count;
    event_sink_->Release();
  }

  
  MOCK_METHOD7(OnBeforeNavigate2, void (IDispatch* dispatch,  
                                        VARIANT* url,
                                        VARIANT* flags,
                                        VARIANT* target_frame_name,
                                        VARIANT* post_data,
                                        VARIANT* headers,
                                        VARIANT_BOOL* cancel));
  MOCK_METHOD2(OnNavigateComplete2, void (IDispatch* dispatch,  
                                          VARIANT* url));
  MOCK_METHOD5(OnNewWindow3, void (IDispatch** dispatch,  
                                   VARIANT_BOOL* cancel,
                                   DWORD flags,
                                   BSTR url_context,
                                   BSTR url));
  MOCK_METHOD2(OnNewWindow2, void (IDispatch** dispatch,  
                                   VARIANT_BOOL* cancel));
  MOCK_METHOD5(OnNavigateError, void (IDispatch* dispatch,  
                                      VARIANT* url,
                                      VARIANT* frame_name,
                                      VARIANT* status_code,
                                      VARIANT* cancel));
  MOCK_METHOD2(OnFileDownload, void (VARIANT_BOOL active_doc,  
                                     VARIANT_BOOL* cancel));
  MOCK_METHOD0(OnQuit, void ());  
  MOCK_METHOD1(OnLoadError, void (const wchar_t* url));  
  MOCK_METHOD3(OnMessage, void (const wchar_t* message,  
                                const wchar_t* origin,
                                const wchar_t* source));
  MOCK_METHOD2(OnNewBrowserWindow, void (IDispatch* dispatch,  
                                         const wchar_t* url));

  
  
  MOCK_METHOD2(OnLoad, void (bool is_cf, const wchar_t* url));  

  
  
  void Attach(IDispatch* dispatch) {
    event_sink_->set_listener(this);
    event_sink_->Attach(dispatch);
  }

  void Detach() {
    event_sink_->set_listener(NULL);
    event_sink_->Uninitialize();
  }

  
  void ExpectNavigation(bool is_cf, const std::wstring& url);

  
  
  
  
  void ExpectNavigationOptionalBefore(bool is_cf, const std::wstring& url);

  
  
  
  
  void ExpectJavascriptWindowOpenNavigation(bool parent_cf, bool new_window_cf,
                                            const std::wstring& url);

  
  
  void ExpectNewWindow(MockIEEventSink* new_window_mock);

  
  void ExpectAnyNavigations();

  void ExpectDocumentReadystate(int ready_state);

  IEEventSink* event_sink() { return event_sink_; }

 private:
  
  
  virtual void OnDocumentComplete(IDispatch* dispatch, VARIANT* url);

  
  virtual void OnLoad(const wchar_t* url) {
    OnLoad(IN_CF, url);
  }

  
  
  
  
  
  testing::ExpectationSet ExpectNavigationCardinality(const std::wstring& url,
      testing::Cardinality before_cardinality,
      testing::Cardinality complete_cardinality);

  
  
  
  
  
  
  CComObject<IEEventSink>* event_sink_;
};

class MockPropertyNotifySinkListener : public PropertyNotifySinkListener {
 public:
  MockPropertyNotifySinkListener() : cookie_(0), sink_(NULL) {
    CComObject<PropertyNotifySinkImpl>::CreateInstance(&sink_);
    sink_->AddRef();
    sink_->set_listener(this);
  }

  ~MockPropertyNotifySinkListener() {
    Detach();
    sink_->set_listener(NULL);
    LOG_IF(ERROR, sink_->m_dwRef != 1)
        << "Event sink is still referenced externally: ref count = "
        << sink_->m_dwRef;
    sink_->Release();
  }

  
  MOCK_METHOD1(OnChanged, void (DISPID dispid));  

  bool Attach(IUnknown* obj) {
    DCHECK_EQ(cookie_, 0UL);
    DCHECK(obj);
    HRESULT hr = AtlAdvise(obj, sink_->GetUnknown(), IID_IPropertyNotifySink,
                           &cookie_);
    if (SUCCEEDED(hr)) {
      event_source_ = obj;
    } else {
      LOG(ERROR) << base::StringPrintf("AtlAdvise: 0x%08X", hr);
      cookie_ = 0;
    }

    return SUCCEEDED(hr);
  }

  void Detach() {
    if (event_source_) {
      DCHECK_NE(cookie_, 0UL);
      AtlUnadvise(event_source_, IID_IPropertyNotifySink, cookie_);
      event_source_.Release();
      cookie_ = 0;
    }
  }

 private:
  CComObject<PropertyNotifySinkImpl>* sink_;
  DWORD cookie_;
  base::win::ScopedComPtr<IUnknown> event_source_;
};

class MockObjectWatcherDelegate : public base::win::ObjectWatcher::Delegate {
 public:
  
  MOCK_METHOD1(OnObjectSignaled, void (HANDLE process_handle));  

  virtual ~MockObjectWatcherDelegate() {
    
    
    for (std::vector<HANDLE>::iterator it = process_handles_.begin();
         it != process_handles_.end(); ++it) {
      ::CloseHandle(*it);
    }
  }

  
  void WatchProcess(HANDLE process_handle) {
    process_handles_.push_back(process_handle);
    object_watcher_.StartWatching(process_handle, this);
  }

  
  void WatchProcessForHwnd(HWND hwnd) {
    DWORD pid = 0;
    ::GetWindowThreadProcessId(hwnd, &pid);
    EXPECT_TRUE(pid);
    if (pid != 0) {
      HANDLE process_handle = ::OpenProcess(SYNCHRONIZE, FALSE, pid);
      EXPECT_TRUE(process_handle);
      if (process_handle != NULL) {
        WatchProcess(process_handle);
      }
    }
  }

 private:
  std::vector<HANDLE> process_handles_;
  base::win::ObjectWatcher object_watcher_;
};

class MockWindowObserver : public WindowObserver {
 public:
  
  MOCK_METHOD1(OnWindowOpen, void (HWND hwnd));  
  MOCK_METHOD1(OnWindowClose, void (HWND hwnd));  

  void WatchWindow(std::string caption_pattern, std::string class_pattern) {
    window_watcher_.AddObserver(this, caption_pattern, class_pattern);
  }

 private:
  WindowWatchdog window_watcher_;
};

class MockAccEventObserver : public AccEventObserver {
 public:
  MOCK_METHOD1(OnAccDocLoad, void (HWND));  
  MOCK_METHOD3(OnAccValueChange, void (HWND, AccObject*,  
                                       const std::wstring&));
  MOCK_METHOD1(OnMenuPopup, void (HWND));  
  MOCK_METHOD2(OnTextCaretMoved, void (HWND, AccObject*));  
};

class MockIEEventSinkTest {
 public:
  MockIEEventSinkTest();
  MockIEEventSinkTest(int port, const std::wstring& address,
                      const base::FilePath& root_dir);

  ~MockIEEventSinkTest() {
    
    
    ie_mock_.Detach();
  }

  
  
  
  void LaunchIEAndNavigate(const std::wstring& url);

  
  void LaunchIENavigateAndLoop(const std::wstring& url,
                               base::TimeDelta timeout);

  
  
  std::wstring GetTestUrl(const std::wstring& relative_path);

  
  
  base::FilePath GetTestFilePath(const std::wstring& relative_path);

  
  
  std::wstring GetSimplePageUrl() {
    return GetTestUrl(L"simple.html");
  }

  
  std::wstring GetSimplePageTitle() {
    return L"simple web page";
  }

  
  
  std::wstring GetLinkPageUrl() {
    return GetTestUrl(L"link.html");
  }

  
  std::wstring GetLinkPageTitle() {
    return L"link";
  }

  
  
  
  
  std::wstring GetAnchorPageUrl(int index) {
    DCHECK_LT(index, 5);
    std::wstring base_name = L"anchor.html";
    if (index > 0)
      base_name += std::wstring(L"#a") + base::IntToString16(index);
    return GetTestUrl(base_name);
  }

  
  std::wstring GetAnchorPageTitle() {
    return L"Chrome Frame Test";
  }

  
  
  std::wstring GetWindowOpenUrl(const wchar_t* target) {
    return GetTestUrl(std::wstring(L"window_open.html?").append(target));
  }

  
  std::wstring GetWindowOpenTitle() {
    return L"window open";
  }

 protected:
  CloseIeAtEndOfScope last_resort_close_ie_;
  chrome_frame_test::TimedMsgLoop loop_;
  testing::StrictMock<MockIEEventSink> ie_mock_;
  testing::StrictMock<MockWebServer> server_mock_;
  scoped_refptr<HungCOMCallDetector> hung_call_detector_;
 private:
  DISALLOW_COPY_AND_ASSIGN(MockIEEventSinkTest);
};

}  

#endif  
