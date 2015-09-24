// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_THEMES_THEME_SERVICE_H_
#define CHROME_BROWSER_THEMES_THEME_SERVICE_H_

#include <map>
#include <set>
#include <string>
#include <utility>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/theme_provider.h"

class CustomThemeSupplier;
class BrowserThemePack;
class ThemeSyncableService;
class Profile;

namespace base {
class FilePath;
}

namespace color_utils {
struct HSL;
}

namespace extensions {
class Extension;
}

namespace gfx {
class Image;
}

namespace theme_service_internal {
class ThemeServiceTest;
}

namespace ui {
class ResourceBundle;
}

#ifdef __OBJC__
@class NSString;
extern "C" NSString* const kBrowserThemeDidChangeNotification;
#endif  

class ThemeService : public base::NonThreadSafe,
                     public content::NotificationObserver,
                     public BrowserContextKeyedService,
                     public ui::ThemeProvider {
 public:
  
  static const char* kDefaultThemeID;

  ThemeService();
  virtual ~ThemeService();

  virtual void Init(Profile* profile);

  
  
  
  
  virtual gfx::Image GetImageNamed(int id) const;

  
  virtual gfx::ImageSkia* GetImageSkiaNamed(int id) const OVERRIDE;
  virtual SkColor GetColor(int id) const OVERRIDE;
  virtual int GetDisplayProperty(int id) const OVERRIDE;
  virtual bool ShouldUseNativeFrame() const OVERRIDE;
  virtual bool HasCustomImage(int id) const OVERRIDE;
  virtual base::RefCountedMemory* GetRawData(
      int id,
      ui::ScaleFactor scale_factor) const OVERRIDE;
#if defined(OS_MACOSX)
  virtual NSImage* GetNSImageNamed(int id) const OVERRIDE;
  virtual NSColor* GetNSImageColorNamed(int id) const OVERRIDE;
  virtual NSColor* GetNSColor(int id) const OVERRIDE;
  virtual NSColor* GetNSColorTint(int id) const OVERRIDE;
  virtual NSGradient* GetNSGradient(int id) const OVERRIDE;
#elif defined(OS_POSIX) && !defined(TOOLKIT_VIEWS) && !defined(OS_ANDROID)
  
  
  
  
  virtual GdkPixbuf* GetRTLEnabledPixbufNamed(int id) const OVERRIDE;
#endif

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  virtual void SetTheme(const extensions::Extension* extension);

  
  virtual void UseDefaultTheme();

  
  
  virtual void SetNativeTheme();

  
  
  virtual bool UsingDefaultTheme() const;

  
  
  virtual bool UsingNativeTheme() const;

  
  
  virtual std::string GetThemeID() const;

  
  
  void OnInfobarDisplayed();

  
  
  void OnInfobarDestroyed();

  
  
  
  void RemoveUnusedThemes(bool ignore_infobars);

  
  
  virtual ThemeSyncableService* GetThemeSyncableService() const;

  // Save the images to be written to disk, mapping file path to id.
  typedef std::map<base::FilePath, int> ImagesDiskCache;

 protected:
  
  virtual void SetCustomDefaultTheme(
      scoped_refptr<CustomThemeSupplier> theme_supplier);

  
  virtual bool ShouldInitWithNativeTheme() const;

  
  color_utils::HSL GetTint(int id) const;

  
  virtual void ClearAllThemeData();

  
  virtual void LoadThemePrefs();

  
  virtual void NotifyThemeChanged();

#if defined(OS_MACOSX)
  
  virtual void NotifyPlatformThemeChanged();
#endif  

  
  
  virtual void FreePlatformCaches();

  Profile* profile() const { return profile_; }

  void set_ready() { ready_ = true; }

  const CustomThemeSupplier* get_theme_supplier() const {
    return theme_supplier_.get();
  }

  
  
  
  bool ready_;

 private:
  friend class theme_service_internal::ThemeServiceTest;

  
  void OnExtensionServiceReady();

  
  
  void MigrateTheme();

  
  
  void SwapThemeSupplier(scoped_refptr<CustomThemeSupplier> theme_supplier);

  
  void SavePackName(const base::FilePath& pack_path);

  
  void SaveThemeID(const std::string& id);

  
  
  void BuildFromExtension(const extensions::Extension* extension);

  
  bool IsManagedUser() const;

  
  
  void SetManagedUserTheme();

  
  void OnManagedUserInitialized();

#if defined(TOOLKIT_GTK)
  
  GdkPixbuf* GetPixbufImpl(int id, bool rtl_enabled) const;
#endif

#if defined(TOOLKIT_GTK)
  typedef std::map<int, GdkPixbuf*> GdkPixbufMap;
  mutable GdkPixbufMap gdk_pixbufs_;
#elif defined(OS_MACOSX)
  
  typedef std::map<int, NSImage*> NSImageMap;
  mutable NSImageMap nsimage_cache_;

  
  typedef std::map<int, NSColor*> NSColorMap;
  mutable NSColorMap nscolor_cache_;

  typedef std::map<int, NSGradient*> NSGradientMap;
  mutable NSGradientMap nsgradient_cache_;
#endif

  ui::ResourceBundle& rb_;
  Profile* profile_;

  scoped_refptr<CustomThemeSupplier> theme_supplier_;

  
  
  
  
  
  std::string installed_pending_load_id_;

  
  int number_of_infobars_;

  content::NotificationRegistrar registrar_;

  scoped_ptr<ThemeSyncableService> theme_syncable_service_;

  base::WeakPtrFactory<ThemeService> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ThemeService);
};

#endif  
