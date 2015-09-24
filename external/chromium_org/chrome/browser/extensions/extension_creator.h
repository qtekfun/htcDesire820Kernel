// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_CREATOR_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_CREATOR_H_

#include <string>
#include <vector>

#include "base/basictypes.h"

namespace base {
class FilePath;
}

namespace crypto {
class RSAPrivateKey;
}

namespace extensions {

// generated randomly (and optionally written to |output_private_key_path|.
class ExtensionCreator {
 public:
  ExtensionCreator();

  
  enum RunFlags {
    kNoRunFlags = 0x0,
    kOverwriteCRX = 0x1,
    kRequireModernManifestVersion = 0x2,
  };

  
  enum ErrorType { kOtherError, kCRXExists };

  bool Run(const base::FilePath& extension_dir,
           const base::FilePath& crx_path,
           const base::FilePath& private_key_path,
           const base::FilePath& private_key_output_path,
           int run_flags);

  
  std::string error_message() { return error_message_; }

  ErrorType error_type() { return error_type_; }

 private:
  
  
  // the path to which final crx will be written.
  
  
  // it is written to |private_key_output_path| -- if provided).
  
  bool InitializeInput(const base::FilePath& extension_dir,
                       const base::FilePath& crx_path,
                       const base::FilePath& private_key_path,
                       const base::FilePath& private_key_output_path,
                       int run_flags);

  
  bool ValidateManifest(const base::FilePath& extension_dir,
                        crypto::RSAPrivateKey* key_pair,
                        int run_flags);

  
  crypto::RSAPrivateKey* ReadInputKey(const base::FilePath& private_key_path);

  
  
  crypto::RSAPrivateKey* GenerateKey(const base::FilePath& private_key_path);

  
  bool CreateZip(const base::FilePath& extension_dir, const base::FilePath& temp_path,
                 base::FilePath* zip_path);

  
  bool SignZip(const base::FilePath& zip_path,
               crypto::RSAPrivateKey* private_key,
               std::vector<uint8>* signature);

  
  bool WriteCRX(const base::FilePath& zip_path,
                crypto::RSAPrivateKey* private_key,
                const std::vector<uint8>& signature,
                const base::FilePath& crx_path);

  
  std::string error_message_;

  
  ErrorType error_type_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionCreator);
};

}  

#endif  
