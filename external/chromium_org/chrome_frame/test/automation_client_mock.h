// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_TEST_AUTOMATION_CLIENT_MOCK_H_
#define CHROME_FRAME_TEST_AUTOMATION_CLIENT_MOCK_H_

#include <windows.h>
#include <string>

#include "base/strings/string_util.h"
#include "chrome_frame/chrome_frame_automation.h"
#include "chrome_frame/test/chrome_frame_test_utils.h"
#include "chrome_frame/test/proxy_factory_mock.h"
#include "chrome_frame/utils.h"
#include "gmock/gmock.h"

using testing::StrictMock;

struct MockCFDelegate : public ChromeFrameDelegateImpl {
  MOCK_CONST_METHOD0(GetWindow, WindowType());
  MOCK_METHOD1(GetBounds, void(RECT* bounds));
  MOCK_METHOD0(GetDocumentUrl, std::string());
  MOCK_METHOD2(ExecuteScript, bool(const std::string& script,
                                   std::string* result));
  MOCK_METHOD0(OnAutomationServerReady, void());
  MOCK_METHOD2(OnAutomationServerLaunchFailed, void(
      AutomationLaunchResult reason, const std::string& server_version));
  
  
  MOCK_METHOD2(OnLoadFailed, void(int error_code, const std::string& url));

  
  
  

  MOCK_METHOD0(OnChannelError, void(void));
  MOCK_METHOD1(OnNavigationStateChanged, void(int flags));
  MOCK_METHOD1(OnUpdateTargetUrl, void(
      const std::wstring& new_target_url));
  MOCK_METHOD1(OnAcceleratorPressed, void(const MSG& accel_message));
  MOCK_METHOD1(OnTabbedOut, void(bool reverse));
  MOCK_METHOD2(OnOpenURL, void(const GURL& url, int open_disposition));
  MOCK_METHOD1(OnDidNavigate, void(
      const NavigationInfo& navigation_info));
  MOCK_METHOD2(OnNavigationFailed, void(int error_code, const GURL& gurl));
  MOCK_METHOD1(OnLoad, void(const GURL& url));
  MOCK_METHOD3(OnMessageFromChromeFrame, void(
      const std::string& message,
      const std::string& origin,
      const std::string& target));
  MOCK_METHOD3(OnHandleContextMenu, void(HANDLE menu_handle,
      int align_flags, const MiniContextMenuParams& params));
  MOCK_METHOD2(OnRequestStart, void(int request_id,
      const AutomationURLRequest& request));
  MOCK_METHOD2(OnRequestRead, void(int request_id, int bytes_to_read));
  MOCK_METHOD2(OnRequestEnd, void(int request_id,
      const net::URLRequestStatus& status));

  
  void SetRequestDelegate(PluginUrlRequestDelegate* request_delegate) {
    request_delegate_ = request_delegate;
  }

  void ReplyStarted(int request_id, const char* headers) {
    request_delegate_->OnResponseStarted(request_id, "text/html", headers,
      0, base::Time::Now(), std::string(), 0, net::HostPortPair(), 0);
  }

  void ReplyData(int request_id, const std::string* data) {
    request_delegate_->OnReadComplete(request_id, *data);
  }

  void Reply(const net::URLRequestStatus& status, int request_id) {
    request_delegate_->OnResponseEnd(request_id, status);
  }

  void Reply404(int request_id) {
    ReplyStarted(request_id, "HTTP/1.1 404\r\n\r\n");
    Reply(net::URLRequestStatus(), request_id);
  }

  PluginUrlRequestDelegate* request_delegate_;
};

class MockAutomationProxy : public ChromeFrameAutomationProxy {
 public:
  MOCK_METHOD1(Send, bool(IPC::Message*));
  MOCK_METHOD3(SendAsAsync,
               void(IPC::SyncMessage* msg,
                    SyncMessageReplyDispatcher::SyncMessageCallContext* context,
                    void* key));
  MOCK_METHOD1(CancelAsync, void(void* key));
  MOCK_METHOD1(CreateTabProxy, scoped_refptr<TabProxy>(int handle));
  MOCK_METHOD1(ReleaseTabProxy, void(AutomationHandle handle));
  MOCK_METHOD0(server_version, std::string(void));
  MOCK_METHOD1(SendProxyConfig, void(const std::string&));

  ~MockAutomationProxy() {}
};

struct MockAutomationMessageSender : public AutomationMessageSender {
  virtual bool Send(IPC::Message* msg) {
    return proxy_->Send(msg);
  }

  virtual bool Send(IPC::Message* msg, int timeout_ms) {
    return proxy_->Send(msg);
  }

  void ForwardTo(StrictMock<MockAutomationProxy> *p) {
    proxy_ = p;
  }

  StrictMock<MockAutomationProxy>* proxy_;
};


class CFACMockTest : public testing::Test {
 public:
  MockProxyFactory factory_;
  MockCFDelegate   cfd_;
  chrome_frame_test::TimedMsgLoop loop_;
  
  
  
  
  StrictMock<MockAutomationProxy> mock_proxy_;
  ChromeFrameAutomationProxy* returned_proxy_;
  scoped_ptr<AutomationHandleTracker> tracker_;
  MockAutomationMessageSender dummy_sender_;
  scoped_refptr<TabProxy> tab_;
  
  scoped_refptr<ChromeFrameAutomationClient> client_;

  base::FilePath profile_path_;
  int timeout_;
  void* id_;  
  int tab_handle_;   

  inline ChromeFrameAutomationProxy* get_proxy() {
    return returned_proxy_;
  }

  inline void CreateTab() {
    ASSERT_EQ(NULL, tab_.get());
    tab_ = new TabProxy(&dummy_sender_, tracker_.get(), tab_handle_);
  }

  
  void SetAutomationServerOk(int times);
  void Set_CFD_LaunchFailed(AutomationLaunchResult result);

 protected:
  CFACMockTest()
    : timeout_(500),
      returned_proxy_(static_cast<ChromeFrameAutomationProxy*>(&mock_proxy_)) {
    GetChromeFrameProfilePath(L"Adam.N.Epilinter", &profile_path_);
    id_ = reinterpret_cast<void*>(5);
    tab_handle_ = 3;
  }

  virtual void SetUp() {
    dummy_sender_.ForwardTo(&mock_proxy_);
    tracker_.reset(new AutomationHandleTracker());

    client_ = new ChromeFrameAutomationClient;
    client_->set_proxy_factory(&factory_);
  }
};

#endif  
