// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_TEST_CHROME_FRAME_TEST_UTILS_H_
#define CHROME_FRAME_TEST_CHROME_FRAME_TEST_UTILS_H_

#include <windows.h>

#include <atlbase.h>
#include <atlwin.h>

#include <string>

#include "base/basictypes.h"
#include "base/cancelable_callback.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/process/process_handle.h"
#include "base/run_loop.h"
#include "base/test/test_reg_util_win.h"
#include "base/time/time.h"
#include "base/time/time.h"
#include "base/win/registry.h"
#include "base/win/scoped_comptr.h"
#include "chrome_frame/chrome_tab.h"
#include "chrome_frame/test/simulate_input.h"
#include "chrome_frame/test_utils.h"
#include "chrome_frame/utils.h"

#include "gtest/gtest.h"

#define GMOCK_MUTANT_INCLUDE_LATE_OBJECT_BINDING
#include "testing/gmock_mutant.h"

interface IWebBrowser2;

namespace base {
class FilePath;
}

namespace chrome_frame_test {

int CloseVisibleWindowsOnAllThreads(HANDLE process);

base::ProcessHandle LaunchIE(const std::wstring& url);
base::ProcessHandle LaunchChrome(const std::wstring& url,
                                 const base::FilePath& user_data_dir);

int CloseAllIEWindows();

bool TakeSnapshotAndLog();

extern const wchar_t kIEImageName[];
extern const wchar_t kIEBrokerImageName[];
extern const char kChromeImageName[];
extern const wchar_t kChromeLauncher[];
extern const base::TimeDelta kChromeFrameLongNavigationTimeout;
extern const base::TimeDelta kChromeFrameVeryLongNavigationTimeout;

class LowIntegrityToken {
 public:
  LowIntegrityToken();
  ~LowIntegrityToken();
  BOOL Impersonate();
  BOOL RevertToSelf();
 protected:
  static bool IsImpersonated();
  bool impersonated_;
};

class HungCOMCallDetector
    : public CComObjectRootEx<CComMultiThreadModel>,
      public IMessageFilter,
      public CWindowImpl<HungCOMCallDetector> {
 public:
  HungCOMCallDetector()
      : is_hung_(false) {
  }

  ~HungCOMCallDetector() {
    TearDown();
  }

  BEGIN_MSG_MAP(HungCOMCallDetector)
    MESSAGE_HANDLER(WM_TIMER, OnTimer)
  END_MSG_MAP()

  BEGIN_COM_MAP(HungCOMCallDetector)
    COM_INTERFACE_ENTRY(IMessageFilter)
  END_COM_MAP()

  static CComObject<HungCOMCallDetector>* Setup(int timeout_seconds) {
    CComObject<HungCOMCallDetector>* this_instance  = NULL;
    CComObject<HungCOMCallDetector>::CreateInstance(&this_instance);
    EXPECT_TRUE(this_instance != NULL);
    scoped_refptr<HungCOMCallDetector> ref_instance = this_instance;

    HRESULT hr = ref_instance->Initialize(timeout_seconds);
    if (FAILED(hr)) {
      LOG(ERROR) << "Failed to Initialize Hung COM call detector. Error:"
                 << hr;
      return NULL;
    }
    return this_instance;
  }

  void TearDown() {
    if (prev_filter_) {
      base::win::ScopedComPtr<IMessageFilter> prev_filter;
      CoRegisterMessageFilter(prev_filter_.get(), prev_filter.Receive());
      DCHECK(prev_filter.IsSameObject(this));
      prev_filter_.Release();
    }
    if (IsWindow()) {
      DestroyWindow();
      m_hWnd = NULL;
    }
  }

  STDMETHOD_(DWORD, HandleInComingCall)(DWORD call_type,
                                        HTASK caller,
                                        DWORD tick_count,
                                        LPINTERFACEINFO interface_info) {
    return SERVERCALL_ISHANDLED;
  }

  STDMETHOD_(DWORD, RetryRejectedCall)(HTASK callee,
                                       DWORD tick_count,
                                       DWORD reject_type) {
    return -1;
  }

  STDMETHOD_(DWORD, MessagePending)(HTASK callee,
                                    DWORD tick_count,
                                    DWORD pending_type) {
    MSG msg = {0};
    if (is_hung_) {
      return PENDINGMSG_CANCELCALL;
    }
    return PENDINGMSG_WAITDEFPROCESS;
  }

  bool is_hung() const {
    return is_hung_;
  }

  LRESULT OnTimer(UINT msg, WPARAM wp, LPARAM lp, BOOL& handled) {  
    is_hung_ = true;
    return 1;
  }

 private:
  HRESULT Initialize(int timeout_seconds) {
    
    
    
    
    
    Create(HWND_MESSAGE);
    if (!IsWindow()) {
      LOG(ERROR) << "Failed to create window. Error:" << GetLastError();
      return E_FAIL;
    }

    HRESULT hr = CoRegisterMessageFilter(this, prev_filter_.Receive());
    if (FAILED(hr)) {
      LOG(ERROR) << "Failed to register message filter. Error:" << hr;
      return hr;
    }

    static const int kHungDetectTimerId = 0x0000baba;
    SetTimer(kHungDetectTimerId, 1000 * (timeout_seconds + 40), NULL);
    return S_OK;
  }

  
  bool is_hung_;
  base::win::ScopedComPtr<IMessageFilter> prev_filter_;
};

class TimedMsgLoop {
 public:
  TimedMsgLoop() : snapshot_on_timeout_(false), quit_loop_invoked_(false) {
  }

