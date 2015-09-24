// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BOOKMARKS_BOOKMARK_BUBBLE_SIGN_IN_DELEGATE_H_
#define CHROME_BROWSER_UI_BOOKMARKS_BOOKMARK_BUBBLE_SIGN_IN_DELEGATE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/ui/bookmarks/bookmark_bubble_delegate.h"
#include "chrome/browser/ui/browser_list_observer.h"
#include "chrome/browser/ui/host_desktop.h"

class Browser;
class Profile;

class BookmarkBubbleSignInDelegate : public BookmarkBubbleDelegate,
                                     public chrome::BrowserListObserver {
 public:
  explicit BookmarkBubbleSignInDelegate(Browser* browser);

 private:
  virtual ~BookmarkBubbleSignInDelegate();

  
  virtual void OnSignInLinkClicked() OVERRIDE;

  
  virtual void OnBrowserRemoved(Browser* browser) OVERRIDE;

  
  void EnsureBrowser();

  
  Browser* browser_;

  
  Profile* profile_;

  
  chrome::HostDesktopType desktop_type_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkBubbleSignInDelegate);
};

#endif  
