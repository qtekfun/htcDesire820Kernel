// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_DRIVE_AUTH_SERVICE_H_
#define GOOGLE_APIS_DRIVE_AUTH_SERVICE_H_

#include <string>
#include <vector>

#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/threading/thread_checker.h"
#include "google_apis/drive/auth_service_interface.h"
#include "google_apis/gaia/oauth2_token_service.h"

namespace net {
class URLRequestContextGetter;
}

namespace google_apis {

class AuthServiceObserver;

class AuthService : public AuthServiceInterface,
                    public OAuth2TokenService::Observer {
 public:
  
  
  
  
  AuthService(OAuth2TokenService* oauth2_token_service,
              const std::string& account_id,
              net::URLRequestContextGetter* url_request_context_getter,
              const std::vector<std::string>& scopes);
  virtual ~AuthService();

  
  virtual void AddObserver(AuthServiceObserver* observer) OVERRIDE;
  virtual void RemoveObserver(AuthServiceObserver* observer) OVERRIDE;
  virtual void StartAuthentication(const AuthStatusCallback& callback) OVERRIDE;
  virtual bool HasAccessToken() const OVERRIDE;
  virtual bool HasRefreshToken() const OVERRIDE;
  virtual const std::string& access_token() const OVERRIDE;
  virtual void ClearAccessToken() OVERRIDE;
  virtual void ClearRefreshToken() OVERRIDE;

  
  virtual void OnRefreshTokenAvailable(const std::string& account_id) OVERRIDE;
  virtual void OnRefreshTokenRevoked(const std::string& account_id) OVERRIDE;

 private:
  
  void OnHandleRefreshToken(bool has_refresh_token);

  
  
  void OnAuthCompleted(const AuthStatusCallback& callback,
                       GDataErrorCode error,
                       const std::string& access_token);

  OAuth2TokenService* oauth2_token_service_;
  std::string account_id_;
  scoped_refptr<net::URLRequestContextGetter> url_request_context_getter_;
  bool has_refresh_token_;
  std::string access_token_;
  std::vector<std::string> scopes_;
  ObserverList<AuthServiceObserver> observers_;
  base::ThreadChecker thread_checker_;

  
  
  base::WeakPtrFactory<AuthService> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(AuthService);
};

}  

#endif  
