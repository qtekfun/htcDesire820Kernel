// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_BLOCKED_CONTENT_CONTAINER_H_
#define CHROME_BROWSER_BLOCKED_CONTENT_CONTAINER_H_
#pragma once

#include <vector>

#include "content/browser/tab_contents/tab_contents_delegate.h"

class BlockedContentContainer : public TabContentsDelegate {
 public:
  
  explicit BlockedContentContainer(TabContents* owner);
  virtual ~BlockedContentContainer();

  
  
  void AddTabContents(TabContents* tab_contents,
                      WindowOpenDisposition disposition,
                      const gfx::Rect& bounds,
                      bool user_gesture);

  
  void LaunchForContents(TabContents* tab_contents);

  
  size_t GetBlockedContentsCount() const;

  
  
  void GetBlockedContents(std::vector<TabContents*>* blocked_contents) const;

  
  void Destroy();

  

  
  virtual void OpenURLFromTab(TabContents* source,
                              const GURL& url, const GURL& referrer,
                              WindowOpenDisposition disposition,
                              PageTransition::Type transition);

  
  virtual void NavigationStateChanged(const TabContents* source,
                                      unsigned changed_flags) {}

  
  virtual void AddNewContents(TabContents* source,
                              TabContents* new_contents,
                              WindowOpenDisposition disposition,
                              const gfx::Rect& initial_position,
                              bool user_gesture);

  
  
  virtual void ActivateContents(TabContents* contents) {}
  virtual void DeactivateContents(TabContents* contents) {}

  
  virtual void LoadingStateChanged(TabContents* source) {}

  
  virtual void CloseContents(TabContents* source);

  
  virtual void MoveContents(TabContents* source, const gfx::Rect& new_bounds);

  
  virtual bool IsPopup(const TabContents* source) const;

  
  virtual TabContents* GetConstrainingContents(TabContents* source);

  
  virtual void UpdateTargetURL(TabContents* source, const GURL& url) {}

  
  
  
  static const size_t kImpossibleNumberOfPopups;

 private:
  struct BlockedContent;

  typedef std::vector<BlockedContent> BlockedContents;

  
  TabContents* owner_;

  
  BlockedContents blocked_contents_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(BlockedContentContainer);
};

#endif  
