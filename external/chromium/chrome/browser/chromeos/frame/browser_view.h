// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FRAME_BROWSER_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_FRAME_BROWSER_VIEW_H_
#pragma once

#include <vector>

#include "chrome/browser/chromeos/status/status_area_host.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "views/controls/menu/menu_wrapper.h"

class AccessibleToolbarView;
class Profile;
class TabStripModel;

namespace ui {
class SimpleMenuModel;
}  

namespace views {
class ImageButton;
class ImageView;
class Menu2;
}  

namespace chromeos {

class StatusAreaView;
class StatusAreaButton;

class BrowserView : public ::BrowserView,
                    public views::ContextMenuController,
                    public views::MenuListener,
                    public StatusAreaHost {
 public:
  explicit BrowserView(Browser* browser);
  virtual ~BrowserView();

  
  virtual void Init() OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void ShowInactive() OVERRIDE;
  virtual void FocusChromeOSStatus() OVERRIDE;
  virtual views::LayoutManager* CreateLayoutManager() const OVERRIDE;
  virtual void ChildPreferredSizeChanged(View* child) OVERRIDE;
  virtual bool GetSavedWindowBounds(gfx::Rect* bounds) const OVERRIDE;
  virtual void Cut() OVERRIDE;
  virtual void Copy() OVERRIDE;
  virtual void Paste() OVERRIDE;

  
  virtual void ShowContextMenuForView(views::View* source,
                                      const gfx::Point& p,
                                      bool is_mouse_gesture) OVERRIDE;

  
  virtual void OnMenuOpened() OVERRIDE;

  
  virtual Profile* GetProfile() const OVERRIDE;
  virtual gfx::NativeWindow GetNativeWindow() const OVERRIDE;
  virtual bool ShouldOpenButtonOptions(
      const views::View* button_view) const OVERRIDE;
  virtual void ExecuteBrowserCommand(int id) const OVERRIDE;
  virtual void OpenButtonOptions(const views::View* button_view) OVERRIDE;
  virtual ScreenMode GetScreenMode() const OVERRIDE;
  virtual TextStyle GetTextStyle() const OVERRIDE;

  gfx::NativeView saved_focused_widget() const {
    return saved_focused_widget_;
  }

 protected:
  virtual void GetAccessiblePanes(
      std::vector<AccessiblePaneView*>* panes);

 private:
  void InitSystemMenu();

  void ShowInternal(bool is_active);

  
  StatusAreaView* status_area_;

  
  scoped_ptr<ui::SimpleMenuModel> system_menu_contents_;
  scoped_ptr<views::Menu2> system_menu_menu_;

  
  
  gfx::NativeView saved_focused_widget_;

  DISALLOW_COPY_AND_ASSIGN(BrowserView);
};

}  

#endif  
