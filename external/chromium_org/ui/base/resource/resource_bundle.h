// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_RESOURCE_RESOURCE_BUNDLE_H_
#define UI_BASE_RESOURCE_RESOURCE_BUNDLE_H_

#include "build/build_config.h"

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/platform_file.h"
#include "base/strings/string16.h"
#include "base/strings/string_piece.h"
#include "ui/base/layout.h"
#include "ui/base/ui_export.h"
#include "ui/gfx/font_list.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/native_widget_types.h"

class SkBitmap;

namespace base {
class Lock;
class RefCountedStaticMemory;
}

namespace ui {

class DataPack;
class ResourceHandle;

class UI_EXPORT ResourceBundle {
 public:
  
  
  
  enum FontStyle {
    
    
    SmallFont,
    SmallBoldFont,
    BaseFont,
    BoldFont,
    MediumFont,
    MediumBoldFont,
    LargeFont,
    LargeBoldFont,
  };

  enum ImageRTL {
    
    RTL_ENABLED,
    
    RTL_DISABLED,
  };

  
  
  class Delegate {
   public:
    
    
    
    
    virtual base::FilePath GetPathForResourcePack(
        const base::FilePath& pack_path,
        ScaleFactor scale_factor) = 0;

    
    
    
    
    virtual base::FilePath GetPathForLocalePack(
        const base::FilePath& pack_path,
        const std::string& locale) = 0;

    
    
    virtual gfx::Image GetImageNamed(int resource_id) = 0;

    
    
    virtual gfx::Image GetNativeImageNamed(int resource_id, ImageRTL rtl) = 0;

    
    
    virtual base::RefCountedStaticMemory* LoadDataResourceBytes(
        int resource_id,
        ScaleFactor scale_factor) = 0;

    
    
    virtual bool GetRawDataResource(int resource_id,
                                    ScaleFactor scale_factor,
                                    base::StringPiece* value) = 0;

    
    
    virtual bool GetLocalizedString(int message_id, base::string16* value) = 0;

    
    virtual scoped_ptr<gfx::Font> GetFont(FontStyle style) = 0;

   protected:
    virtual ~Delegate() {}
  };

  
  
  
  
  
  
  
  
  
  static std::string InitSharedInstanceWithLocale(
      const std::string& pref_locale, Delegate* delegate);

  
  
  static std::string InitSharedInstanceLocaleOnly(
      const std::string& pref_locale, Delegate* delegate);

  
  
  
  
  static void InitSharedInstanceWithPakFile(
      base::PlatformFile file, bool should_load_common_resources);

  
  static void InitSharedInstanceWithPakPath(const base::FilePath& path);

  
  static void CleanupSharedInstance();

  
  static bool HasSharedInstance();

  
  static ResourceBundle& GetSharedInstance();

  
  bool LocaleDataPakExists(const std::string& locale);

  
  
  
  
  
  
  
  void AddDataPackFromPath(const base::FilePath& path,
                           ScaleFactor scale_factor);

  
  void AddDataPackFromFile(base::PlatformFile file, ScaleFactor scale_factor);

  
  
  void AddOptionalDataPackFromPath(const base::FilePath& path,
                                   ScaleFactor scale_factor);

  
  
  
  
  
  
  std::string ReloadLocaleResources(const std::string& pref_locale);

  
  
  
  
  
  
  gfx::ImageSkia* GetImageSkiaNamed(int resource_id);

  
  
  gfx::Image& GetImageNamed(int resource_id);

  
  
  
  
  
  
  
  
  
  gfx::Image& GetNativeImageNamed(int resource_id, ImageRTL rtl);

  
  gfx::Image& GetNativeImageNamed(int resource_id);

  
  base::RefCountedStaticMemory* LoadDataResourceBytes(int resource_id) const;

  
  
  
  
  
  base::RefCountedStaticMemory* LoadDataResourceBytesForScale(
      int resource_id,
      ScaleFactor scale_factor) const;

  
  
  base::StringPiece GetRawDataResource(int resource_id) const;

  
  
  
  
