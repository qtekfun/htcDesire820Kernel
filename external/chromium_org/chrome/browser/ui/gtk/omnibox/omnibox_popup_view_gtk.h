// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_OMNIBOX_OMNIBOX_POPUP_VIEW_GTK_H_
#define CHROME_BROWSER_UI_GTK_OMNIBOX_OMNIBOX_POPUP_VIEW_GTK_H_

#include <gtk/gtk.h>

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "chrome/browser/ui/omnibox/omnibox_popup_view.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/window_open_disposition.h"
#include "ui/gfx/font.h"

class AutocompleteResult;
class GtkThemeService;
class OmniboxEditModel;
class OmniboxPopupModel;
class OmniboxView;
class SkBitmap;

namespace gfx {
class Image;
}

class OmniboxPopupViewGtk : public OmniboxPopupView,
                            public content::NotificationObserver {
 public:
  OmniboxPopupViewGtk(const gfx::Font& font,
                      OmniboxView* omnibox_view,
                      OmniboxEditModel* edit_model,
                      GtkWidget* location_bar);
  virtual ~OmniboxPopupViewGtk();

  
  virtual void Init();

  
  virtual bool IsOpen() const OVERRIDE;
  virtual void InvalidateLine(size_t line) OVERRIDE;
  virtual void UpdatePopupAppearance() OVERRIDE;
  virtual gfx::Rect GetTargetBounds() OVERRIDE;
  virtual void PaintUpdatesNow() OVERRIDE;
  virtual void OnDragCanceled() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 protected:
  
  size_t LineFromY(int y) const;

  
  
  
  gfx::Rect GetRectForLine(size_t line, int width) const;

  
  
  
  size_t GetHiddenMatchCount() const;

  
  virtual const AutocompleteResult& GetResult() const;

 private:
  
  friend class OmniboxPopupViewGtkTest;

  static void SetupLayoutForMatch(
      PangoLayout* layout,
      const base::string16& text,
      const AutocompleteMatch::ACMatchClassifications& classifications,
      const GdkColor* base_color,
      const GdkColor* dim_color,
      const GdkColor* url_color,
      const std::string& prefix_text);

  virtual void Show(size_t num_results);
  virtual void Hide();

  
  void StackWindow();

  
  void AcceptLine(size_t line, WindowOpenDisposition disposition);

  
  gfx::Image IconForMatch(const AutocompleteMatch& match,
                          bool selected,
                          bool is_selected_keyword);

  
  
  void GetVisibleMatchForInput(size_t index,
                               const AutocompleteMatch** match,
                               bool* is_selected_keyword);

  CHROMEGTK_CALLBACK_1(OmniboxPopupViewGtk, gboolean, HandleMotion,
                       GdkEventMotion*);

  CHROMEGTK_CALLBACK_1(OmniboxPopupViewGtk, gboolean, HandleButtonPress,
                       GdkEventButton*);

  CHROMEGTK_CALLBACK_1(OmniboxPopupViewGtk, gboolean, HandleButtonRelease,
                       GdkEventButton*);

  CHROMEGTK_CALLBACK_1(OmniboxPopupViewGtk, gboolean, HandleExpose,
                       GdkEventExpose*);

  scoped_ptr<OmniboxPopupModel> model_;
  OmniboxView* omnibox_view_;
  GtkWidget* location_bar_;

  
  
  GtkWidget* window_;
  
  PangoLayout* layout_;

  GtkThemeService* theme_service_;
  content::NotificationRegistrar registrar_;

  
  
  gfx::Font font_;

  
  
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

  DISALLOW_COPY_AND_ASSIGN(OmniboxPopupViewGtk);
};

#endif  
