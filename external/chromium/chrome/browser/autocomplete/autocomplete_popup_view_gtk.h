// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_POPUP_VIEW_GTK_H_
#define CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_POPUP_VIEW_GTK_H_
#pragma once

#include <gtk/gtk.h>
#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "chrome/browser/autocomplete/autocomplete_popup_view.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/gfx/font.h"
#include "webkit/glue/window_open_disposition.h"

class AutocompleteEditModel;
class AutocompleteEditView;
class AutocompletePopupModel;
class GtkThemeService;
class Profile;
class SkBitmap;

class AutocompletePopupViewGtk : public AutocompletePopupView,
                                 public NotificationObserver {
 public:
  AutocompletePopupViewGtk(const gfx::Font& font,
                           AutocompleteEditView* edit_view,
                           AutocompleteEditModel* edit_model,
                           Profile* profile,
                           GtkWidget* location_bar);
  ~AutocompletePopupViewGtk();

  
  virtual bool IsOpen() const;
  virtual void InvalidateLine(size_t line);
  virtual void UpdatePopupAppearance();
  virtual gfx::Rect GetTargetBounds();
  virtual void PaintUpdatesNow();
  virtual void OnDragCanceled();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  
  friend class AutocompletePopupViewGtkTest;
  static void SetupLayoutForMatch(
      PangoLayout* layout,
      const string16& text,
      const AutocompleteMatch::ACMatchClassifications& classifications,
      const GdkColor* base_color,
      const GdkColor* dim_color,
      const GdkColor* url_color,
      const std::string& prefix_text);

  void Show(size_t num_results);
  void Hide();

  
  void StackWindow();

  
  size_t LineFromY(int y);

  
  void AcceptLine(size_t line, WindowOpenDisposition disposition);

  GdkPixbuf* IconForMatch(const AutocompleteMatch& match, bool selected);

  static gboolean HandleExposeThunk(GtkWidget* widget, GdkEventExpose* event,
                                    gpointer userdata) {
    return reinterpret_cast<AutocompletePopupViewGtk*>(userdata)->
        HandleExpose(widget, event);
  }
  gboolean HandleExpose(GtkWidget* widget, GdkEventExpose* event);

  static gboolean HandleMotionThunk(GtkWidget* widget, GdkEventMotion* event,
                                    gpointer userdata) {
    return reinterpret_cast<AutocompletePopupViewGtk*>(userdata)->
        HandleMotion(widget, event);
  }
  gboolean HandleMotion(GtkWidget* widget, GdkEventMotion* event);

  static gboolean HandleButtonPressThunk(GtkWidget* widget,
                                         GdkEventButton* event,
                                         gpointer userdata) {
    return reinterpret_cast<AutocompletePopupViewGtk*>(userdata)->
        HandleButtonPress(widget, event);
  }
  gboolean HandleButtonPress(GtkWidget* widget, GdkEventButton* event);

  static gboolean HandleButtonReleaseThunk(GtkWidget* widget,
                                           GdkEventButton* event,
                                           gpointer userdata) {
    return reinterpret_cast<AutocompletePopupViewGtk*>(userdata)->
        HandleButtonRelease(widget, event);
  }
  gboolean HandleButtonRelease(GtkWidget* widget, GdkEventButton* event);

  scoped_ptr<AutocompletePopupModel> model_;
  AutocompleteEditView* edit_view_;
  GtkWidget* location_bar_;

  
  
  GtkWidget* window_;
  
  PangoLayout* layout_;

  GtkThemeService* theme_service_;
  NotificationRegistrar registrar_;

  
  
  gfx::Font font_;

  
  
  typedef std::map<const SkBitmap*, GdkPixbuf*> PixbufMap;
  PixbufMap pixbufs_;

  
  
  GdkColor border_color_;
  GdkColor background_color_;
  GdkColor selected_background_color_;
  GdkColor hovered_background_color_;
  GdkColor content_text_color_;
  GdkColor selected_content_text_color_;
  GdkColor content_dim_text_color_;
  GdkColor selected_content_dim_text_color_;
  GdkColor url_text_color_;
  GdkColor url_selected_text_color_;

  
  
  
  
  
  
  bool ignore_mouse_drag_;

  
  bool opened_;

  DISALLOW_COPY_AND_ASSIGN(AutocompletePopupViewGtk);
};

#endif  
