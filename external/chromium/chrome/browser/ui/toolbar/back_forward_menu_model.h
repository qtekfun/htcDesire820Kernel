// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TOOLBAR_BACK_FORWARD_MENU_MODEL_H_
#define CHROME_BROWSER_UI_TOOLBAR_BACK_FORWARD_MENU_MODEL_H_
#pragma once

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/string16.h"
#include "chrome/browser/favicon_service.h"
#include "ui/base/models/menu_model.h"
#include "webkit/glue/window_open_disposition.h"

class Browser;
class SkBitmap;
class TabContents;
class NavigationEntry;

class BackForwardMenuModel : public ui::MenuModel {
 public:
  
  
  enum ModelType {
    FORWARD_MENU = 1,
    BACKWARD_MENU = 2
  };

  BackForwardMenuModel(Browser* browser, ModelType model_type);
  virtual ~BackForwardMenuModel();

  
  virtual bool HasIcons() const;
  
  
  
  
  virtual int GetItemCount() const;
  virtual ItemType GetTypeAt(int index) const;
  virtual int GetCommandIdAt(int index) const;
  virtual string16 GetLabelAt(int index) const;
  virtual bool IsItemDynamicAt(int index) const;
  virtual bool GetAcceleratorAt(int index,
                                ui::Accelerator* accelerator) const;
  virtual bool IsItemCheckedAt(int index) const;
  virtual int GetGroupIdAt(int index) const;
  virtual bool GetIconAt(int index, SkBitmap* icon);
  virtual ui::ButtonMenuItemModel* GetButtonMenuItemAt(int index) const;
  virtual bool IsEnabledAt(int index) const;
  virtual MenuModel* GetSubmenuModelAt(int index) const;
  virtual void HighlightChangedTo(int index);
  virtual void ActivatedAt(int index);
  virtual void ActivatedAtWithDisposition(int index, int disposition);
  virtual void MenuWillShow();

  
  bool IsSeparator(int index) const;

  
  virtual void SetMenuModelDelegate(ui::MenuModelDelegate* menu_model_delegate);

 protected:
   ui::MenuModelDelegate* menu_model_delegate() { return menu_model_delegate_; }

 private:
  friend class BackFwdMenuModelTest;
  FRIEND_TEST_ALL_PREFIXES(BackFwdMenuModelTest, BasicCase);
  FRIEND_TEST_ALL_PREFIXES(BackFwdMenuModelTest, MaxItemsTest);
  FRIEND_TEST_ALL_PREFIXES(BackFwdMenuModelTest, ChapterStops);
  FRIEND_TEST_ALL_PREFIXES(BackFwdMenuModelTest, EscapeLabel);
  FRIEND_TEST_ALL_PREFIXES(BackFwdMenuModelTest, FaviconLoadTest);

  
  
  void FetchFavicon(NavigationEntry* entry);

  
  void OnFavIconDataAvailable(FaviconService::Handle handle,
                              history::FaviconData favicon);

  
  void set_test_tab_contents(TabContents* test_tab_contents) {
    test_tab_contents_ = test_tab_contents;
  }

  
  
  
  
  
  
  
  
  
  int GetHistoryItemCount() const;

  
  
  
  
  int GetChapterStopCount(int history_items) const;

  
  
  
  
  
  
  
  
  
  
  int GetIndexOfNextChapterStop(int start_from, bool forward) const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int FindChapterStop(int offset, bool forward, int skip) const;

  
  static const int kMaxHistoryItems;

  
  static const int kMaxChapterStops;

  
  
  
  
  int MenuIndexToNavEntryIndex(int index) const;

  
  bool ItemHasCommand(int index) const;

  
  bool ItemHasIcon(int index) const;

  
  string16 GetShowFullHistoryLabel() const;

  
  NavigationEntry* GetNavigationEntry(int index) const;

  
  
  
  TabContents* GetTabContents() const;

  
  
  
  
  std::string BuildActionName(const std::string& name, int index) const;

  Browser* browser_;

  
  TabContents* test_tab_contents_;

  
  
  ModelType model_type_;

  
  
  std::set<int> requested_favicons_;

  
  CancelableRequestConsumerTSimple<int> load_consumer_;

  
  ui::MenuModelDelegate* menu_model_delegate_;

  DISALLOW_COPY_AND_ASSIGN(BackForwardMenuModel);
};

#endif  
