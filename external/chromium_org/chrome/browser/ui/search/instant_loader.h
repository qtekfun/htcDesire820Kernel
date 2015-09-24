// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SEARCH_INSTANT_LOADER_H_
#define CHROME_BROWSER_UI_SEARCH_INSTANT_LOADER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer/timer.h"
#include "chrome/browser/ui/tab_contents/core_tab_helper_delegate.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_delegate.h"

class GURL;
class Profile;

namespace content {
struct OpenURLParams;
class WebContents;
}

class InstantLoader : public content::NotificationObserver,
                      public content::WebContentsDelegate,
                      public CoreTabHelperDelegate {
 public:
  
  
  class Delegate {
   public:
    
    virtual void OnSwappedContents() = 0;

    
    
    
    
    virtual content::WebContents* OpenURLFromTab(
        content::WebContents* source,
        const content::OpenURLParams& params) = 0;

    
    virtual void LoadCompletedMainFrame() = 0;

   protected:
    ~Delegate();
  };

  explicit InstantLoader(Delegate* delegate);
  virtual ~InstantLoader();

  
  
  
  
  void Init(const GURL& instant_url,
            Profile* profile,
            const base::Closure& on_stale_callback);

  
  void Load();

  
  content::WebContents* contents() const { return contents_.get(); }

  
  
  void SetContents(scoped_ptr<content::WebContents> contents);

  
  
  scoped_ptr<content::WebContents> ReleaseContents();

 private:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void SwapTabContents(content::WebContents* old_contents,
                               content::WebContents* new_contents) OVERRIDE;

  
  virtual bool ShouldSuppressDialogs() OVERRIDE;
  virtual bool ShouldFocusPageAfterCrash() OVERRIDE;
  virtual void CanDownload(content::RenderViewHost* render_view_host,
                           int request_id,
                           const std::string& request_method,
                           const base::Callback<void(bool)>& callback) OVERRIDE;
  virtual bool OnGoToEntryOffset(int offset) OVERRIDE;
  virtual content::WebContents* OpenURLFromTab(
      content::WebContents* source,
      const content::OpenURLParams& params) OVERRIDE;

  Delegate* const delegate_;
  scoped_ptr<content::WebContents> contents_;

  
  GURL instant_url_;

  
  base::Closure on_stale_callback_;

  
  base::Timer stale_page_timer_;

  
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(InstantLoader);
};

#endif  
