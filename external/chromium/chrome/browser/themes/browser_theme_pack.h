// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_THEMES_BROWSER_THEME_PACK_H_
#define CHROME_BROWSER_THEMES_BROWSER_THEME_PACK_H_
#pragma once

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/common/extensions/extension.h"
#include "content/browser/browser_thread.h"
#include "ui/gfx/color_utils.h"

class DictionaryValue;
class FilePath;
class RefCountedMemory;
namespace ui {
class DataPack;
}

class BrowserThemePack : public base::RefCountedThreadSafe<
    BrowserThemePack, BrowserThread::DeleteOnFileThread> {
 public:
  
  
  
  static BrowserThemePack* BuildFromExtension(const Extension* extension);

  
  
  
  static scoped_refptr<BrowserThemePack> BuildFromDataPack(
      FilePath path, const std::string& expected_id);

  
  
  
  
  
  bool WriteToDisk(FilePath path) const;

  
  
  
  
  bool GetTint(int id, color_utils::HSL* hsl) const;
  bool GetColor(int id, SkColor* color) const;
  bool GetDisplayProperty(int id, int* result) const;

  
  
  
  
  SkBitmap* GetBitmapNamed(int id) const;

  
  
  RefCountedMemory* GetRawData(int id) const;

  
  bool HasCustomImage(int id) const;

 private:
  friend struct BrowserThread::DeleteOnThread<BrowserThread::FILE>;
  friend class DeleteTask<BrowserThemePack>;
  friend class BrowserThemePackTest;

  
  
  
  typedef std::map<int, SkBitmap*> ImageCache;

  
  typedef std::map<int, scoped_refptr<RefCountedMemory> > RawImages;

  
  typedef std::map<uint32, base::StringPiece> RawDataForWriting;

  
  typedef std::map<int, FilePath> FilePathMap;

  
  BrowserThemePack();

  virtual ~BrowserThemePack();

  
  void BuildHeader(const Extension* extension);

  
  
  void BuildTintsFromJSON(DictionaryValue* tints_value);

  
  
  void BuildColorsFromJSON(DictionaryValue* color_value);

  
  void ReadColorsFromJSON(DictionaryValue* colors_value,
                          std::map<int, SkColor>* temp_colors);
  void GenerateMissingColors(std::map<int, SkColor>* temp_colors);

  
  void BuildDisplayPropertiesFromJSON(DictionaryValue* display_value);

  
  void ParseImageNamesFromJSON(DictionaryValue* images_value,
                               const FilePath& images_path,
                               FilePathMap* file_paths) const;

  
  void BuildSourceImagesArray(const FilePathMap& file_paths);

  
  
  bool LoadRawBitmapsTo(const FilePathMap& file_paths,
                        ImageCache* raw_bitmaps);

  
  
  void GenerateFrameImages(ImageCache* bitmaps) const;

  
  
  void GenerateTintedButtons(const color_utils::HSL& button_tint,
                             ImageCache* processed_bitmaps) const;

  
  
  void GenerateTabBackgroundImages(ImageCache* bitmaps) const;

  
  
  void RepackImages(const ImageCache& images,
                    RawImages* reencoded_images) const;

  
  
  void MergeImageCaches(const ImageCache& source,
                        ImageCache* destination) const;

  
  void AddRawImagesTo(const RawImages& images, RawDataForWriting* out) const;

  
  
  
  color_utils::HSL GetTintInternal(int id) const;

  
  scoped_ptr<ui::DataPack> data_pack_;

  // All structs written to disk need to be packed; no alignment tricks here,
  
#pragma pack(push,1)
  // Header that is written to disk.
  struct BrowserThemePackHeader {
    
    int32 version;

    
    int32 little_endian;

    
    uint8 theme_id[16];
  } *header_;

  
  
  
  struct TintEntry {
    int32 id;
    double h;
    double s;
    double l;
  } *tints_;

  struct ColorPair {
    int32 id;
    SkColor color;
  } *colors_;

  struct DisplayPropertyPair {
    int32 id;
    int32 property;
  } *display_properties_;

  
  
  int* source_images_;
#pragma pack(pop)

  
  
  // image data that needs to be written to the DataPack during WriteToDisk()
  
  RawImages image_memory_;

  
  
  
  
  
  ImageCache prepared_images_;

  
  mutable ImageCache loaded_images_;

  DISALLOW_COPY_AND_ASSIGN(BrowserThemePack);
};

#endif  
