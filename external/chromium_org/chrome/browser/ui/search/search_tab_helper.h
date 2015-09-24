// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SEARCH_SEARCH_TAB_HELPER_H_
#define CHROME_BROWSER_UI_SEARCH_SEARCH_TAB_HELPER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "chrome/browser/search/instant_service_observer.h"
#include "chrome/browser/ui/search/search_ipc_router.h"
#include "chrome/browser/ui/search/search_model.h"
#include "chrome/common/instant_types.h"
#include "chrome/common/ntp_logging_events.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "ui/base/window_open_disposition.h"

namespace content {
class WebContents;
struct LoadCommittedDetails;
}

class GURL;
class InstantPageTest;
class InstantService;
class Profile;
class SearchIPCRouterTest;

class SearchTabHelper : public content::WebContentsObserver,
                        public content::WebContentsUserData<SearchTabHelper>,
                        public InstantServiceObserver,
                        public SearchIPCRouter::Delegate {
 public:
  virtual ~SearchTabHelper();

  SearchModel* model() {
    return &model_;
  }

  
  void InitForPreloadedNTP();

  
  
  void OmniboxEditModelChanged(bool user_input_in_progress, bool cancelling);

  
  
  
  
  void NavigationEntryUpdated();

  
  void InstantSupportChanged(bool supports_instant);

  
  
  bool SupportsInstant() const;

  
  void SetSuggestionToPrefetch(const InstantSuggestion& suggestion);

  
  void Submit(const base::string16& text);

  
  void OnTabActivated();

  
  void OnTabDeactivated();

  
  void ToggleVoiceSearch();

  
  bool IsSearchResultsPage();

 private:
  friend class content::WebContentsUserData<SearchTabHelper>;
  friend class InstantPageTest;
  friend class SearchIPCRouterPolicyTest;
  friend class SearchIPCRouterTest;
  FRIEND_TEST_ALL_PREFIXES(SearchTabHelperTest,
                           DetermineIfPageSupportsInstant_Local);
  FRIEND_TEST_ALL_PREFIXES(SearchTabHelperTest,
                           DetermineIfPageSupportsInstant_NonLocal);
  FRIEND_TEST_ALL_PREFIXES(SearchTabHelperTest,
                           PageURLDoesntBelongToInstantRenderer);
  FRIEND_TEST_ALL_PREFIXES(SearchTabHelperTest,
                           OnChromeIdentityCheckMatch);
  FRIEND_TEST_ALL_PREFIXES(SearchTabHelperTest,
                           OnChromeIdentityCheckMismatch);
  FRIEND_TEST_ALL_PREFIXES(SearchTabHelperTest,
                           OnChromeIdentityCheckSignedOutMatch);
  FRIEND_TEST_ALL_PREFIXES(SearchTabHelperTest,
                           OnChromeIdentityCheckSignedOutMismatch);
  FRIEND_TEST_ALL_PREFIXES(SearchTabHelperWindowTest,
                           OnProvisionalLoadFailRedirectNTPToLocal);
  FRIEND_TEST_ALL_PREFIXES(SearchTabHelperWindowTest,
                           OnProvisionalLoadFailDontRedirectIfAborted);
  FRIEND_TEST_ALL_PREFIXES(SearchTabHelperWindowTest,
                           OnProvisionalLoadFailDontRedirectNonNTP);
  FRIEND_TEST_ALL_PREFIXES(SearchIPCRouterTest,
                           IgnoreMessageIfThePageIsNotActive);
  FRIEND_TEST_ALL_PREFIXES(SearchIPCRouterTest,
                           DoNotSendSetDisplayInstantResultsMsg);
  FRIEND_TEST_ALL_PREFIXES(SearchIPCRouterTest, HandleTabChangedEvents);
  FRIEND_TEST_ALL_PREFIXES(InstantPageTest,
                           DetermineIfPageSupportsInstant_Local);
  FRIEND_TEST_ALL_PREFIXES(InstantPageTest,
                           DetermineIfPageSupportsInstant_NonLocal);
  FRIEND_TEST_ALL_PREFIXES(InstantPageTest,
                           PageURLDoesntBelongToInstantRenderer);
  FRIEND_TEST_ALL_PREFIXES(InstantPageTest, PageSupportsInstant);

  explicit SearchTabHelper(content::WebContents* web_contents);

  
  virtual void RenderViewCreated(
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidStartNavigationToPendingEntry(
      const GURL& url,
      content::NavigationController::ReloadType reload_type) OVERRIDE;
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;
  virtual void DidFailProvisionalLoad(
      int64 frame_id,
      const base::string16& frame_unique_name,
      bool is_main_frame,
      const GURL& validated_url,
      int error_code,
      const base::string16& error_description,
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidFinishLoad(
      int64 frame_id,
      const GURL& validated_url,
      bool is_main_frame,
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void NavigationEntryCommitted(
      const content::LoadCommittedDetails& load_details) OVERRIDE;

  
  virtual void OnInstantSupportDetermined(bool supports_instant) OVERRIDE;
  virtual void OnSetVoiceSearchSupport(bool supports_voice_search) OVERRIDE;
  virtual void FocusOmnibox(OmniboxFocusState state) OVERRIDE;
  virtual void NavigateToURL(const GURL& url,
                             WindowOpenDisposition disposition,
                             bool is_most_visited_item_url) OVERRIDE;
  virtual void OnDeleteMostVisitedItem(const GURL& url) OVERRIDE;
  virtual void OnUndoMostVisitedDeletion(const GURL& url) OVERRIDE;
  virtual void OnUndoAllMostVisitedDeletions() OVERRIDE;
  virtual void OnLogEvent(NTPLoggingEventType event) OVERRIDE;
  virtual void OnLogImpression(int position,
                               const base::string16& provider) OVERRIDE;
  virtual void PasteIntoOmnibox(const base::string16& text) OVERRIDE;
  virtual void OnChromeIdentityCheck(const base::string16& identity) OVERRIDE;

  
  virtual void ThemeInfoChanged(const ThemeBackgroundInfo& theme_info) OVERRIDE;
  virtual void MostVisitedItemsChanged(
      const std::vector<InstantMostVisitedItem>& items) OVERRIDE;

  
  
  
  void MaybeRemoveMostVisitedItems(std::vector<InstantMostVisitedItem>* items);

  
  
  
  
  
  void UpdateMode(bool update_origin, bool is_preloaded_ntp);

  
  
  
  void DetermineIfPageSupportsInstant();

  
  SearchIPCRouter& ipc_router() { return ipc_router_; }

  Profile* profile() const;

  
  
  void RedirectToLocalNTP();

  const bool is_search_enabled_;

  
  bool user_input_in_progress_;

  
  SearchModel model_;

  content::WebContents* web_contents_;

  SearchIPCRouter ipc_router_;

  InstantService* instant_service_;

  DISALLOW_COPY_AND_ASSIGN(SearchTabHelper);
};

#endif  
