// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_BROWSER_ACTIONS_TOOLBAR_GTK_H_
#define CHROME_BROWSER_UI_GTK_BROWSER_ACTIONS_TOOLBAR_GTK_H_

#include <map>
#include <string>

#include "base/compiler_specific.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/extensions/extension_toolbar_model.h"
#include "chrome/browser/ui/gtk/custom_button.h"
#include "chrome/browser/ui/gtk/menu_gtk.h"
#include "chrome/browser/ui/gtk/overflow_button.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/gtk_signal_registrar.h"
#include "ui/base/gtk/owned_widget_gtk.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/animation/slide_animation.h"

class Browser;
class BrowserActionButton;
class GtkThemeService;
class Profile;

namespace extensions {
class Extension;
}

typedef struct _GdkDragContext GdkDragContext;
typedef struct _GtkWidget GtkWidget;

class BrowserActionsToolbarGtk : public ExtensionToolbarModel::Observer,
                                 public gfx::AnimationDelegate,
                                 public MenuGtk::Delegate,
                                 public ui::SimpleMenuModel::Delegate,
                                 public content::NotificationObserver {
 public:
  explicit BrowserActionsToolbarGtk(Browser* browser);
  virtual ~BrowserActionsToolbarGtk();

  GtkWidget* widget() { return hbox_.get(); }
  GtkWidget* chevron() { return overflow_button_->widget(); }

  
  
  
  GtkWidget* GetBrowserActionWidget(const extensions::Extension* extension);
  BrowserActionButton* GetBrowserActionButton(
      const extensions::Extension* extension);

  int button_count() { return extension_button_map_.size(); }

  Browser* browser() { return browser_; }

  ExtensionToolbarModel* model() { return model_; }

  
  int GetCurrentTabId() const;

  
  void Update();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  bool animating() {
    return resize_animation_.is_animating();
  }

 private:
  friend class BrowserActionButton;

  
  void SetupDrags();

  
  
  void CreateAllButtons();

  
  void SetContainerWidth();

  
  
  void CreateButtonForExtension(const extensions::Extension* extension,
                                int index);

  
  void RemoveButtonForExtension(const extensions::Extension* extension);

  
  
  void UpdateVisibility();

  
  void HidePopup();

  
  
  void AnimateToShowNIcons(int count);

  
  
  
  bool ShouldDisplayBrowserAction(const extensions::Extension* extension);

  
  virtual void BrowserActionAdded(const extensions::Extension* extension,
                                  int index) OVERRIDE;
  virtual void BrowserActionRemoved(
      const extensions::Extension* extension) OVERRIDE;
  virtual void BrowserActionMoved(const extensions::Extension* extension,
                                  int index) OVERRIDE;
  virtual bool BrowserActionShowPopup(
      const extensions::Extension* extension) OVERRIDE;
  virtual void VisibleCountChanged() OVERRIDE;

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;

  
  
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

  
  virtual void StoppedShowing() OVERRIDE;
  virtual bool AlwaysShowIconForCmd(int command_id) const OVERRIDE;

  
  void DragStarted(BrowserActionButton* button, GdkDragContext* drag_context);

  
  
  void SetButtonHBoxWidth(int new_width);

  
  void UpdateChevronVisibility();

  CHROMEGTK_CALLBACK_4(BrowserActionsToolbarGtk, gboolean, OnDragMotion,
                       GdkDragContext*, gint, gint, guint);
  CHROMEGTK_CALLBACK_1(BrowserActionsToolbarGtk, void, OnDragEnd,
                       GdkDragContext*);
  CHROMEGTK_CALLBACK_2(BrowserActionsToolbarGtk, gboolean, OnDragFailed,
                       GdkDragContext*, GtkDragResult);
  CHROMEGTK_CALLBACK_1(BrowserActionsToolbarGtk, void, OnHierarchyChanged,
                       GtkWidget*);
  CHROMEGTK_CALLBACK_1(BrowserActionsToolbarGtk, void, OnSetFocus, GtkWidget*);
  CHROMEGTK_CALLBACK_1(BrowserActionsToolbarGtk, gboolean,
                       OnGripperMotionNotify, GdkEventMotion*);
  CHROMEGTK_CALLBACK_1(BrowserActionsToolbarGtk, gboolean, OnGripperExpose,
                       GdkEventExpose*);
  CHROMEGTK_CALLBACK_1(BrowserActionsToolbarGtk, gboolean,
                       OnGripperEnterNotify, GdkEventCrossing*);
  CHROMEGTK_CALLBACK_1(BrowserActionsToolbarGtk, gboolean,
                       OnGripperLeaveNotify, GdkEventCrossing*);
  CHROMEGTK_CALLBACK_1(BrowserActionsToolbarGtk, gboolean,
                       OnGripperButtonRelease, GdkEventButton*);
  CHROMEGTK_CALLBACK_1(BrowserActionsToolbarGtk, gboolean,
                       OnGripperButtonPress, GdkEventButton*);
  
  CHROMEGTK_CALLBACK_1(BrowserActionsToolbarGtk, gboolean,
                       OnOverflowButtonPress, GdkEventButton*);
  
  CHROMEGTK_CALLBACK_1(BrowserActionsToolbarGtk, gboolean,
                       OnOverflowMenuButtonPress, GdkEventButton*);
  CHROMEGTK_CALLBACK_0(BrowserActionsToolbarGtk, void, OnButtonShowOrHide);

  Browser* browser_;

  Profile* profile_;
  GtkThemeService* theme_service_;

  ExtensionToolbarModel* model_;

  
  ui::OwnedWidgetGtk hbox_;

  
  ui::OwnedWidgetGtk button_hbox_;

  
  scoped_ptr<CustomDrawButton> overflow_button_;
  
  
  ui::OwnedWidgetGtk separator_;
  scoped_ptr<MenuGtk> overflow_menu_;
  scoped_ptr<ui::SimpleMenuModel> overflow_menu_model_;
  ui::OwnedWidgetGtk overflow_area_;
  
  ui::OwnedWidgetGtk overflow_alignment_;

  
  BrowserActionButton* drag_button_;

  
  int drop_index_;

  
  
  
  typedef std::map<std::string, linked_ptr<BrowserActionButton> >
      ExtensionButtonMap;
  ExtensionButtonMap extension_button_map_;

  
  gfx::SlideAnimation resize_animation_;
  
  int desired_width_;
  
  int start_width_;

  ui::GtkSignalRegistrar signals_;

  content::NotificationRegistrar registrar_;

  base::WeakPtrFactory<BrowserActionsToolbarGtk> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(BrowserActionsToolbarGtk);
};

#endif  
