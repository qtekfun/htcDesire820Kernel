// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_GTK_THEME_SERVICE_H_
#define CHROME_BROWSER_UI_GTK_GTK_THEME_SERVICE_H_

#include <map>
#include <vector>

#include "base/compiler_specific.h"
#include "base/lazy_instance.h"
#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/themes/theme_service.h"
#include "ui/base/glib/glib_integers.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/owned_widget_gtk.h"
#include "ui/gfx/color_utils.h"

class CustomThemeSupplier;
class Profile;

namespace content {
class NotificationObserver;
}

namespace extensions {
class Extension;
}

namespace gfx {
class CairoCachedSurface;
}

namespace ui {
class GtkSignalRegistrar;
}

typedef struct _GdkDisplay GdkDisplay;
typedef struct _GdkEventExpose GdkEventExpose;
typedef struct _GdkPixbuf GdkPixbuf;
typedef struct _GtkIconSet GtkIconSet;
typedef struct _GtkStyle GtkStyle;
typedef struct _GtkWidget GtkWidget;

class GtkThemeService : public ThemeService {
 public:
  
  
  enum CairoDefaultIcon {
    NATIVE_FAVICON = 1,
    CHROME_FAVICON,
    NATIVE_FOLDER,
    CHROME_FOLDER
  };

  
  static GtkThemeService* GetFrom(Profile* profile);

  GtkThemeService();
  virtual ~GtkThemeService();

  
  
  void InitThemesFor(content::NotificationObserver* observer);

  
  
  
  
  virtual void Init(Profile* profile) OVERRIDE;
  virtual gfx::ImageSkia* GetImageSkiaNamed(int id) const OVERRIDE;
  virtual gfx::Image GetImageNamed(int id) const OVERRIDE;
  virtual SkColor GetColor(int id) const OVERRIDE;
  virtual bool HasCustomImage(int id) const OVERRIDE;
  virtual void SetTheme(const extensions::Extension* extension) OVERRIDE;
  virtual void UseDefaultTheme() OVERRIDE;
  virtual void SetNativeTheme() OVERRIDE;
  virtual bool UsingDefaultTheme() const OVERRIDE;
  virtual bool UsingNativeTheme() const OVERRIDE;
  virtual void SetCustomDefaultTheme(
      scoped_refptr<CustomThemeSupplier> theme_supplier) OVERRIDE;
  virtual bool ShouldInitWithNativeTheme() const OVERRIDE;

  
  
  
  GtkWidget* BuildChromeButton();

  
  
  GtkWidget* BuildChromeLinkButton(const std::string& text);

  
  
  
  GtkWidget* BuildLabel(const std::string& text, const GdkColor& color);

  
  GtkWidget* CreateToolbarSeparator();

  
  
  GdkColor GetGdkColor(int id) const;

  
  
  GdkColor GetBorderColor() const;

  
  
  GtkIconSet* GetIconSetForId(int id) const;

  
  
  
  void GetScrollbarColors(GdkColor* thumb_active_color,
                          GdkColor* thumb_inactive_color,
                          GdkColor* track_color);

  
  GtkWidget* fake_label() { return fake_label_.get(); }

  
  SkColor get_focus_ring_color() const { return focus_ring_color_; }
  SkColor get_thumb_active_color() const { return thumb_active_color_; }
  SkColor get_thumb_inactive_color() const { return thumb_inactive_color_; }
  SkColor get_track_color() const { return track_color_; }
  SkColor get_active_selection_bg_color() const {
    return active_selection_bg_color_;
  }
  SkColor get_active_selection_fg_color() const {
    return active_selection_fg_color_;
  }
  SkColor get_inactive_selection_bg_color() const {
    return inactive_selection_bg_color_;
  }
  SkColor get_inactive_selection_fg_color() const {
    return inactive_selection_fg_color_;
  }
  SkColor get_location_bar_text_color() const {
    return location_bar_text_color_;
  }
  SkColor get_location_bar_bg_color() const {
    return location_bar_bg_color_;
  }

  
  
  
  static gfx::Image GetFolderIcon(bool native);
  static gfx::Image GetDefaultFavicon(bool native);

  
  
  static bool DefaultUsesSystemTheme();

 private:
  typedef std::map<int, SkColor> ColorMap;
  typedef std::map<int, color_utils::HSL> TintMap;
  typedef std::map<int, gfx::Image*> ImageCache;

  
  virtual void LoadThemePrefs() OVERRIDE;

  
  virtual void NotifyThemeChanged() OVERRIDE;

  
  virtual void FreePlatformCaches() OVERRIDE;

  
  
  void SetXDGIconTheme();

  
  
  void LoadGtkValues();

  
  
  GdkColor BuildFrameColors(GtkStyle* frame_style);

  
  void LoadDefaultValues();

  
  
  
  void RebuildMenuIconSets();

  
  void SetThemeColorFromGtk(int id, const GdkColor* color);
  void SetThemeTintFromGtk(int id, const GdkColor* color);

  
  
  
  GdkColor BuildAndSetFrameColor(const GdkColor* base,
                                 const GdkColor* gtk_base,
                                 const color_utils::HSL& tint,
                                 int color_id,
                                 int tint_id);

  
  void FreeIconSets();

  
  SkBitmap GenerateGtkThemeBitmap(int id) const;

  
  
  
  
  SkBitmap GenerateFrameImage(int color_id,
                              const char* gradient_name) const;

  
  SkBitmap GenerateTabImage(int base_id) const;

  
  SkBitmap GenerateTintedIcon(int base_id,
                              const color_utils::HSL& tint) const;

  
  
  void GetNormalButtonTintHSL(color_utils::HSL* tint) const;

  
  void GetNormalEntryForegroundHSL(color_utils::HSL* tint) const;

  
  
  void GetSelectedEntryForegroundHSL(color_utils::HSL* tint) const;

  
  CHROMEGTK_CALLBACK_1(GtkThemeService, void, OnStyleSet, GtkStyle*);

  
  
  CHROMEGTK_CALLBACK_0(GtkThemeService, void, OnDestroyChromeButton);
  CHROMEGTK_CALLBACK_0(GtkThemeService, void, OnDestroyChromeLinkButton);
  CHROMEGTK_CALLBACK_0(GtkThemeService, void, OnDestroyLabel);

  CHROMEGTK_CALLBACK_1(GtkThemeService, gboolean, OnSeparatorExpose,
                       GdkEventExpose*);

  void OnUsesSystemThemeChanged();

  
  bool use_gtk_;

  
  
  GtkWidget* fake_window_;
  GtkWidget* fake_frame_;
  ui::OwnedWidgetGtk fake_label_;
  ui::OwnedWidgetGtk fake_entry_;
  ui::OwnedWidgetGtk fake_menu_item_;

  
  
  
  std::vector<GtkWidget*> chrome_buttons_;
  std::vector<GtkWidget*> link_buttons_;
  std::map<GtkWidget*, GdkColor> labels_;

  
  scoped_ptr<ui::GtkSignalRegistrar> signals_;

  
  
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
  SkColor location_bar_bg_color_;
  SkColor location_bar_text_color_;

  
  
  
  GtkIconSet* fullscreen_icon_set_;

  
  
  mutable ImageCache gtk_images_;

  PrefChangeRegistrar registrar_;

  
  
  static GtkWidget* icon_widget_;

  
  
  
  
  static base::LazyInstance<gfx::Image> default_folder_icon_;
  static base::LazyInstance<gfx::Image> default_bookmark_icon_;
};

#endif  
