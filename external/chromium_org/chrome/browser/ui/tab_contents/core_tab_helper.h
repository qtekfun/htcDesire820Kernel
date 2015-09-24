// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TAB_CONTENTS_CORE_TAB_HELPER_H_
#define CHROME_BROWSER_UI_TAB_CONTENTS_CORE_TAB_HELPER_H_

#include "base/time/time.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

class CoreTabHelperDelegate;

class CoreTabHelper : public content::WebContentsObserver,
                      public content::WebContentsUserData<CoreTabHelper> {
 public:
  virtual ~CoreTabHelper();

  
  static base::string16 GetDefaultTitle();

  
  base::string16 GetStatusText() const;

  
  
  void OnCloseStarted();

  
  
  void OnCloseCanceled();

  
  
  
  void OnUnloadStarted();

  
  void OnUnloadDetachedStarted();

  void UpdateContentRestrictions(int content_restrictions);

  CoreTabHelperDelegate* delegate() const { return delegate_; }
  void set_delegate(CoreTabHelperDelegate* d) { delegate_ = d; }

  void set_new_tab_start_time(const base::TimeTicks& time) {
    new_tab_start_time_ = time;
  }

  base::TimeTicks new_tab_start_time() const { return new_tab_start_time_; }
  int content_restrictions() const { return content_restrictions_; }

 private:
  explicit CoreTabHelper(content::WebContents* web_contents);
  friend class content::WebContentsUserData<CoreTabHelper>;

  
  virtual void DidStartLoading(
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void WasShown() OVERRIDE;
  virtual void WebContentsDestroyed(
      content::WebContents* web_contents) OVERRIDE;
  virtual void BeforeUnloadFired(const base::TimeTicks& proceed_time) OVERRIDE;
  virtual void BeforeUnloadDialogCancelled() OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  void OnRequestThumbnailForContextNodeACK(const SkBitmap& bitmap,
                                           const gfx::Size& original_size);

  
  CoreTabHelperDelegate* delegate_;

  
  
  base::TimeTicks new_tab_start_time_;

  
  base::TimeTicks close_start_time_;

  
  base::TimeTicks before_unload_end_time_;

  
  base::TimeTicks unload_detached_start_time_;

  
  
  int content_restrictions_;

  DISALLOW_COPY_AND_ASSIGN(CoreTabHelper);
};

#endif  
