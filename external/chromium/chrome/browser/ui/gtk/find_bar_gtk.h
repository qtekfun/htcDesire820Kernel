// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_FIND_BAR_GTK_H_
#define CHROME_BROWSER_UI_GTK_FIND_BAR_GTK_H_
#pragma once

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/find_bar/find_bar.h"
#include "chrome/browser/ui/gtk/focus_store_gtk.h"
#include "chrome/browser/ui/gtk/owned_widget_gtk.h"
#include "chrome/browser/ui/gtk/slide_animator_gtk.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/gfx/point.h"

class Browser;
class BrowserWindowGtk;
class CustomDrawButton;
class FindBarController;
class GtkThemeService;
class NineBox;
class SlideAnimatorGtk;
class TabContentsContainerGtk;

typedef struct _GtkFloatingContainer GtkFloatingContainer;

class FindBarGtk : public FindBar,
                   public FindBarTesting,
                   public NotificationObserver {
 public:
  explicit FindBarGtk(Browser* browser);
  virtual ~FindBarGtk();

  GtkWidget* widget() const { return slide_widget_->widget(); }

  
  virtual FindBarController* GetFindBarController() const;
  virtual void SetFindBarController(FindBarController* find_bar_controller);
  virtual void Show(bool animate);
  virtual void Hide(bool animate);
  virtual void SetFocusAndSelection();
  virtual void ClearResults(const FindNotificationDetails& results);
  virtual void StopAnimation();
  virtual void MoveWindowIfNecessary(const gfx::Rect& selection_rect,
                                     bool no_redraw);
  virtual void SetFindText(const string16& find_text);
  virtual void UpdateUIForFindResult(const FindNotificationDetails& result,
                                     const string16& find_text);
  virtual void AudibleAlert();
  virtual bool IsFindBarVisible();
  virtual void RestoreSavedFocus();
  virtual FindBarTesting* GetFindBarTesting();

  
  virtual bool GetFindBarWindowInfo(gfx::Point* position,
                                    bool* fully_visible);
  virtual string16 GetFindText();
  virtual string16 GetFindSelectedText();
  virtual string16 GetMatchCountText();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  void InitWidgets();

  
  
  void StoreOutsideFocus();

  
  
  
  
  bool MaybeForwardKeyEventToRenderer(GdkEventKey* event);

  
  
  void FindEntryTextInContents(bool forward_search);

  void UpdateMatchLabelAppearance(bool failure);

  
  void Reposition();

  
  
  
  
  
  
  
  
  
  gfx::Rect GetDialogPosition(gfx::Rect avoid_overlapping_rect);

  
  
  
  void AdjustTextAlignment();

  
  gfx::Point GetPosition();

  static void OnParentSet(GtkWidget* widget, GtkObject* old_parent,
                          FindBarGtk* find_bar);

  static void OnSetFloatingPosition(GtkFloatingContainer* floating_container,
                                    GtkAllocation* allocation,
                                    FindBarGtk* find_bar);

  
  static gboolean OnChanged(GtkWindow* window, FindBarGtk* find_bar);

  static gboolean OnKeyPressEvent(GtkWidget* widget, GdkEventKey* event,
                                  FindBarGtk* find_bar);
  static gboolean OnKeyReleaseEvent(GtkWidget* widget, GdkEventKey* event,
                                    FindBarGtk* find_bar);

  
  static void OnClicked(GtkWidget* button, FindBarGtk* find_bar);

  
  static gboolean OnExpose(GtkWidget* widget, GdkEventExpose* event,
                           FindBarGtk* bar);

  
  static gboolean OnContentEventBoxExpose(GtkWidget* widget,
                                          GdkEventExpose* event,
                                          FindBarGtk* bar);

  
  static gboolean OnFocus(GtkWidget* text_entry, GtkDirectionType focus,
                          FindBarGtk* find_bar);
  static gboolean OnButtonPress(GtkWidget* text_entry, GdkEventButton* e,
                                FindBarGtk* find_bar);

  
  static void OnMoveCursor(GtkEntry* entry, GtkMovementStep step, gint count,
                           gboolean selection, FindBarGtk* bar);

  
  static void OnActivate(GtkEntry* entry, FindBarGtk* bar);

  static void OnWidgetDirectionChanged(GtkWidget* widget,
                                       GtkTextDirection previous_direction,
                                       FindBarGtk* find_bar) {
    find_bar->AdjustTextAlignment();
  }

  static void OnKeymapDirectionChanged(GdkKeymap* keymap,
                                       FindBarGtk* find_bar) {
    find_bar->AdjustTextAlignment();
  }

  static gboolean OnFocusIn(GtkWidget* entry, GdkEventFocus* event,
                            FindBarGtk* find_bar);

  static gboolean OnFocusOut(GtkWidget* entry, GdkEventFocus* event,
                             FindBarGtk* find_bar);

  Browser* browser_;
  BrowserWindowGtk* window_;

  
  GtkThemeService* theme_service_;

  
  scoped_ptr<SlideAnimatorGtk> slide_widget_;

  
  GtkWidget* container_;

  
  
  int container_width_;
  int container_height_;

  
  GtkWidget* text_entry_;

  
  GtkWidget* content_event_box_;
  GtkWidget* content_alignment_;

  
  GtkWidget* border_bin_;
  GtkWidget* border_bin_alignment_;

  
  scoped_ptr<CustomDrawButton> find_previous_button_;
  scoped_ptr<CustomDrawButton> find_next_button_;

  
  GtkWidget* match_count_label_;
  GtkWidget* match_count_event_box_;
  
  
  bool match_label_failure_;

  
  scoped_ptr<CustomDrawButton> close_button_;

  
  int last_reported_matchcount_;

  
  FindBarController* find_bar_controller_;

  
  FocusStoreGtk focus_store_;

  
  bool ignore_changed_signal_;

  
  
  int current_fixed_width_;

  scoped_ptr<NineBox> dialog_background_;

  
  
  gfx::Rect selection_rect_;

  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(FindBarGtk);
};

#endif  
