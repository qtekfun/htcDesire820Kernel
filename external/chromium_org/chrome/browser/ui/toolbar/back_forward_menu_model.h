// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TOOLBAR_BACK_FORWARD_MENU_MODEL_H_
#define CHROME_BROWSER_UI_TOOLBAR_BACK_FORWARD_MENU_MODEL_H_

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/strings/string16.h"
#include "chrome/browser/favicon/favicon_service.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "ui/base/models/menu_model.h"
#include "ui/base/window_open_disposition.h"

class Browser;

namespace chrome {
struct FaviconImageResult;
}

namespace content {
class NavigationEntry;
class WebContents;
}

namespace gfx {
class Image;
}

class BackForwardMenuModel : public ui::MenuModel {
 public:
  
  
  enum ModelType {
    FORWARD_MENU = 1,
    BACKWARD_MENU = 2
  };

  BackForwardMenuModel(Browser* browser, ModelType model_type);
  virtual ~BackForwardMenuModel();

  
  virtual bool HasIcons() const OVERRIDE;
  
  
  
  
  virtual int GetItemCount() const OVERRIDE;
  virtual ItemType GetTypeAt(int index) const OVERRIDE;
  virtual ui::MenuSeparatorType GetSeparatorTypeAt(int index) const OVERRIDE;
  virtual int GetCommandIdAt(int index) const OVERRIDE;
  virtual base::string16 GetLabelAt(int index) const OVERRIDE;
  virtual bool IsItemDynamicAt(int index) const OVERRIDE;
  virtual bool GetAcceleratorAt(int index,
                                ui::Accelerator* accelerator) const OVERRIDE;
  virtual bool IsItemCheckedAt(int index) const OVERRIDE;
  virtual int GetGroupIdAt(int index) const OVERRIDE;
  virtual bool GetIconAt(int index, gfx::Image* icon) OVERRIDE;
  virtual ui::ButtonMenuItemModel* GetButtonMenuItemAt(
      int index) const OVERRIDE;
  virtual bool IsEnabledAt(int index) const OVERRIDE;
  virtual MenuModel* GetSubmenuModelAt(int index) const OVERRIDE;
  virtual void HighlightChangedTo(int index) OVERRIDE;
  virtual void ActivatedAt(int index) OVERRIDE;
  virtual void ActivatedAt(int index, int event_flags) OVERRIDE;
  virtual void MenuWillShow() OVERRIDE;

  
  bool IsSeparator(int index) const;

  
  virtual void SetMenuModelDelegate(
      ui::MenuModelDelegate* menu_model_delegate) OVERRIDE;
  virtual ui::MenuModelDelegate* GetMenuModelDelegate() const OVERRIDE;

 protected:
   ui::MenuModelDelegate* menu_model_delegate() { return menu_model_delegate_; }

 private:
  friend class BackFwdMenuModelTest;
  FRIEND_TEST_ALL_PREFIXES(BackFwdMenuModelTest, BasicCase);
  FRIEND_TEST_ALL_PREFIXES(BackFwdMenuModelTest, MaxItemsTest);
  FRIEND_TEST_ALL_PREFIXES(BackFwdMenuModelTest, ChapterStops);
  FRIEND_TEST_ALL_PREFIXES(BackFwdMenuModelTest, EscapeLabel);
  FRIEND_TEST_ALL_PREFIXES(BackFwdMenuModelTest, FaviconLoadTest);

  
  
  void FetchFavicon(content::NavigationEntry* entry);

  
  void OnFavIconDataAvailable(
      int navigation_entry_unique_id,
      const chrome::FaviconImageResult& image_result);

  
  void set_test_web_contents(content::WebContents* test_web_contents) {
    test_web_contents_ = test_web_contents;
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

  
  base::string16 GetShowFullHistoryLabel() const;

  
  content::NavigationEntry* GetNavigationEntry(int index) const;

  
  
  
  content::WebContents* GetWebContents() const;

  
  
  
  
  std::string BuildActionName(const std::string& name, int index) const;

  Browser* browser_;

  
  content::WebContents* test_web_contents_;

  
  
  ModelType model_type_;

  
  
  
  std::set<int> requested_favicons_;

  
  CancelableTaskTracker cancelable_task_tracker_;

  
  ui::MenuModelDelegate* menu_model_delegate_;

  DISALLOW_COPY_AND_ASSIGN(BackForwardMenuModel);
};

#endif  