  base::StringPiece GetRawDataResourceForScale(int resource_id,
                                               ScaleFactor scale_factor) const;

  
  
  base::string16 GetLocalizedString(int message_id);

  
  const gfx::FontList& GetFontList(FontStyle style);

  
  const gfx::Font& GetFont(FontStyle style);

  
  
  void ReloadFonts();

  
  
  void OverrideLocalePakForTest(const base::FilePath& pak_path);

  
  
  
  
  base::FilePath GetLocaleFilePath(const std::string& app_locale,
                                   bool test_file_exists);

  
  
  ScaleFactor GetMaxScaleFactor() const;

 private:
  FRIEND_TEST_ALL_PREFIXES(ResourceBundleTest, DelegateGetPathForLocalePack);
  FRIEND_TEST_ALL_PREFIXES(ResourceBundleTest, DelegateGetImageNamed);
  FRIEND_TEST_ALL_PREFIXES(ResourceBundleTest, DelegateGetNativeImageNamed);

  friend class ResourceBundleImageTest;
  friend class ResourceBundleTest;

  class ResourceBundleImageSource;
  friend class ResourceBundleImageSource;

  
  explicit ResourceBundle(Delegate* delegate);
  ~ResourceBundle();

  
  static void InitSharedInstance(Delegate* delegate);

  
  void FreeImages();

  
  void LoadCommonResources();

  
  
  void AddDataPackFromPathInternal(const base::FilePath& path,
                                   ScaleFactor scale_factor,
                                   bool optional);

  
  
  void AddDataPack(DataPack* data_pack);

  
  
  std::string LoadLocaleResources(const std::string& pref_locale);

  
  
  void LoadTestResources(const base::FilePath& path,
                         const base::FilePath& locale_path);

  
  
  void UnloadLocaleResources();

  
  
  void LoadFontsIfNecessary();

  
  
  scoped_ptr<gfx::FontList> GetFontListFromDelegate(FontStyle style);

  
  
  
  
  
  bool LoadBitmap(const ResourceHandle& data_handle,
                  int resource_id,
                  SkBitmap* bitmap,
                  bool* fell_back_to_1x) const;

  
  
  
  
  bool LoadBitmap(int resource_id,
                  ScaleFactor* scale_factor,
                  SkBitmap* bitmap,
                  bool* fell_back_to_1x) const;

  
  
  static bool ShouldHighlightMissingScaledResources();

  
  
  static bool PNGContainsFallbackMarker(const unsigned char* buf, size_t size);

  
  
  
  
  static bool DecodePNG(const unsigned char* buf,
                        size_t size,
                        SkBitmap* bitmap,
                        bool* fell_back_to_1x);

  
  
  gfx::Image& GetEmptyImage();

  const base::FilePath& GetOverriddenPakPath();

  
  
  Delegate* delegate_;

  
  scoped_ptr<base::Lock> images_and_fonts_lock_;

  
  scoped_ptr<base::Lock> locale_resources_data_lock_;

  
  scoped_ptr<ResourceHandle> locale_resources_data_;
  ScopedVector<ResourceHandle> data_packs_;

  
  ScaleFactor max_scale_factor_;

  
  
  typedef std::map<int, gfx::Image> ImageMap;
  ImageMap images_;

  gfx::Image empty_image_;

  
  
  scoped_ptr<gfx::FontList> base_font_list_;
  scoped_ptr<gfx::FontList> bold_font_list_;
  scoped_ptr<gfx::FontList> small_font_list_;
  scoped_ptr<gfx::FontList> small_bold_font_list_;
  scoped_ptr<gfx::FontList> medium_font_list_;
  scoped_ptr<gfx::FontList> medium_bold_font_list_;
  scoped_ptr<gfx::FontList> large_font_list_;
  scoped_ptr<gfx::FontList> large_bold_font_list_;
  scoped_ptr<gfx::FontList> web_font_list_;

  base::FilePath overridden_pak_path_;

  DISALLOW_COPY_AND_ASSIGN(ResourceBundle);
};

}  

using ui::ResourceBundle;

#endif  
