// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_PAGE_ACTION_IMAGE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_PAGE_ACTION_IMAGE_VIEW_H_
#pragma once

#include <map>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/image_loading_tracker.h"
#include "chrome/browser/extensions/extension_context_menu_model.h"
#include "chrome/browser/ui/views/extensions/extension_popup.h"
#include "views/controls/image_view.h"

class LocationBarView;
namespace views {
class Menu2;
};

class PageActionImageView : public views::ImageView,
                            public ImageLoadingTracker::Observer,
                            public ExtensionContextMenuModel::PopupDelegate,
                            public ExtensionPopup::Observer {
 public:
  PageActionImageView(LocationBarView* owner,
                      Profile* profile,
                      ExtensionAction* page_action);
  virtual ~PageActionImageView();

  ExtensionAction* page_action() { return page_action_; }

  int current_tab_id() { return current_tab_id_; }

  void set_preview_enabled(bool preview_enabled) {
    preview_enabled_ = preview_enabled;
  }

  
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual bool OnMousePressed(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const views::MouseEvent& event) OVERRIDE;
  virtual bool OnKeyPressed(const views::KeyEvent& event) OVERRIDE;
  virtual void ShowContextMenu(const gfx::Point& p,
                               bool is_mouse_gesture) OVERRIDE;

  
  virtual void OnImageLoaded(SkBitmap* image,
                             const ExtensionResource& resource,
                             int index) OVERRIDE;

  
  virtual void InspectPopup(ExtensionAction* action) OVERRIDE;

  
  virtual void ExtensionPopupIsClosing(ExtensionPopup* popup) OVERRIDE;

  
  
  
  void UpdateVisibility(TabContents* contents, const GURL& url);

  
  void ExecuteAction(int button, bool inspect_with_devtools);

 private:
  
  void HidePopup();

  
  LocationBarView* owner_;

  
  Profile* profile_;

  
  
  ExtensionAction* page_action_;

  
  typedef std::map<std::string, SkBitmap> PageActionMap;
  PageActionMap page_action_icons_;

  
  scoped_refptr<ExtensionContextMenuModel> context_menu_contents_;
  scoped_ptr<views::Menu2> context_menu_menu_;

  
  
  ImageLoadingTracker tracker_;

  
  int current_tab_id_;

  
  GURL current_url_;

  
  std::string tooltip_;

  
  
  bool preview_enabled_;

  
  ExtensionPopup* popup_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(PageActionImageView);
};

#endif  
