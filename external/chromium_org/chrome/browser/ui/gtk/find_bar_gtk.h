// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_FIND_BAR_GTK_H_
#define CHROME_BROWSER_UI_GTK_FIND_BAR_GTK_H_

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/find_bar/find_bar.h"
#include "chrome/browser/ui/gtk/slide_animator_gtk.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/gtk/focus_store_gtk.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/owned_widget_gtk.h"
#include "ui/gfx/point.h"

class Browser;
class BrowserWindowGtk;
class CustomDrawButton;
class FindBarController;
class GtkThemeService;
class NineBox;

typedef struct _GtkFloatingContainer GtkFloatingContainer;

class FindBarGtk : public FindBar,
                   public FindBarTesting,
                   public content::NotificationObserver {
 public:
  explicit FindBarGtk(BrowserWindowGtk* window);
  virtual ~FindBarGtk();

  GtkWidget* widget() const { return slide_widget_->widget(); }

  
  virtual FindBarController* GetFindBarController() const OVERRIDE;
  virtual void SetFindBarController(
      FindBarController* find_bar_controller) OVERRIDE;
  virtual void Show(bool animate) OVERRIDE;
  virtual void Hide(bool animate) OVERRIDE;
  virtual void SetFocusAndSelection() OVERRIDE;
  virtual void ClearResults(const FindNotificationDetails& results) OVERRIDE;
  virtual void StopAnimation() OVERRIDE;
  virtual void MoveWindowIfNecessary(const gfx::Rect& selection_rect,
                                     bool no_redraw) OVERRIDE;
  virtual void SetFindTextAndSelectedRange(
      const base::string16& find_text,
      const gfx::Range& selected_range) OVERRIDE;
  virtual base::string16 GetFindText() OVERRIDE;
  virtual gfx::Range GetSelectedRange() OVERRIDE;
  virtual void UpdateUIForFindResult(const FindNotificationDetails& result,
                                     const base::string16& find_text) OVERRIDE;
  virtual void AudibleAlert() OVERRIDE;
  virtual bool IsFindBarVisible() OVERRIDE;
  virtual void RestoreSavedFocus() OVERRIDE;
  virtual bool HasGlobalFindPasteboard() OVERRIDE;
  virtual void UpdateFindBarForChangedWebContents() OVERRIDE;
  virtual FindBarTesting* GetFindBarTesting() OVERRIDE;

  
  virtual bool GetFindBarWindowInfo(gfx::Point* position,
                                    bool* fully_visible) OVERRIDE;
  virtual base::string16 GetFindSelectedText() OVERRIDE;
  virtual base::string16 GetMatchCountText() OVERRIDE;
  virtual int GetWidth() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  void InitWidgets();

  
  
  void StoreOutsideFocus();

  
  
  
  
  bool MaybeForwardKeyEventToRenderer(GdkEventKey* event);

  
  
  void FindEntryTextInContents(bool forward_search);

  void UpdateMatchLabelAppearance(bool failure);

  
  void Reposition();

  
  
  
  
  
  
  
  
  
  gfx::Rect GetDialogPosition(const gfx::Rect& avoid_overlapping_rect);

  
  
  
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

  
  CHROMEGTK_CALLBACK_0(FindBarGtk, void, OnClicked);

  
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

  
  CHROMEGTK_CALLBACK_0(FindBarGtk, void, OnActivate);

  static void OnWidgetDirectionChanged(GtkWidget* widget,
                                       GtkTextDirection previous_direction,
                                       FindBarGtk* find_bar) {
    find_bar->AdjustTextAlignment();
  }

  static void OnKeymapDirectionChanged(GdkKeymap* keymap,
                                       FindBarGtk* find_bar) {
    find_bar->AdjustTextAlignment();
  }

  CHROMEGTK_CALLBACK_1(FindBarGtk, gboolean, OnFocusIn, GdkEventFocus*);
  CHROMEGTK_CALLBACK_1(FindBarGtk, gboolean, OnFocusOut, GdkEventFocus*);

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

  
  ui::FocusStoreGtk focus_store_;

  
  bool ignore_changed_signal_;

  
  
  int current_fixed_width_;

  scoped_ptr<NineBox> dialog_background_;

  
  
  gfx::Rect selection_rect_;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(FindBarGtk);
};

#endif  
