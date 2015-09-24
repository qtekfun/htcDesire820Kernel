// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_TEST_TEST_WITH_WEB_SERVER_H_
#define CHROME_FRAME_TEST_TEST_WITH_WEB_SERVER_H_

#include <windows.h>
#include <string>

#include "base/files/scoped_temp_dir.h"
#include "base/strings/string_util.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "base/win/scoped_handle.h"
#include "chrome_frame/chrome_tab.h"
#include "chrome_frame/test/chrome_frame_test_utils.h"
#include "chrome_frame/test/test_server.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

class CFInvocation {
 public:
  enum Type {
    NONE = 0,
    META_TAG,
    HTTP_HEADER
  };

  CFInvocation(): method_(NONE) {}
  explicit CFInvocation(Type method): method_(method) {}

  
  static CFInvocation None() { return CFInvocation(NONE); }
  static CFInvocation MetaTag() { return CFInvocation(META_TAG); }
  static CFInvocation HttpHeader() { return CFInvocation(HTTP_HEADER); }

  
  bool invokes_cf() const {
    return method_ != NONE;
  }

  Type type() const { return method_; }

 private:
  Type method_;
};

class WebServerListener {
 public:
  virtual ~WebServerListener() {}

  
  
  virtual void OnExpectedResponse() = 0;
};

class MockWebServer : public test_server::HTTPTestServer {
 public:
  MockWebServer(int port, const std::wstring& address, base::FilePath root_dir)
      : test_server::HTTPTestServer(port, address, root_dir), listener_(NULL) {}

  
  MOCK_METHOD3(Get, void(test_server::ConfigurableConnection* connection,
                         const std::wstring& path,
                         const test_server::Request& r));
  MOCK_METHOD3(Post, void(test_server::ConfigurableConnection* connection,
                          const std::wstring& path,
                          const test_server::Request& r));

  
  
  
  
  
  void ExpectAndServeRequest(CFInvocation invocation, const std::wstring& url);

  
  
  void ExpectAndServeRequestWithCardinality(CFInvocation invocation,
                                            const std::wstring& url,
                                            testing::Cardinality cardinality);

  
  void ExpectAndServeRequestAllowCache(CFInvocation invocation,
                                       const std::wstring& url);

  
  
  void ExpectAndServeRequestAnyNumberTimes(CFInvocation invocation,
                                           const std::wstring& path_prefix);

  void set_listener(WebServerListener* listener) { listener_ = listener; }

  
  
  
  
  void ExpectAndHandlePostedResult(CFInvocation invocation,
                                   const std::wstring& post_suffix);

  
  void ExpectAndServeAnyRequests(CFInvocation invocation) {
    ExpectAndServeRequestAnyNumberTimes(invocation, L"");
  }


  
  
  
  void SendResponseHelper(test_server::ConfigurableConnection* connection,
                          const std::wstring& resource_uri,
                          const test_server::Request& request,
                          CFInvocation invocation,
                          bool add_no_cache_header);
  
  void HandlePostedResponse(test_server::ConfigurableConnection* connection,
                            const test_server::Request& request);

  void ClearResults() {
    posted_result_.clear();
    expected_result_.clear();
  }

  void set_expected_result(const std::string& expected_result) {
    expected_result_  = expected_result;
  }

  const std::string& posted_result() const {
    return posted_result_;
  }

 private:
  WebServerListener* listener_;
  
  std::string posted_result_;
  std::string expected_result_;
};

class MockWebServerListener : public WebServerListener {
 public:
  MOCK_METHOD0(OnExpectedResponse, void());
};

class ChromeFrameTestWithWebServer : public testing::Test {
 public:
  ChromeFrameTestWithWebServer();

 protected:
  enum BrowserKind { INVALID, IE, CHROME };

  bool LaunchBrowser(BrowserKind browser, const wchar_t* url);

  
  bool WaitForTestToComplete(base::TimeDelta duration);

  
  
  bool WaitForOnLoad(int milliseconds);

  
  
  
  
  
  void SimpleBrowserTestExpectedResult(BrowserKind browser,
      const wchar_t* page, const char* result);
  void SimpleBrowserTest(BrowserKind browser, const wchar_t* page);

  
  void ExpectAndHandlePostedResult();

  
  void VersionTest(BrowserKind browser, const wchar_t* page);

  void CloseBrowser();

  
  bool BringBrowserToTop();

  const base::FilePath& GetCFTestFilePath() {
    return test_file_path_;
  }

  static chrome_frame_test::TimedMsgLoop& loop() {
    return *loop_;
  }

