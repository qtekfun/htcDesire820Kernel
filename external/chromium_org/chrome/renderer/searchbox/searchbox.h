// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_SEARCHBOX_SEARCHBOX_H_
#define CHROME_RENDERER_SEARCHBOX_SEARCHBOX_H_

#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "chrome/common/instant_restricted_id_cache.h"
#include "chrome/common/instant_types.h"
#include "chrome/common/ntp_logging_events.h"
#include "chrome/common/omnibox_focus_state.h"
#include "content/public/renderer/render_view_observer.h"
#include "content/public/renderer/render_view_observer_tracker.h"
#include "ui/base/window_open_disposition.h"
#include "url/gurl.h"

namespace content {
class RenderView;
}

class SearchBox : public content::RenderViewObserver,
                  public content::RenderViewObserverTracker<SearchBox> {
 public:
  explicit SearchBox(content::RenderView* render_view);
  virtual ~SearchBox();

  
  void LogEvent(NTPLoggingEventType event);

  
  void LogImpression(int position, const base::string16& provider);

  
  void CheckIsUserSignedInToChromeAs(const base::string16& identity);

  
  void DeleteMostVisitedItem(InstantRestrictedID most_visited_item_id);

  
  
  
  
  
  
  
  
  
  bool GenerateFaviconURLFromTransientURL(const GURL& transient_url,
                                          GURL* url) const;

  
  
  
  
  
  
  
  bool GenerateThumbnailURLFromTransientURL(const GURL& transient_url,
                                            GURL* url) const;

  
  void GetMostVisitedItems(
      std::vector<InstantMostVisitedItemIDPair>* items) const;

  
  
  bool GetMostVisitedItemWithID(InstantRestrictedID most_visited_item_id,
                                InstantMostVisitedItem* item) const;

  
  void Focus();

  
  void NavigateToURL(const GURL& url,
                     WindowOpenDisposition disposition,
                     bool is_most_visited_item_url);

  
  void Paste(const base::string16& text);

  const ThemeBackgroundInfo& GetThemeBackgroundInfo();

  
  void SetVoiceSearchSupported(bool supported);

  
  void StartCapturingKeyStrokes();

  
  void StopCapturingKeyStrokes();

  
  
  void UndoAllMostVisitedDeletions();

  
  void UndoMostVisitedDeletion(InstantRestrictedID most_visited_item_id);

  bool app_launcher_enabled() const { return app_launcher_enabled_; }
  bool is_focused() const { return is_focused_; }
  bool is_input_in_progress() const { return is_input_in_progress_; }
  bool is_key_capture_enabled() const { return is_key_capture_enabled_; }
  bool display_instant_results() const { return display_instant_results_; }
  const base::string16& query() const { return query_; }
  int start_margin() const { return start_margin_; }
  const InstantSuggestion& suggestion() const { return suggestion_; }

 private:
  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  void OnChromeIdentityCheckResult(const base::string16& identity,
                                   bool identity_match);
  void OnDetermineIfPageSupportsInstant();
  void OnFocusChanged(OmniboxFocusState new_focus_state,
                      OmniboxFocusChangeReason reason);
  void OnMarginChange(int margin, int width);
  void OnMostVisitedChanged(
      const std::vector<InstantMostVisitedItem>& items);
  void OnPromoInformationReceived(bool is_app_launcher_enabled);
  void OnSetDisplayInstantResults(bool display_instant_results);
  void OnSetInputInProgress(bool input_in_progress);
  void OnSetSuggestionToPrefetch(const InstantSuggestion& suggestion);
  void OnSubmit(const base::string16& query);
  void OnThemeChanged(const ThemeBackgroundInfo& theme_info);
  void OnToggleVoiceSearch();

  
  double GetZoom() const;

  
  void Reset();

  
  GURL GetURLForMostVisitedItem(InstantRestrictedID item_id) const;

  bool app_launcher_enabled_;
  bool is_focused_;
  bool is_input_in_progress_;
  bool is_key_capture_enabled_;
  bool display_instant_results_;
  InstantRestrictedIDCache<InstantMostVisitedItem> most_visited_items_cache_;
  ThemeBackgroundInfo theme_info_;
  base::string16 query_;
  int start_margin_;
  InstantSuggestion suggestion_;
  int width_;

  DISALLOW_COPY_AND_ASSIGN(SearchBox);
};

#endif  
