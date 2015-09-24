// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INSTANT_INSTANT_LOADER_H_
#define CHROME_BROWSER_INSTANT_INSTANT_LOADER_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "base/timer.h"
#include "chrome/browser/instant/instant_commit_type.h"
#include "chrome/browser/search_engines/template_url_id.h"
#include "chrome/common/instant_types.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "content/common/page_transition_types.h"
#include "googleurl/src/gurl.h"
#include "ui/gfx/rect.h"

class InstantLoaderDelegate;
class InstantLoaderManagerTest;
class TabContents;
class TabContentsWrapper;
class TemplateURL;

class InstantLoader : public NotificationObserver {
 public:
  InstantLoader(InstantLoaderDelegate* delegate, TemplateURLID id);
  virtual ~InstantLoader();

  
  
  
  bool Update(TabContentsWrapper* tab_contents,
              const TemplateURL* template_url,
              const GURL& url,
              PageTransition::Type transition_type,
              const string16& user_text,
              bool verbatim,
              string16* suggested_text);

  
  
  
  void SetOmniboxBounds(const gfx::Rect& bounds);

  
  
  bool IsMouseDownFromActivate();

  
  
  
  TabContentsWrapper* ReleasePreviewContents(InstantCommitType type);

  
  bool ShouldCommitInstantOnMouseUp();
  void CommitInstantLoader();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  
  TabContentsWrapper* preview_contents() const {
    return preview_contents_.get();
  }

  
  bool ready() const { return ready_; }

  
  bool http_status_ok() const { return http_status_ok_; }

  
  
  bool needs_reload() const { return needs_reload_; }

  const GURL& url() const { return url_; }

  bool verbatim() const { return verbatim_; }

  
  bool is_showing_instant() const { return template_url_id_ != 0; }

  
  TemplateURLID template_url_id() const { return template_url_id_; }

  
  const string16& user_text() const { return user_text_; }

 private:
  friend class InstantLoaderManagerTest;
  friend class InstantTest;
  class FrameLoadObserver;
  class PaintObserverImpl;
  class TabContentsDelegateImpl;

  
  
  
  void SetCompleteSuggestedText(const string16& suggested_text,
                                InstantCompleteBehavior behavior);

  
  void PreviewPainted();

  
  void SetHTTPStatusOK(bool is_ok);

  
  
  
  void ShowPreview();

  
  void PageFinishedLoading();

  
  gfx::Rect GetOmniboxBoundsInTermsOfPreview();

  
  bool is_waiting_for_load() const {
    return frame_load_observer_.get() != NULL;
  }

  
  
  
  void PageDoesntSupportInstant(bool needs_reload);

  
  void ProcessBoundsChange();

  
  
  
  void SendBoundsToPage(bool force_if_loading);

  
  void CreatePreviewContents(TabContentsWrapper* tab_contents);

  InstantLoaderDelegate* delegate_;

  
  
  const TemplateURLID template_url_id_;

  
  GURL url_;

  
  
  scoped_ptr<TabContentsDelegateImpl> preview_tab_contents_delegate_;

  
  scoped_ptr<TabContentsWrapper> preview_contents_;

  
  bool ready_;

  
  bool http_status_ok_;

  
  string16 user_text_;

  
  string16 complete_suggested_text_;

  
  string16 last_suggestion_;

  
  gfx::Rect omnibox_bounds_;

  
  gfx::Rect last_omnibox_bounds_;

  scoped_ptr<FrameLoadObserver> frame_load_observer_;

  
  PageTransition::Type last_transition_type_;

  
  base::OneShotTimer<InstantLoader> update_bounds_timer_;

  
  NotificationRegistrar registrar_;

  
  bool verbatim_;

  
  bool needs_reload_;

  DISALLOW_COPY_AND_ASSIGN(InstantLoader);
};

#endif  
