// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AEROPEEK_MANAGER_H_
#define CHROME_BROWSER_AEROPEEK_MANAGER_H_
#pragma once

#include <windows.h>

#include <list>

#include "chrome/browser/tabs/tab_strip_model_observer.h"
#include "ui/gfx/insets.h"

namespace gfx {
class Size;
}
class AeroPeekWindow;
class SkBitmap;
class TabContents;

class AeroPeekWindowDelegate {
 public:
  virtual void ActivateTab(int tab_id) = 0;
  virtual void CloseTab(int tab_id) = 0;
  virtual void GetContentInsets(gfx::Insets* insets) = 0;
  virtual bool GetTabThumbnail(int tab_id, SkBitmap* thumbnail) = 0;
  virtual bool GetTabPreview(int tab_id, SkBitmap* preview) = 0;

 protected:
  virtual ~AeroPeekWindowDelegate() {}
};

class AeroPeekManager : public TabStripModelObserver,
                        public AeroPeekWindowDelegate {
 public:
  explicit AeroPeekManager(HWND application_window);
  virtual ~AeroPeekManager();

  
  
  void SetContentInsets(const gfx::Insets& insets);

  
  
  static bool Enabled();

  
  virtual void TabInsertedAt(TabContentsWrapper* contents,
                             int index,
                             bool foreground);
  virtual void TabDetachedAt(TabContentsWrapper* contents, int index);
  virtual void TabSelectedAt(TabContentsWrapper* old_contents,
                             TabContentsWrapper* new_contents,
                             int index,
                             bool user_gesture);
  virtual void TabMoved(TabContentsWrapper* contents,
                        int from_index,
                        int to_index,
                        bool pinned_state_changed);
  virtual void TabChangedAt(TabContentsWrapper* contents,
                            int index,
                            TabChangeType change_type);
  virtual void TabReplacedAt(TabStripModel* tab_strip_model,
                             TabContentsWrapper* old_contents,
                             TabContentsWrapper* new_contents,
                             int index);

  
  virtual void CloseTab(int tab_id);
  virtual void ActivateTab(int tab_id);
  virtual void GetContentInsets(gfx::Insets* insets);
  virtual bool GetTabThumbnail(int tab_id, SkBitmap* thumbnail);
  virtual bool GetTabPreview(int tab_id, SkBitmap* preview);

 private:
  
  
  void DeleteAeroPeekWindow(int tab_id);

  
  
  void DeleteAeroPeekWindowForTab(TabContentsWrapper* tab);

  
  
  AeroPeekWindow* GetAeroPeekWindow(int tab_id) const;

  
  
  void CreateAeroPeekWindowIfNecessary(TabContentsWrapper* tab,
                                       bool foreground);

  
  
  
  void GetOutputBitmapSize(const gfx::Size& destination,
                           const gfx::Size& source,
                           gfx::Size* output) const;

  
  
  
  
  
  
  
  TabContents* GetTabContents(int tab_id) const;

  
  int GetTabID(TabContents* contents) const;

 private:
  
  
  HWND application_window_;

  
  std::list<AeroPeekWindow*> tab_list_;

  
  
  
  int border_left_;
  int border_top_;

  
  
  
  int toolbar_top_;

  
  
  
  gfx::Insets content_insets_;
};

#endif  
