// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INSTANT_INSTANT_CONTROLLER_H_
#define CHROME_BROWSER_INSTANT_INSTANT_CONTROLLER_H_
#pragma once

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/string16.h"
#include "base/task.h"
#include "base/timer.h"
#include "chrome/browser/instant/instant_commit_type.h"
#include "chrome/browser/instant/instant_loader_delegate.h"
#include "chrome/browser/search_engines/template_url_id.h"
#include "chrome/common/instant_types.h"
#include "content/common/page_transition_types.h"
#include "googleurl/src/gurl.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"

struct AutocompleteMatch;
class InstantDelegate;
class InstantLoader;
class InstantLoaderManager;
class InstantTest;
class PrefService;
class Profile;
class TabContents;
class TabContentsWrapper;
class TemplateURL;

class InstantController : public InstantLoaderDelegate {
 public:
  
  static const int kAutoCommitPauseTimeMS = 1000;
  
  static const int kAutoCommitFadeInTimeMS = 300;

  InstantController(Profile* profile, InstantDelegate* delegate);
  ~InstantController();

  
  static void RegisterUserPrefs(PrefService* prefs);

  
  static void RecordMetrics(Profile* profile);

  
  static bool IsEnabled(Profile* profile);

  
  static void Enable(Profile* profile);

  
  static void Disable(Profile* profile);

  
  
  static bool CommitIfCurrent(InstantController* controller);

  
  
  
  
  
  
  
  void Update(TabContentsWrapper* tab_contents,
              const AutocompleteMatch& match,
              const string16& user_text,
              bool verbatim,
              string16* suggested_text);

  
  
  
  void SetOmniboxBounds(const gfx::Rect& bounds);

  
  
  void DestroyPreviewContents();

  
  void DestroyPreviewContentsAndLeaveActive();

  
  
  
  
  bool IsCurrent();

  
  
  void CommitCurrentPreview(InstantCommitType type);

  
  
  void SetCommitOnMouseUp();

  bool commit_on_mouse_up() const { return commit_on_mouse_up_; }

  
  
  bool IsMouseDownFromActivate();

  
  
  void OnAutocompleteLostFocus(gfx::NativeView view_gaining_focus);

  
  
  
  
  
  TabContentsWrapper* ReleasePreviewContents(InstantCommitType type);

  
  
  void CompleteRelease(TabContents* tab);

  
  TabContentsWrapper* tab_contents() const { return tab_contents_; }

  
  TabContentsWrapper* GetPreviewContents();

  
  
  bool is_active() const { return is_active_; }

  
  
  bool is_displayable() const { return displayable_loader_ != NULL; }

  
  PageTransition::Type last_transition_type() const {
    return last_transition_type_;
  }

  
  
  
  bool IsShowingInstant();

  
  
  
  
  
  
  
  
  
  
  bool MightSupportInstant();

  
  
  GURL GetCurrentURL();

  
  virtual void InstantStatusChanged(InstantLoader* loader) OVERRIDE;
  virtual void SetSuggestedTextFor(InstantLoader* loader,
                                   const string16& text,
                                   InstantCompleteBehavior behavior) OVERRIDE;
  virtual gfx::Rect GetInstantBounds() OVERRIDE;
  virtual bool ShouldCommitInstantOnMouseUp() OVERRIDE;
  virtual void CommitInstantLoader(InstantLoader* loader) OVERRIDE;
  virtual void InstantLoaderDoesntSupportInstant(
      InstantLoader* loader) OVERRIDE;
  virtual void AddToBlacklist(InstantLoader* loader,
                              const GURL& url) OVERRIDE;

 private:
  friend class InstantTest;

  typedef std::set<std::string> HostBlacklist;

  
  void UpdateDisplayableLoader();

  
  
  TabContentsWrapper* GetPendingPreviewContents();

  
  bool ShouldUpdateNow(TemplateURLID instant_id, const GURL& url);

  
  void ScheduleUpdate(const GURL& url);

  
  void ProcessScheduledUpdate();

  
  
  void ProcessInstantStatusChanged(InstantLoader* loader);

  
  
  void ShowTimerFired();

  
  
  void UpdateLoader(const TemplateURL* template_url,
                    const GURL& url,
                    PageTransition::Type transition_type,
                    const string16& user_text,
                    bool verbatim,
                    string16* suggested_text);

  
  
  bool ShouldShowPreviewFor(const AutocompleteMatch& match,
                            const TemplateURL** template_url);

  
  void BlacklistFromInstant(TemplateURLID id);

  
  
  bool IsBlacklistedFromInstant(TemplateURLID id);

  
  void ClearBlacklist();

  
  
  
  
  
  void ScheduleDestroy(InstantLoader* loader);

  
  void DestroyLoaders();

  
  
  const TemplateURL* GetTemplateURL(const AutocompleteMatch& match);

  InstantDelegate* delegate_;

  
  TabContentsWrapper* tab_contents_;

  
  bool is_active_;

  
  InstantLoader* displayable_loader_;

  
  gfx::Rect omnibox_bounds_;

  
  bool commit_on_mouse_up_;

  
  PageTransition::Type last_transition_type_;

  scoped_ptr<InstantLoaderManager> loader_manager_;

  
  
  
  
  std::set<TemplateURLID> blacklisted_ids_;

  
  base::OneShotTimer<InstantController> update_timer_;

  
  base::OneShotTimer<InstantController> show_timer_;

  
  ScopedRunnableMethodFactory<InstantController> destroy_factory_;

  
  GURL scheduled_url_;

  
  ScopedVector<InstantLoader> loaders_to_destroy_;

  
  
  static HostBlacklist* host_blacklist_;

  DISALLOW_COPY_AND_ASSIGN(InstantController);
};

#endif  
