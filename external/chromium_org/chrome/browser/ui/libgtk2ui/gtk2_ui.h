// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_LIBGTK2UI_GTK2_UI_H_
#define CHROME_BROWSER_UI_LIBGTK2UI_GTK2_UI_H_

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/observer_list.h"
#include "chrome/browser/ui/libgtk2ui/libgtk2ui_export.h"
#include "chrome/browser/ui/libgtk2ui/owned_widget_gtk2.h"
#include "ui/gfx/color_utils.h"
#include "ui/views/linux_ui/linux_ui.h"
#include "ui/views/window/frame_buttons.h"

typedef struct _GdkColor GdkColor;
typedef struct _GtkStyle GtkStyle;
typedef struct _GtkWidget GtkWidget;

class SkBitmap;

namespace gfx {
class Image;
}

namespace libgtk2ui {
class GConfTitlebarListener;

class Gtk2UI : public views::LinuxUI {
 public:
  Gtk2UI();
  virtual ~Gtk2UI();

  void SetWindowButtonOrdering(
    const std::vector<views::FrameButton>& leading_buttons,
    const std::vector<views::FrameButton>& trailing_buttons);

  
  virtual scoped_ptr<ui::LinuxInputMethodContext> CreateInputMethodContext(
      ui::LinuxInputMethodContextDelegate* delegate) const OVERRIDE;

  
  virtual ui::SelectFileDialog* CreateSelectFileDialog(
      ui::SelectFileDialog::Listener* listener,
      ui::SelectFilePolicy* policy) const OVERRIDE;

  
  virtual void Initialize() OVERRIDE;
  virtual gfx::Image GetThemeImageNamed(int id) const OVERRIDE;
  virtual bool GetColor(int id, SkColor* color) const OVERRIDE;
  virtual bool HasCustomImage(int id) const OVERRIDE;
  virtual SkColor GetFocusRingColor() const OVERRIDE;
  virtual SkColor GetThumbActiveColor() const OVERRIDE;
  virtual SkColor GetThumbInactiveColor() const OVERRIDE;
  virtual SkColor GetTrackColor() const OVERRIDE;
  virtual SkColor GetActiveSelectionBgColor() const OVERRIDE;
  virtual SkColor GetActiveSelectionFgColor() const OVERRIDE;
  virtual SkColor GetInactiveSelectionBgColor() const OVERRIDE;
  virtual SkColor GetInactiveSelectionFgColor() const OVERRIDE;
  virtual double GetCursorBlinkInterval() const OVERRIDE;
  virtual ui::NativeTheme* GetNativeTheme() const OVERRIDE;
  virtual void SetUseSystemTheme(bool use_system_theme) OVERRIDE;
  virtual bool GetDefaultUsesSystemTheme() const OVERRIDE;
  virtual void SetDownloadCount(int count) const OVERRIDE;
  virtual void SetProgressFraction(float percentage) const OVERRIDE;
  virtual bool IsStatusIconSupported() const OVERRIDE;
  virtual scoped_ptr<views::StatusIconLinux> CreateLinuxStatusIcon(
      const gfx::ImageSkia& image,
      const base::string16& tool_tip) const OVERRIDE;
  virtual gfx::Image GetIconForContentType(
      const std::string& content_type, int size) const OVERRIDE;
  virtual void AddWindowButtonOrderObserver(
      views::WindowButtonOrderObserver* observer) OVERRIDE;
  virtual void RemoveWindowButtonOrderObserver(
      views::WindowButtonOrderObserver* observer) OVERRIDE;
  virtual bool UnityIsRunning() OVERRIDE;

 private:
  typedef std::map<int, SkColor> ColorMap;
  typedef std::map<int, color_utils::HSL> TintMap;
  typedef std::map<int, gfx::Image> ImageCache;

  
  
  
  void GetScrollbarColors(GdkColor* thumb_active_color,
                          GdkColor* thumb_inactive_color,
                          GdkColor* track_color);

  
  
  void SetXDGIconTheme();

  
  
  void LoadGtkValues();

  
  
  GdkColor BuildFrameColors(GtkStyle* frame_style);

  
  void SetThemeColorFromGtk(int id, const GdkColor* color);
  void SetThemeTintFromGtk(int id, const GdkColor* color);

  
  
  
  GdkColor BuildAndSetFrameColor(const GdkColor* base,
                                 const GdkColor* gtk_base,
                                 const color_utils::HSL& tint,
                                 int color_id,
                                 int tint_id);

  
  SkBitmap GenerateGtkThemeBitmap(int id) const;

  
  
  
  
  SkBitmap GenerateFrameImage(int color_id,
                              const char* gradient_name) const;

  
  SkBitmap GenerateTabImage(int base_id) const;

  
  SkBitmap GenerateTintedIcon(int base_id,
                              const color_utils::HSL& tint) const;

  
  
  SkBitmap GenerateGTKIcon(int base_id) const;

  
  
  SkBitmap GenerateToolbarBezel(int gtk_state, int sizing_idr) const;

  
  
  void GetNormalButtonTintHSL(color_utils::HSL* tint) const;

  
  void GetNormalEntryForegroundHSL(color_utils::HSL* tint) const;

  
  
  void GetSelectedEntryForegroundHSL(color_utils::HSL* tint) const;

  
  SkBitmap DrawGtkButtonBorder(int gtk_state, int width, int height) const;

  
  void ClearAllThemeData();

  GtkWidget* fake_window_;
  GtkWidget* fake_frame_;
  OwnedWidgetGtk fake_label_;
  OwnedWidgetGtk fake_entry_;

  
  
  ColorMap colors_;
  TintMap tints_;

  
  color_utils::HSL button_tint_;
  color_utils::HSL entry_tint_;
  color_utils::HSL selected_entry_tint_;

  
  
  SkColor focus_ring_color_;
  SkColor thumb_active_color_;
  SkColor thumb_inactive_color_;
  SkColor track_color_;
  SkColor active_selection_bg_color_;
  SkColor active_selection_fg_color_;
  SkColor inactive_selection_bg_color_;
  SkColor inactive_selection_fg_color_;

#if defined(USE_GCONF)
  
  
  scoped_ptr<GConfTitlebarListener> titlebar_listener_;
#endif  

  
  
  std::vector<views::FrameButton> leading_buttons_;
  std::vector<views::FrameButton> trailing_buttons_;

  
  ObserverList<views::WindowButtonOrderObserver> observer_list_;

  
  mutable ImageCache gtk_images_;

  
  bool use_gtk_;

  DISALLOW_COPY_AND_ASSIGN(Gtk2UI);
};

}  

LIBGTK2UI_EXPORT views::LinuxUI* BuildGtk2UI();

#endif  