  static testing::StrictMock<MockWebServerListener>& listener_mock() {
    return *listener_mock_;
  }

  static testing::StrictMock<MockWebServer>& server_mock() {
    return *server_mock_;
  }

  static void SetUpTestCase();
  static void TearDownTestCase();

  static const base::FilePath& GetChromeUserDataDirectory();

  virtual void SetUp() OVERRIDE;
  virtual void TearDown() OVERRIDE;

  
  static base::FilePath test_file_path_;
  static base::FilePath results_dir_;
  static base::FilePath CFInstall_path_;
  static base::FilePath CFInstance_path_;
  static base::FilePath chrome_user_data_dir_;

  
  static base::ScopedTempDir temp_dir_;

  
  static chrome_frame_test::TimedMsgLoop* loop_;
  static std::string local_address_;
  static testing::StrictMock<MockWebServerListener>* listener_mock_;
  static testing::StrictMock<MockWebServer>* server_mock_;

  BrowserKind browser_;
  base::win::ScopedHandle browser_handle_;
};

class SimpleWebServerTest {
 public:
  SimpleWebServerTest(const std::string& address, int port)
      : server_(address, port), port_(port) {
  }

  ~SimpleWebServerTest() {
    server_.DeleteAllResponses();
  }

  template <class ResponseClass>
  void PopulateStaticFileListT(const wchar_t* pages[], int count,
                               const base::FilePath& directory) {
    for (int i = 0; i < count; ++i) {
      server_.AddResponse(new ResponseClass(
          base::StringPrintf("/%ls", pages[i]).c_str(),
                             directory.Append(pages[i])));
    }
  }

  std::wstring FormatHttpPath(const wchar_t* document_path) {
    return base::StringPrintf(L"http://%ls:%i/%ls",
                              ASCIIToWide(server_.host()).c_str(), port_,
                              document_path);
  }

  
  
  const test_server::Request& last_request() const {
    const test_server::ConnectionList& connections = server_.connections();
    DCHECK(connections.size());
    const test_server::Connection* c = connections.back();
    return c->request();
  }

  bool FindRequest(const std::string& path,
                   const test_server::Request** request) {
    test_server::ConnectionList::const_iterator index;
    for (index = server_.connections().begin();
         index != server_.connections().end(); index++) {
      const test_server::Connection* connection = *index;
      if (!lstrcmpiA(connection->request().path().c_str(), path.c_str())) {
        if (request)
          *request = &connection->request();
        return true;
      }
    }
    return false;
  }

  
  
  
  int GetRequestCountForPage(const wchar_t* page, const char* expected_method) {
    
    test_server::ConnectionList::const_iterator it;
    int requests = 0;
    const test_server::ConnectionList& connections = server_.connections();
    for (it = connections.begin(); it != connections.end(); ++it) {
      const test_server::Connection* c = (*it);
      const test_server::Request& r = c->request();
      if (!r.path().empty() &&
          ASCIIToWide(r.path().substr(1)).compare(page) == 0) {
        if (expected_method) {
          EXPECT_EQ(expected_method, r.method());
        }
        requests++;
      }
    }
    return requests;
  }

  test_server::SimpleWebServer* web_server() {
    return &server_;
  }

 protected:
  test_server::SimpleWebServer server_;
  int port_;
};

ACTION_P2(SendFast, headers, content) {
  arg0->Send(headers, content);
}

ACTION_P4(Send, headers, content, chunk, timeout) {
  test_server::ConfigurableConnection::SendOptions options(
      test_server::ConfigurableConnection::SendOptions::
        IMMEDIATE_HEADERS_DELAYED_CONTENT, chunk, timeout);
  arg0->SendWithOptions(std::string(headers),
                        std::string(content),
                        options);
}

ACTION_P4(SendSlow, headers, content, chunk, timeout) {
  test_server::ConfigurableConnection::SendOptions options(
    test_server::ConfigurableConnection::SendOptions::DELAYED, chunk, timeout);
  arg0->SendWithOptions(std::string(headers),
                        std::string(content),
                        options);
}

ACTION_P2(SendResponse, server, invocation) {
  server->SendResponseHelper(arg0, arg1, arg2, invocation, true);
}

ACTION_P2(SendAllowCacheResponse, server, invocation) {
  server->SendResponseHelper(arg0, arg1, invocation, false);
}

ACTION_P2(HandlePostedResponseHelper, server, invocation) {
  server->HandlePostedResponse(arg0, arg2);
}

#endif  