  void set_snapshot_on_timeout(bool value) {
    snapshot_on_timeout_ = value;
  }

  void RunFor(base::TimeDelta duration) {
    quit_loop_invoked_ = false;
    if (snapshot_on_timeout_)
      timeout_closure_.Reset(base::Bind(&TimedMsgLoop::SnapshotAndQuit));
    else
      timeout_closure_.Reset(base::MessageLoop::QuitClosure());
    loop_.PostDelayedTask(FROM_HERE, timeout_closure_.callback(), duration);
    loop_.base::MessageLoop::Run();
    timeout_closure_.Cancel();
  }

  void PostTask(const tracked_objects::Location& from_here,
                const base::Closure& task) {
    loop_.PostTask(from_here, task);
  }

  void PostDelayedTask(const tracked_objects::Location& from_here,
                       const base::Closure& task, base::TimeDelta delay) {
    loop_.PostDelayedTask(from_here, task, delay);
  }

  void Quit() {
    
    QuitAfter(base::TimeDelta());
  }

  void QuitAfter(base::TimeDelta delay) {
    timeout_closure_.Cancel();
    quit_loop_invoked_ = true;
    loop_.PostDelayedTask(FROM_HERE, base::MessageLoop::QuitClosure(), delay);
  }

  bool WasTimedOut() const {
    return !quit_loop_invoked_;
  }

  void RunUntilIdle() {
    loop_.RunUntilIdle();
  }

 private:
  static void SnapshotAndQuit() {
    TakeSnapshotAndLog();
    base::MessageLoop::current()->Quit();
  }

  base::MessageLoopForUI loop_;
  base::CancelableClosure timeout_closure_;
  bool snapshot_on_timeout_;
  bool quit_loop_invoked_;
};

#define QUIT_LOOP(loop) testing::InvokeWithoutArgs(\
  testing::CreateFunctor(&loop, &chrome_frame_test::TimedMsgLoop::Quit))

#define QUIT_LOOP_SOON(loop, delay) testing::InvokeWithoutArgs(\
  testing::CreateFunctor(&loop, &chrome_frame_test::TimedMsgLoop::QuitAfter, \
                         delay))

HRESULT LaunchIEAsComServer(IWebBrowser2** web_browser);

std::wstring GetExecutableAppPath(const std::wstring& file);

base::FilePath GetProfilePathForIE();

std::wstring GetExeVersion(const std::wstring& exe_path);

IEVersion GetInstalledIEVersion();

base::FilePath GetTestDataFolder();

base::FilePath GetSeleniumTestFolder();

std::wstring GetPathFromUrl(const std::wstring& url);

std::wstring GetPathAndQueryFromUrl(const std::wstring& url);

bool AddCFMetaTag(std::string* html_data);

std::wstring GetClipboardText();

void DestroyClipboard();

void SetClipboardText(const std::wstring& text);

class CloseIeAtEndOfScope {
 public:
  CloseIeAtEndOfScope() {}
  ~CloseIeAtEndOfScope();
};

base::ProcessHandle StartCrashService();

class ScopedVirtualizeHklmAndHkcu {
 public:
  ScopedVirtualizeHklmAndHkcu();
  ~ScopedVirtualizeHklmAndHkcu();

 protected:
  registry_util::RegistryOverrideManager override_manager_;
};

bool KillProcesses(const std::wstring& executable_name, int exit_code,
                   bool wait);

ScopedChromeFrameRegistrar::RegistrationType GetTestBedType();

void ClearIESessionHistory();

std::string GetLocalIPv4Address();

}  

void DeleteAllSingletons();

#endif  
