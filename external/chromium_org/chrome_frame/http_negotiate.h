// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_HTTP_NEGOTIATE_H_
#define CHROME_FRAME_HTTP_NEGOTIATE_H_

#include <shdeprecated.h>
#include <urlmon.h>

#include <string>

#include "base/basictypes.h"
#include "base/win/scoped_comptr.h"

typedef HRESULT (STDMETHODCALLTYPE* IHttpNegotiate_BeginningTransaction_Fn)(
    IHttpNegotiate* me, LPCWSTR url, LPCWSTR headers, DWORD reserved,
    LPWSTR* additional_headers);
typedef HRESULT (STDMETHODCALLTYPE* IHttpNegotiate_OnResponse_Fn)(
    IHttpNegotiate* me, DWORD response_code, LPCWSTR response_header,
    LPCWSTR request_header, LPWSTR* additional_request_headers);

typedef HRESULT (STDMETHODCALLTYPE* IBindStatusCallback_StartBinding_Fn)(
    IBindStatusCallback* me, DWORD reserved, IBinding *binding);

typedef HRESULT (STDMETHODCALLTYPE* IInternetProtocolSink_ReportProgress_Fn)(
    IInternetProtocolSink* me, ULONG status_code, LPCWSTR status_text);


class HttpNegotiatePatch {
  
  HttpNegotiatePatch();
  ~HttpNegotiatePatch();

 public:
  static bool Initialize();
  static void Uninitialize();

  
  
  static void set_modify_user_agent(bool value) { modify_user_agent_ = value; }

  
  static STDMETHODIMP BeginningTransaction(
      IHttpNegotiate_BeginningTransaction_Fn original, IHttpNegotiate* me,
      LPCWSTR url, LPCWSTR headers, DWORD reserved, LPWSTR* additional_headers);

 protected:
  static HRESULT PatchHttpNegotiate(IUnknown* to_patch);

 private:
  static bool modify_user_agent_;

  DISALLOW_COPY_AND_ASSIGN(HttpNegotiatePatch);
};

extern const int LOCAL_BINDSTATUS_SERVER_MIMETYPEAVAILABLE;

std::string AppendCFUserAgentString(LPCWSTR headers,
                                    LPCWSTR additional_headers);

std::string ReplaceOrAddUserAgent(LPCWSTR headers,
                                  const std::string& user_agent_value);

#endif  
