// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIDEBAR_SIDEBAR_CONTAINER_H_
#define CHROME_BROWSER_SIDEBAR_SIDEBAR_CONTAINER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "chrome/browser/extensions/image_loading_tracker.h"
#include "content/browser/tab_contents/tab_contents_delegate.h"

class BrowserWindow;
class Profile;
class RenderViewHost;
class SkBitmap;
class TabContents;

class SidebarContainer
    : public TabContentsDelegate,
      private ImageLoadingTracker::Observer {
 public:
  
  class Delegate {
   public:
    Delegate() {}
    virtual ~Delegate() {}
    virtual void UpdateSidebar(SidebarContainer* host) = 0;
   private:
    DISALLOW_COPY_AND_ASSIGN(Delegate);
  };

  SidebarContainer(TabContents* tab,
                   const std::string& content_id,
                   Delegate* delegate);
  virtual ~SidebarContainer();

  
  
  void SidebarClosing();

  
  void LoadDefaults();

  
  const std::string& content_id() const { return content_id_; }

  
  TabContents* tab_contents() const { return tab_; }

  
  TabContents* sidebar_contents() const { return sidebar_contents_.get(); }

  
  const string16& badge_text() const { return badge_text_; }

  
  const SkBitmap& icon() const { return *icon_; }

  
  const string16& title() const { return title_; }

  

  
  void Show();

  
  void Expand();

  
  void Collapse();

  
  void Navigate(const GURL& url);

  
  void SetBadgeText(const string16& badge_text);

  
  void SetIcon(const SkBitmap& bitmap);

  
  void SetTitle(const string16& title);

 private:
  
  virtual void OpenURLFromTab(TabContents* source,
                              const GURL& url,
                              const GURL& referrer,
                              WindowOpenDisposition disposition,
                              PageTransition::Type transition) {}
  virtual void NavigationStateChanged(const TabContents* source,
                                      unsigned changed_flags) {}
  virtual void AddNewContents(TabContents* source,
                              TabContents* new_contents,
                              WindowOpenDisposition disposition,
                              const gfx::Rect& initial_pos,
                              bool user_gesture) {}
  virtual void ActivateContents(TabContents* contents) {}
  virtual void DeactivateContents(TabContents* contents) {}
  virtual void LoadingStateChanged(TabContents* source) {}
  virtual void CloseContents(TabContents* source) {}
  virtual void MoveContents(TabContents* source, const gfx::Rect& pos) {}
  virtual bool IsPopup(const TabContents* source) const;
  virtual void UpdateTargetURL(TabContents* source, const GURL& url) {}

  
  virtual void OnImageLoaded(SkBitmap* image,
                             const ExtensionResource& resource,
                             int index);

  
  const Extension* GetExtension() const;

  
  TabContents* tab_;

  
  
  const std::string content_id_;

  
  Delegate* delegate_;

  
  scoped_ptr<TabContents> sidebar_contents_;

  
  string16 badge_text_;

  
  scoped_ptr<SkBitmap> icon_;

  
  string16 title_;

  
  
  
  
  bool navigate_to_default_page_on_expand_;
  
  
  
  
  bool use_default_icon_;

  
  scoped_ptr<ImageLoadingTracker> image_loading_tracker_;

  DISALLOW_COPY_AND_ASSIGN(SidebarContainer);
};

#endif  
