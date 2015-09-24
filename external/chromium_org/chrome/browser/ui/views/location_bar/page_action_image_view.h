// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_PAGE_ACTION_IMAGE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_PAGE_ACTION_IMAGE_VIEW_H_

#include <map>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/extension_action.h"
#include "chrome/browser/extensions/extension_action_icon_factory.h"
#include "chrome/browser/extensions/extension_context_menu_model.h"
#include "chrome/browser/ui/views/extensions/extension_popup.h"
#include "ui/views/context_menu_controller.h"
#include "ui/views/controls/image_view.h"
#include "ui/views/widget/widget_observer.h"

class Browser;
class LocationBarView;

namespace content {
class WebContents;
}
namespace views {
class MenuRunner;
}

class PageActionImageView : public views::ImageView,
                            public ExtensionContextMenuModel::PopupDelegate,
                            public views::WidgetObserver,
                            public views::ContextMenuController,
                            public ExtensionActionIconFactory::Observer,
                            public ExtensionAction::IconAnimation::Observer {
 public:
  PageActionImageView(LocationBarView* owner,
                      ExtensionAction* page_action,
                      Browser* browser);
  virtual ~PageActionImageView();

  ExtensionAction* page_action() { return page_action_; }

  int current_tab_id() { return current_tab_id_; }

  void set_preview_enabled(bool preview_enabled) {
    preview_enabled_ = preview_enabled;
  }

  
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;

  
  virtual void InspectPopup(ExtensionAction* action) OVERRIDE;

  
  virtual void OnWidgetDestroying(views::Widget* widget) OVERRIDE;

  
  virtual void ShowContextMenuForView(View* source,
                                      const gfx::Point& point,
                                      ui::MenuSourceType source_type) OVERRIDE;

  
  virtual void OnIconUpdated() OVERRIDE;

  
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;
  virtual bool CanHandleAccelerators() const OVERRIDE;

  
  
  
  void UpdateVisibility(content::WebContents* contents, const GURL& url);

  
  void ExecuteAction(ExtensionPopup::ShowAction show_action);

 private:
  
  virtual void OnIconChanged() OVERRIDE;

  
  virtual void PaintChildren(gfx::Canvas* canvas) OVERRIDE;

  
  void ShowPopupWithURL(const GURL& popup_url,
                        ExtensionPopup::ShowAction show_action);

  
  void HidePopup();

  
  LocationBarView* owner_;

  
  
  ExtensionAction* page_action_;

  
  Browser* browser_;

  
  
  
  
  scoped_ptr<ExtensionActionIconFactory> icon_factory_;

  
  int current_tab_id_;

  
  GURL current_url_;

  
  std::string tooltip_;

  
  
  bool preview_enabled_;

  
  ExtensionPopup* popup_;

  
  
  scoped_ptr<ui::Accelerator> page_action_keybinding_;

  
  
  scoped_ptr<ui::Accelerator> script_badge_keybinding_;

  scoped_ptr<views::MenuRunner> menu_runner_;

  
  ExtensionAction::IconAnimation::ScopedObserver
      scoped_icon_animation_observer_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(PageActionImageView);
};

#endif  
