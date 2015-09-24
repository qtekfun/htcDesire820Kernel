// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_EXTENSION_UNPACKER_H_
#define CHROME_COMMON_EXTENSIONS_EXTENSION_UNPACKER_H_
#pragma once

#include <string>
#include <vector>

#include "base/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/tuple.h"

class DictionaryValue;
class SkBitmap;

class ExtensionUnpacker {
 public:
  typedef std::vector< Tuple2<SkBitmap, FilePath> > DecodedImages;

  explicit ExtensionUnpacker(const FilePath& extension_path);
  ~ExtensionUnpacker();

  
  
  bool Run();

  
  
  
  bool DumpImagesToFile();

  
  
  
  bool DumpMessageCatalogsToFile();

  
  
  
  static bool ReadImagesFromFile(const FilePath& extension_path,
                                 DecodedImages* images);

  
  
  
  static bool ReadMessageCatalogsFromFile(const FilePath& extension_path,
                                          DictionaryValue* catalogs);

  const std::string& error_message() { return error_message_; }
  DictionaryValue* parsed_manifest() {
    return parsed_manifest_.get();
  }
  const DecodedImages& decoded_images() { return decoded_images_; }
  DictionaryValue* parsed_catalogs() { return parsed_catalogs_.get(); }

 private:
  
  
  DictionaryValue* ReadManifest();

  
  bool ReadAllMessageCatalogs(const std::string& default_locale);

  
  
  bool AddDecodedImage(const FilePath& path);

  
  
  bool ReadMessageCatalog(const FilePath& message_path);

  
  void SetError(const std::string& error);

  
  FilePath extension_path_;

  
  FilePath temp_install_dir_;

  
  scoped_ptr<DictionaryValue> parsed_manifest_;

  
  
  DecodedImages decoded_images_;

  
  
  scoped_ptr<DictionaryValue> parsed_catalogs_;

  
  std::string error_message_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionUnpacker);
};

#endif  
