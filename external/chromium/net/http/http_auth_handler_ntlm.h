// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_AUTH_HANDLER_NTLM_H_
#define NET_HTTP_HTTP_AUTH_HANDLER_NTLM_H_
#pragma once

#include "build/build_config.h"

#if defined(OS_WIN)
#define NTLM_SSPI
#else
#define NTLM_PORTABLE
#endif

#if defined(NTLM_SSPI)
#define SECURITY_WIN32 1
#include <windows.h>
#include <security.h>
#include "net/http/http_auth_sspi_win.h"
#endif

#include <string>

#include "base/basictypes.h"
#include "base/string16.h"
#include "net/http/http_auth_handler.h"
#include "net/http/http_auth_handler_factory.h"

namespace net {

class URLSecurityManager;

class HttpAuthHandlerNTLM : public HttpAuthHandler {
 public:
  class Factory : public HttpAuthHandlerFactory {
   public:
    Factory();
    virtual ~Factory();

    virtual int CreateAuthHandler(HttpAuth::ChallengeTokenizer* challenge,
                                  HttpAuth::Target target,
                                  const GURL& origin,
                                  CreateReason reason,
                                  int digest_nonce_count,
                                  const BoundNetLog& net_log,
                                  scoped_ptr<HttpAuthHandler>* handler);
#if defined(NTLM_SSPI)
    
    
    
    
    
    
    void set_sspi_library(SSPILibrary* sspi_library) {
      sspi_library_ = sspi_library;
    }
#endif  
   private:
#if defined(NTLM_SSPI)
    ULONG max_token_length_;
    bool first_creation_;
    bool is_unsupported_;
    SSPILibrary* sspi_library_;
#endif  
  };

#if defined(NTLM_PORTABLE)
  
  typedef void (*GenerateRandomProc)(uint8* output, size_t n);

  
  
  typedef std::string (*HostNameProc)();

  
  
  class ScopedProcSetter {
   public:
    ScopedProcSetter(GenerateRandomProc random_proc,
                     HostNameProc host_name_proc) {
      old_random_proc_ = SetGenerateRandomProc(random_proc);
      old_host_name_proc_ = SetHostNameProc(host_name_proc);
    }

    ~ScopedProcSetter() {
      SetGenerateRandomProc(old_random_proc_);
      SetHostNameProc(old_host_name_proc_);
    }

   private:
    GenerateRandomProc old_random_proc_;
    HostNameProc old_host_name_proc_;
  };
#endif

#if defined(NTLM_PORTABLE)
  HttpAuthHandlerNTLM();
#endif
#if defined(NTLM_SSPI)
  HttpAuthHandlerNTLM(SSPILibrary* sspi_library, ULONG max_token_length,
                      URLSecurityManager* url_security_manager);
#endif

  virtual bool NeedsIdentity();

  virtual bool AllowsDefaultCredentials();

  virtual HttpAuth::AuthorizationResult HandleAnotherChallenge(
      HttpAuth::ChallengeTokenizer* challenge);

 protected:
  
  
  int InitializeBeforeFirstChallenge();

  virtual bool Init(HttpAuth::ChallengeTokenizer* tok);

  virtual int GenerateAuthTokenImpl(const string16* username,
                                    const string16* password,
                                    const HttpRequestInfo* request,
                                    CompletionCallback* callback,
                                    std::string* auth_token);

 private:
  ~HttpAuthHandlerNTLM();

#if defined(NTLM_PORTABLE)
  
  
  static GenerateRandomProc SetGenerateRandomProc(GenerateRandomProc proc);
  static HostNameProc SetHostNameProc(HostNameProc proc);
#endif

  
  HttpAuth::AuthorizationResult ParseChallenge(
      HttpAuth::ChallengeTokenizer* tok, bool initial_challenge);

  
  
  int GetNextToken(const void* in_token,
                   uint32 in_token_len,
                   void** out_token,
                   uint32* out_token_len);

  
  static std::wstring CreateSPN(const GURL& origin);

#if defined(NTLM_SSPI)
  HttpAuthSSPI auth_sspi_;
#endif

#if defined(NTLM_PORTABLE)
  static GenerateRandomProc generate_random_proc_;
  static HostNameProc get_host_name_proc_;
#endif

  string16 domain_;
  string16 username_;
  string16 password_;

  
  
  std::string auth_data_;

#if defined(NTLM_SSPI)
  URLSecurityManager* url_security_manager_;
#endif
};

}  

#endif  
