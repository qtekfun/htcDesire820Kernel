// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_FOLDER_HEADER_VIEW_H_
#define UI_APP_LIST_VIEWS_FOLDER_HEADER_VIEW_H_

#include <string>

#include "ui/app_list/app_list_item_model_observer.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/textfield/textfield_controller.h"
#include "ui/views/view.h"

namespace views {
class ImageButton;
}  

namespace app_list {

class AppListFolderItem;
class AppListFolderView;
class FolderHeaderViewDelegate;

class FolderHeaderView : public views::View,
                         public views::TextfieldController,
                         public views::ButtonListener,
                         public AppListItemModelObserver {
 public:
  explicit FolderHeaderView(FolderHeaderViewDelegate* delegate);
  virtual ~FolderHeaderView();

  void SetFolderItem(AppListFolderItem* folder_item);

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;

 private:
  class FolderNameView;

  
  void Update();

  
  virtual void Layout() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  
  virtual void ContentsChanged(views::Textfield* sender,
                               const base::string16& new_contents) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void ItemIconChanged() OVERRIDE;
  virtual void ItemTitleChanged() OVERRIDE;
  virtual void ItemHighlightedChanged() OVERRIDE;
  virtual void ItemIsInstallingChanged() OVERRIDE;
  virtual void ItemPercentDownloadedChanged() OVERRIDE;

  AppListFolderItem* folder_item_;  

  views::ImageButton* back_button_;  
  FolderNameView* folder_name_view_;  

  FolderHeaderViewDelegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(FolderHeaderView);
};

}  

#endif  
