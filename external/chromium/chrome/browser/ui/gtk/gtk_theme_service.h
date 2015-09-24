// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_GTK_THEME_SERVICE_H_
#define CHROME_BROWSER_UI_GTK_GTK_THEME_SERVICE_H_
#pragma once

#include <map>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/prefs/pref_change_registrar.h"
#include "chrome/browser/themes/theme_service.h"
#include "chrome/browser/ui/gtk/owned_widget_gtk.h"
#include "content/common/notification_observer.h"
#include "ui/base/gtk/gtk_integers.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/gfx/color_utils.h"

class CairoCachedSurface;
class Profile;

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
  
  static GtkThemeService* GetFrom(Profile* profile);

  GtkThemeService();
  virtual ~GtkThemeService();

  
  
  void InitThemesFor(NotificationObserver* observer);

  
  
  
  
  virtual void Init(Profile* profile);
  virtual SkBitmap* GetBitmapNamed(int id) const;
  virtual SkColor GetColor(int id) const;
  virtual bool HasCustomImage(int id) const;
  virtual void SetTheme(const Extension* extension);
  virtual void UseDefaultTheme();
  virtual void SetNativeTheme();
  virtual bool UsingDefaultTheme();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  
  GtkWidget* BuildChromeButton();

  
  GtkWidget* CreateToolbarSeparator();

  
  bool UseGtkTheme() const;

  
  
  GdkColor GetGdkColor(int id) const;

  
  
  GdkColor GetBorderColor() const;

  
  
  GtkIconSet* GetIconSetForId(int id) const;

  
  
  
  void GetScrollbarColors(GdkColor* thumb_active_color,
                          GdkColor* thumb_inactive_color,
                          GdkColor* track_color);

  
  GtkWidget* fake_label() { return fake_label_.get(); }

  
  
  
  CairoCachedSurface* GetSurfaceNamed(int id, GtkWidget* widget_on_display);

  
  CairoCachedSurface* GetRTLEnabledSurfaceNamed(int id,
                                                GtkWidget* widget_on_display);

  
  
  
  
  
  CairoCachedSurface* GetUnthemedSurfaceNamed(int id,
                                              GtkWidget* widget_on_display);

  
  const SkColor& get_focus_ring_color() const { return focus_ring_color_; }
  const SkColor& get_thumb_active_color() const { return thumb_active_color_; }
  const SkColor& get_thumb_inactive_color() const {
    return thumb_inactive_color_;
  }
  const SkColor& get_track_color() const { return track_color_; }
  const SkColor& get_active_selection_bg_color() const {
    return active_selection_bg_color_;
  }
  const SkColor& get_active_selection_fg_color() const {
    return active_selection_fg_color_;
  }
  const SkColor& get_inactive_selection_bg_color() const {
    return inactive_selection_bg_color_;
  }
  const SkColor& get_inactive_selection_fg_color() const {
    return inactive_selection_fg_color_;
  }

  
  
  static GdkPixbuf* GetFolderIcon(bool native);
  static GdkPixbuf* GetDefaultFavicon(bool native);

  
  
  static bool DefaultUsesSystemTheme();

 private:
  typedef std::map<int, SkColor> ColorMap;
  typedef std::map<int, color_utils::HSL> TintMap;
  typedef std::map<int, SkBitmap*> ImageCache;
  typedef std::map<int, CairoCachedSurface*> CairoCachedSurfaceMap;
  typedef std::map<GdkDisplay*, CairoCachedSurfaceMap> PerDisplaySurfaceMap;

  
  virtual void ClearAllThemeData();

  
  virtual void LoadThemePrefs();

  
  virtual void NotifyThemeChanged();

  
  virtual void FreePlatformCaches();

  
  
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

  
  
  
  
  void FreePerDisplaySurfaces(PerDisplaySurfaceMap* per_display_map);

  
  void FreeIconSets();

  
  SkBitmap* GenerateGtkThemeBitmap(int id) const;

  
  
  
  
  SkBitmap* GenerateFrameImage(int color_id,
                               const char* gradient_name) const;

  
  SkBitmap* GenerateTabImage(int base_id) const;

  
  SkBitmap* GenerateTintedIcon(int base_id,
                               const color_utils::HSL& tint) const;

  
  
  void GetNormalButtonTintHSL(color_utils::HSL* tint) const;

  
  void GetNormalEntryForegroundHSL(color_utils::HSL* tint) const;

  
  
  void GetSelectedEntryForegroundHSL(color_utils::HSL* tint) const;

  
  CairoCachedSurface* GetSurfaceNamedImpl(int id,
                                          PerDisplaySurfaceMap* surface_map,
                                          GdkPixbuf* pixbuf,
                                          GtkWidget* widget_on_display);

  
  CHROMEGTK_CALLBACK_1(GtkThemeService, void, OnStyleSet, GtkStyle*);

  
  
  CHROMEGTK_CALLBACK_0(GtkThemeService, void, OnDestroyChromeButton);

  CHROMEGTK_CALLBACK_1(GtkThemeService, gboolean, OnSeparatorExpose,
                       GdkEventExpose*);

  
  bool use_gtk_;

  
  
  GtkWidget* fake_window_;
  GtkWidget* fake_frame_;
  OwnedWidgetGtk fake_label_;
  OwnedWidgetGtk fake_entry_;
  OwnedWidgetGtk fake_menu_item_;

  
  
  std::vector<GtkWidget*> chrome_buttons_;

  
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

  
  
  
  GtkIconSet* fullscreen_icon_set_;

  
  
  mutable ImageCache gtk_images_;

  
  PerDisplaySurfaceMap per_display_surfaces_;
  PerDisplaySurfaceMap per_display_unthemed_surfaces_;

  PrefChangeRegistrar registrar_;

  
  
  static GtkWidget* icon_widget_;

  
  
  
  
  static GdkPixbuf* default_folder_icon_;
  static GdkPixbuf* default_bookmark_icon_;
};

#endif  
