// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_CREATOR_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_CREATOR_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"

namespace crypto {
class RSAPrivateKey;
}

class FilePath;

// generated randomly (and optionally written to |output_private_key_path|.
class ExtensionCreator {
 public:
  ExtensionCreator() {}

  bool Run(const FilePath& extension_dir,
           const FilePath& crx_path,
           const FilePath& private_key_path,
           const FilePath& private_key_output_path);

  
  std::string error_message() { return error_message_; }

 private:
  
  
  
  
  // it is written to |private_key_output_path| -- if provided).
  bool InitializeInput(const FilePath& extension_dir,
                       const FilePath& private_key_path,
                       const FilePath& private_key_output_path);

  
  crypto::RSAPrivateKey* ReadInputKey(const FilePath& private_key_path);

  
  
  crypto::RSAPrivateKey* GenerateKey(const FilePath& private_key_path);

  
  bool CreateZip(const FilePath& extension_dir, const FilePath& temp_path,
                 FilePath* zip_path);

  
  bool SignZip(const FilePath& zip_path,
               crypto::RSAPrivateKey* private_key,
               std::vector<uint8>* signature);

  
  bool WriteCRX(const FilePath& zip_path,
                crypto::RSAPrivateKey* private_key,
                const std::vector<uint8>& signature,
                const FilePath& crx_path);

  
  std::string error_message_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionCreator);
};

#endif  
