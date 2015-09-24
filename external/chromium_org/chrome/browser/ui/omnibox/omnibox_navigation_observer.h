// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_OMNIBOX_OMNIBOX_NAVIGATION_OBSERVER_H_
#define CHROME_BROWSER_UI_OMNIBOX_OMNIBOX_NAVIGATION_OBSERVER_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_observer.h"
#include "net/url_request/url_fetcher_delegate.h"

namespace history {
class ShortcutsBackend;
}

namespace net {
class URLFetcher;
class URLRequestStatus;
}

class OmniboxNavigationObserver : public content::NotificationObserver,
                                  public content::WebContentsObserver,
                                  public net::URLFetcherDelegate {
 public:
  enum LoadState {
    LOAD_NOT_SEEN,
    LOAD_PENDING,
    LOAD_COMMITTED,
  };

  OmniboxNavigationObserver(Profile* profile,
                            const base::string16& text,
                            const AutocompleteMatch& match,
                            const AutocompleteMatch& alternate_nav_match);
  virtual ~OmniboxNavigationObserver();

  LoadState load_state() const { return load_state_; }

  
  
  
  void OnSuccessfulNavigation();

 private:
  enum FetchState {
    FETCH_NOT_COMPLETE,
    FETCH_SUCCEEDED,
    FETCH_FAILED,
  };

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void DidStartNavigationToPendingEntry(
      const GURL& url,
      content::NavigationController::ReloadType reload_type) OVERRIDE;
  virtual void NavigationEntryCommitted(
      const content::LoadCommittedDetails& load_details) OVERRIDE;
  virtual void WebContentsDestroyed(
      content::WebContents* web_contents) OVERRIDE;

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  
  void OnAllLoadingFinished();

  const base::string16 text_;
  const AutocompleteMatch match_;
  const AutocompleteMatch alternate_nav_match_;
  scoped_refptr<history::ShortcutsBackend> shortcuts_backend_;  
                                                                
  scoped_ptr<net::URLFetcher> fetcher_;
  LoadState load_state_;
  FetchState fetch_state_;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(OmniboxNavigationObserver);
};

#endif  
