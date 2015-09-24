// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_UTILITY_EXTENSIONS_UNPACKER_H_
#define CHROME_UTILITY_EXTENSIONS_UNPACKER_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "extensions/common/manifest.h"

class SkBitmap;

namespace base {
class DictionaryValue;
}

namespace extensions {

class Unpacker {
 public:
  Unpacker(const base::FilePath& extension_path,
           const std::string& extension_id,
           Manifest::Location location,
           int creation_flags);
  ~Unpacker();

  
  
  bool Run();

  
  
  
  bool DumpImagesToFile();

  
  
  
  bool DumpMessageCatalogsToFile();

  const string16& error_message() { return error_message_; }
  base::DictionaryValue* parsed_manifest() {
    return parsed_manifest_.get();
  }
  base::DictionaryValue* parsed_catalogs() { return parsed_catalogs_.get(); }

 private:
  
  
  base::DictionaryValue* ReadManifest();

  
  bool ReadAllMessageCatalogs(const std::string& default_locale);

  
  
  bool AddDecodedImage(const base::FilePath& path);

  
  
  bool ReadMessageCatalog(const base::FilePath& message_path);

  
  void SetError(const std::string& error);
  void SetUTF16Error(const string16& error);

  
  base::FilePath extension_path_;

  
  std::string extension_id_;

  
  Manifest::Location location_;

  
  int creation_flags_;

  
  base::FilePath temp_install_dir_;

  
  scoped_ptr<base::DictionaryValue> parsed_manifest_;

  
  
  struct InternalData;
  scoped_ptr<InternalData> internal_data_;

  
  
  scoped_ptr<base::DictionaryValue> parsed_catalogs_;

  
  string16 error_message_;

  DISALLOW_COPY_AND_ASSIGN(Unpacker);
};

}  

#endif  
