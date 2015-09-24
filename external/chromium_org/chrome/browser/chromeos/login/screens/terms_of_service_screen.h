// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_TERMS_OF_SERVICE_SCREEN_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_TERMS_OF_SERVICE_SCREEN_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer/timer.h"
#include "chrome/browser/chromeos/login/screens/terms_of_service_screen_actor.h"
#include "chrome/browser/chromeos/login/screens/wizard_screen.h"
#include "net/url_request/url_fetcher_delegate.h"

namespace net {
class URLFetcher;
}

namespace chromeos {

class ScreenObserver;

class TermsOfServiceScreen : public WizardScreen,
                             public TermsOfServiceScreenActor::Delegate,
                             public net::URLFetcherDelegate {
 public:
  TermsOfServiceScreen(ScreenObserver* screen_observer,
                       TermsOfServiceScreenActor* actor);
  virtual ~TermsOfServiceScreen();

  
  virtual void PrepareToShow() OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual std::string GetName() const OVERRIDE;

  
  virtual void OnDecline() OVERRIDE;
  virtual void OnAccept() OVERRIDE;
  virtual void OnActorDestroyed(TermsOfServiceScreenActor* actor) OVERRIDE;

 private:
  
  void StartDownload();

  
  void OnDownloadTimeout();

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  TermsOfServiceScreenActor* actor_;

  scoped_ptr<net::URLFetcher> terms_of_service_fetcher_;

  
  
  base::OneShotTimer<TermsOfServiceScreen> download_timer_;

  DISALLOW_COPY_AND_ASSIGN(TermsOfServiceScreen);
};

}  

#endif  
