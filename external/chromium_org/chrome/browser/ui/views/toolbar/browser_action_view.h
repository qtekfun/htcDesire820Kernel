// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TOOLBAR_BROWSER_ACTION_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_TOOLBAR_BROWSER_ACTION_VIEW_H_

#include <string>

#include "chrome/browser/extensions/extension_action_icon_factory.h"
#include "chrome/browser/extensions/extension_context_menu_model.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/views/context_menu_controller.h"
#include "ui/views/controls/button/menu_button.h"
#include "ui/views/controls/button/menu_button_listener.h"
#include "ui/views/drag_controller.h"
#include "ui/views/view.h"

class Browser;
class BrowserActionButton;
class ExtensionAction;

namespace extensions {
class Extension;
}

namespace gfx {
class Image;
}

namespace views {
class MenuItemView;
class MenuRunner;
}

class BrowserActionView : public views::View {
 public:
  
  
  class Delegate : public views::DragController,
                   public ExtensionContextMenuModel::PopupDelegate {
   public:
    
    virtual int GetCurrentTabId() const = 0;

    
    virtual void OnBrowserActionExecuted(BrowserActionButton* button) = 0;

    
    virtual void OnBrowserActionVisibilityChanged() = 0;

    
    virtual gfx::Point GetViewContentOffset() const = 0;

    virtual bool NeedToShowMultipleIconStates() const;
    virtual bool NeedToShowTooltip() const;

   protected:
    virtual ~Delegate() {}
  };

  BrowserActionView(const extensions::Extension* extension,
                    Browser* browser,
                    Delegate* delegate);
  virtual ~BrowserActionView();

  BrowserActionButton* button() { return button_; }

  
  gfx::ImageSkia GetIconWithBadge();

  
  virtual void Layout() OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;

 protected:
  
  virtual void PaintChildren(gfx::Canvas* canvas) OVERRIDE;

 private:
  
  Browser* browser_;

  
  Delegate* delegate_;

  
  BrowserActionButton* button_;

  
  const extensions::Extension* extension_;

  DISALLOW_COPY_AND_ASSIGN(BrowserActionView);
};


class BrowserActionButton : public views::MenuButton,
                            public views::ButtonListener,
                            public views::ContextMenuController,
                            public content::NotificationObserver,
                            public ExtensionActionIconFactory::Observer {
 public:
  BrowserActionButton(const extensions::Extension* extension,
                      Browser* browser_,
                      BrowserActionView::Delegate* delegate);

  
  void Destroy();

  ExtensionAction* browser_action() const { return browser_action_; }
  const extensions::Extension* extension() { return extension_; }

  
  void UpdateState();

  
  virtual bool IsPopup();
  virtual GURL GetPopupUrl();

  
  virtual bool CanHandleAccelerators() const OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void ShowContextMenuForView(View* source,
                                      const gfx::Point& point,
                                      ui::MenuSourceType source_type) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnIconUpdated() OVERRIDE;

  
  
  
  
  
  virtual bool Activate() OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnKeyReleased(const ui::KeyEvent& event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;

  
  
  void SetButtonPushed();
  void SetButtonNotPushed();

  
  
  
  bool IsEnabled(int tab_id) const;

  
  ExtensionActionIconFactory& icon_factory() { return icon_factory_; }

  
  gfx::ImageSkia GetIconForTest();

 protected:
  
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;

 private:
  virtual ~BrowserActionButton();

  
  void MaybeRegisterExtensionCommand();

  
  
  void MaybeUnregisterExtensionCommand(bool only_if_active);

  
  Browser* browser_;

  
  
  ExtensionAction* browser_action_;

  
  const extensions::Extension* extension_;

  
  
  
  
  ExtensionActionIconFactory icon_factory_;

  
  BrowserActionView::Delegate* delegate_;

  
  views::MenuItemView* context_menu_;

  
  
  bool called_registered_extension_command_;

  content::NotificationRegistrar registrar_;

  
  
  scoped_ptr<ui::Accelerator> keybinding_;

  
  scoped_ptr<views::MenuRunner> menu_runner_;

  friend class base::DeleteHelper<BrowserActionButton>;

  DISALLOW_COPY_AND_ASSIGN(BrowserActionButton);
};

#endif  
