// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_IDENTITY_GAIA_WEB_AUTH_FLOW_H_
#define CHROME_BROWSER_EXTENSIONS_API_IDENTITY_GAIA_WEB_AUTH_FLOW_H_

#include "chrome/browser/extensions/api/identity/web_auth_flow.h"
#include "chrome/browser/signin/ubertoken_fetcher.h"
#include "chrome/browser/ui/host_desktop.h"
#include "chrome/common/extensions/api/identity/oauth2_manifest_handler.h"

namespace extensions {


class GaiaWebAuthFlow : public UbertokenConsumer, public WebAuthFlow::Delegate {
 public:
  enum Failure {
    WINDOW_CLOSED,  
    INVALID_REDIRECT,  
    SERVICE_AUTH_ERROR,  
    OAUTH_ERROR,  
    LOAD_FAILED  
  };

  class Delegate {
   public:
    
    
    virtual void OnGaiaFlowFailure(Failure failure,
                                   GoogleServiceAuthError service_error,
                                   const std::string& oauth_error) = 0;
    
    virtual void OnGaiaFlowCompleted(const std::string& access_token,
                                     const std::string& expiration) = 0;
  };

  GaiaWebAuthFlow(Delegate* delegate,
                  Profile* profile,
                  const std::string& extension_id,
                  const OAuth2Info& oauth2_info,
                  const std::string& locale);
  virtual ~GaiaWebAuthFlow();

  
  virtual void Start();

  
  virtual void OnUbertokenSuccess(const std::string& token) OVERRIDE;
  virtual void OnUbertokenFailure(const GoogleServiceAuthError& error) OVERRIDE;

  
  virtual void OnAuthFlowFailure(WebAuthFlow::Failure failure) OVERRIDE;
  virtual void OnAuthFlowURLChange(const GURL& redirect_url) OVERRIDE;
  virtual void OnAuthFlowTitleChange(const std::string& title) OVERRIDE;

 private:
  
  
  virtual scoped_ptr<WebAuthFlow> CreateWebAuthFlow(GURL url);

  Delegate* delegate_;
  Profile* profile_;
  chrome::HostDesktopType host_desktop_type_;
  std::string redirect_scheme_;
  std::string redirect_path_prefix_;
  GURL auth_url_;
  scoped_ptr<UbertokenFetcher> ubertoken_fetcher_;
  scoped_ptr<WebAuthFlow> web_flow_;

  DISALLOW_COPY_AND_ASSIGN(GaiaWebAuthFlow);
};

}  

#endif  
