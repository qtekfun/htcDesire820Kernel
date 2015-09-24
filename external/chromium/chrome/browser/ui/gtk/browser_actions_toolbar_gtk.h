// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_BROWSER_ACTIONS_TOOLBAR_GTK_H_
#define CHROME_BROWSER_UI_GTK_BROWSER_ACTIONS_TOOLBAR_GTK_H_
#pragma once

#include <map>
#include <string>

#include "base/memory/linked_ptr.h"
#include "base/task.h"
#include "chrome/browser/extensions/extension_toolbar_model.h"
#include "chrome/browser/ui/gtk/custom_button.h"
#include "chrome/browser/ui/gtk/menu_gtk.h"
#include "chrome/browser/ui/gtk/overflow_button.h"
#include "chrome/browser/ui/gtk/owned_widget_gtk.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/base/animation/slide_animation.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/gtk_signal_registrar.h"
#include "ui/base/models/simple_menu_model.h"

class Browser;
class BrowserActionButton;
class Extension;
class GtkThemeService;
class Profile;

typedef struct _GdkDragContext GdkDragContext;
typedef struct _GtkWidget GtkWidget;

class BrowserActionsToolbarGtk : public ExtensionToolbarModel::Observer,
                                 public ui::AnimationDelegate,
                                 public MenuGtk::Delegate,
                                 public ui::SimpleMenuModel::Delegate,
                                 public NotificationObserver {
 public:
  explicit BrowserActionsToolbarGtk(Browser* browser);
  virtual ~BrowserActionsToolbarGtk();

  GtkWidget* widget() { return hbox_.get(); }
  GtkWidget* chevron() { return overflow_button_->widget(); }

  
  
  
  GtkWidget* GetBrowserActionWidget(const Extension* extension);

  int button_count() { return extension_button_map_.size(); }

  Browser* browser() { return browser_; }

  
  int GetCurrentTabId();

  
  void Update();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  bool animating() {
    return resize_animation_.is_animating();
  }

 private:
  friend class BrowserActionButton;

  
  void SetupDrags();

  
  
  void CreateAllButtons();

  
  void SetContainerWidth();

  
  
  void CreateButtonForExtension(const Extension* extension, int index);

  
  void RemoveButtonForExtension(const Extension* extension);

  
  
  void UpdateVisibility();

  
  void HidePopup();

  
  
  void AnimateToShowNIcons(int count);

  
  
  
  bool ShouldDisplayBrowserAction(const Extension* extension);

  
  virtual void BrowserActionAdded(const Extension* extension, int index);
  virtual void BrowserActionRemoved(const Extension* extension);
  virtual void BrowserActionMoved(const Extension* extension, int index);
  virtual void ModelLoaded();

  
  virtual void AnimationProgressed(const ui::Animation* animation);
  virtual void AnimationEnded(const ui::Animation* animation);

  
  
  virtual bool IsCommandIdChecked(int command_id) const;
  virtual bool IsCommandIdEnabled(int command_id) const;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator);
  virtual void ExecuteCommand(int command_id);

  
  virtual void StoppedShowing();
  virtual bool AlwaysShowIconForCmd(int command_id) const;

  
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

  
  OwnedWidgetGtk hbox_;

  
  OwnedWidgetGtk button_hbox_;

  
  scoped_ptr<CustomDrawButton> overflow_button_;
  
  
  GtkWidget* separator_;
  scoped_ptr<MenuGtk> overflow_menu_;
  scoped_ptr<ui::SimpleMenuModel> overflow_menu_model_;
  GtkWidget* overflow_area_;
  
  GtkWidget* overflow_alignment_;

  
  BrowserActionButton* drag_button_;

  
  int drop_index_;

  
  
  
  typedef std::map<std::string, linked_ptr<BrowserActionButton> >
      ExtensionButtonMap;
  ExtensionButtonMap extension_button_map_;

  
  ui::SlideAnimation resize_animation_;
  
  int desired_width_;
  
  int start_width_;

  ui::GtkSignalRegistrar signals_;

  NotificationRegistrar registrar_;

  ScopedRunnableMethodFactory<BrowserActionsToolbarGtk> method_factory_;

  DISALLOW_COPY_AND_ASSIGN(BrowserActionsToolbarGtk);
};

#endif  
