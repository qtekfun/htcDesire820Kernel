// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SEARCH_SEARCH_IPC_ROUTER_H_
#define CHROME_BROWSER_UI_SEARCH_SEARCH_IPC_ROUTER_H_

#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/common/instant_types.h"
#include "chrome/common/ntp_logging_events.h"
#include "chrome/common/omnibox_focus_state.h"
#include "content/public/browser/web_contents_observer.h"
#include "ui/base/window_open_disposition.h"

class GURL;

namespace content {
class WebContents;
}

class SearchIPCRouterTest;

class SearchIPCRouter : public content::WebContentsObserver {
 public:
  
  
  class Delegate {
   public:
    
    
    virtual void OnInstantSupportDetermined(bool supports_instant) = 0;

    
    virtual void OnSetVoiceSearchSupport(bool supports_voice_search) = 0;

    
    
    virtual void FocusOmnibox(OmniboxFocusState state) = 0;

    
    
    
    
    virtual void NavigateToURL(const GURL& url,
                               WindowOpenDisposition disposition,
                               bool is_most_visited_item_url) = 0;

    
    virtual void OnDeleteMostVisitedItem(const GURL& url) = 0;

    
    virtual void OnUndoMostVisitedDeletion(const GURL& url) = 0;

    
    virtual void OnUndoAllMostVisitedDeletions() = 0;

    
    virtual void OnLogEvent(NTPLoggingEventType event) = 0;

    
    virtual void OnLogImpression(int position,
                                 const base::string16& provider) = 0;

    
    
    virtual void PasteIntoOmnibox(const base::string16& text) = 0;

    
    
    
    virtual void OnChromeIdentityCheck(const base::string16& identity) = 0;
  };

  
  
  
  class Policy {
   public:
    virtual ~Policy() {}

    
    
    virtual bool ShouldProcessSetVoiceSearchSupport() = 0;
    virtual bool ShouldProcessFocusOmnibox(bool is_active_tab) = 0;
    virtual bool ShouldProcessNavigateToURL(bool is_active_tab) = 0;
    virtual bool ShouldProcessDeleteMostVisitedItem() = 0;
    virtual bool ShouldProcessUndoMostVisitedDeletion() = 0;
    virtual bool ShouldProcessUndoAllMostVisitedDeletions() = 0;
    virtual bool ShouldProcessLogEvent() = 0;
    virtual bool ShouldProcessPasteIntoOmnibox(bool is_active_tab) = 0;
    virtual bool ShouldProcessChromeIdentityCheck() = 0;
    virtual bool ShouldSendSetPromoInformation() = 0;
    virtual bool ShouldSendSetDisplayInstantResults() = 0;
    virtual bool ShouldSendSetSuggestionToPrefetch() = 0;
    virtual bool ShouldSendMostVisitedItems() = 0;
    virtual bool ShouldSendThemeBackgroundInfo() = 0;
    virtual bool ShouldSendToggleVoiceSearch() = 0;
    virtual bool ShouldSubmitQuery() = 0;
  };

  SearchIPCRouter(content::WebContents* web_contents, Delegate* delegate,
                  scoped_ptr<Policy> policy);
  virtual ~SearchIPCRouter();

  
  
  
  void DetermineIfPageSupportsInstant();

  
  void SendChromeIdentityCheckResult(const base::string16& identity,
                                     bool identity_match);

  
  void SetPromoInformation(bool is_app_launcher_enabled);

  
  void SetDisplayInstantResults();

  
  void SetSuggestionToPrefetch(const InstantSuggestion& suggestion);

  
  void SendMostVisitedItems(const std::vector<InstantMostVisitedItem>& items);

  
  void SendThemeBackgroundInfo(const ThemeBackgroundInfo& theme_info);

  
  void ToggleVoiceSearch();

  
  void Submit(const base::string16& text);

  
  void OnTabActivated();

  
  void OnTabDeactivated();

 private:
  friend class SearchIPCRouterPolicyTest;
  friend class SearchIPCRouterTest;
  FRIEND_TEST_ALL_PREFIXES(SearchTabHelperTest,
                           DetermineIfPageSupportsInstant_Local);
  FRIEND_TEST_ALL_PREFIXES(SearchTabHelperTest,
                           DetermineIfPageSupportsInstant_NonLocal);
  FRIEND_TEST_ALL_PREFIXES(SearchTabHelperTest,
                           PageURLDoesntBelongToInstantRenderer);
  FRIEND_TEST_ALL_PREFIXES(SearchIPCRouterTest,
                           IgnoreMessageIfThePageIsNotActive);
  FRIEND_TEST_ALL_PREFIXES(SearchIPCRouterTest,
                           DoNotSendSetDisplayInstantResultsMsg);
  FRIEND_TEST_ALL_PREFIXES(SearchIPCRouterTest, HandleTabChangedEvents);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  void OnInstantSupportDetermined(int page_id, bool supports_instant) const;
  void OnVoiceSearchSupportDetermined(int page_id,
                                      bool supports_voice_search) const;
  void OnFocusOmnibox(int page_id, OmniboxFocusState state) const;
  void OnSearchBoxNavigate(int page_id,
                           const GURL& url,
                           WindowOpenDisposition disposition,
                           bool is_most_visited_item_url) const;
  void OnDeleteMostVisitedItem(int page_id, const GURL& url) const;
  void OnUndoMostVisitedDeletion(int page_id, const GURL& url) const;
  void OnUndoAllMostVisitedDeletions(int page_id) const;
  void OnLogEvent(int page_id, NTPLoggingEventType event) const;
  void OnLogImpression(int page_id,
                       int position,
                       const base::string16& provider) const;
  void OnPasteAndOpenDropDown(int page_id, const base::string16& text) const;
  void OnChromeIdentityCheck(int page_id, const base::string16& identity) const;

  
  void set_delegate(Delegate* delegate);

  
  void set_policy(scoped_ptr<Policy> policy);

  
  Policy* policy() const { return policy_.get(); }

  Delegate* delegate_;
  scoped_ptr<Policy> policy_;

  
  bool is_active_tab_;

  DISALLOW_COPY_AND_ASSIGN(SearchIPCRouter);
};

#endif  
