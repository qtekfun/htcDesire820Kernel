// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_SANDBOXED_EXTENSION_UNPACKER_H_
#define CHROME_BROWSER_EXTENSIONS_SANDBOXED_EXTENSION_UNPACKER_H_
#pragma once

#include <string>

#include "base/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_temp_dir.h"
#include "chrome/browser/utility_process_host.h"

class DictionaryValue;
class Extension;
class ResourceDispatcherHost;

class SandboxedExtensionUnpackerClient
    : public base::RefCountedThreadSafe<SandboxedExtensionUnpackerClient> {
 public:
  
  
  
  
  
  
  
  virtual void OnUnpackSuccess(const FilePath& temp_dir,
                               const FilePath& extension_root,
                               const Extension* extension) = 0;
  virtual void OnUnpackFailure(const std::string& error) = 0;

 protected:
  friend class base::RefCountedThreadSafe<SandboxedExtensionUnpackerClient>;

  virtual ~SandboxedExtensionUnpackerClient() {}
};

class SandboxedExtensionUnpacker : public UtilityProcessHost::Client {
 public:
  
  
  static const size_t kExtensionHeaderMagicSize = 4;

  
  
  
  struct ExtensionHeader {
    char magic[kExtensionHeaderMagicSize];
    uint32 version;
    uint32 key_size;  
    uint32 signature_size;  
    
    
  };

  
  static const uint32 kMaxPublicKeySize = 1 << 16;

  
  static const uint32 kMaxSignatureSize = 1 << 16;

  
  static const char kExtensionHeaderMagic[];

  
  static const uint32 kCurrentVersion = 2;

  
  
  
  SandboxedExtensionUnpacker(const FilePath& crx_path,
                             ResourceDispatcherHost* rdh,
                             SandboxedExtensionUnpackerClient* cilent);

  
  void Start();

 private:
  class ProcessHostClient;

  
  
  
  enum FailureReason {
    
    COULD_NOT_GET_TEMP_DIRECTORY,
    COULD_NOT_CREATE_TEMP_DIRECTORY,

    
    FAILED_TO_COPY_EXTENSION_FILE_TO_TEMP_DIRECTORY,
    COULD_NOT_GET_SANDBOX_FRIENDLY_PATH,

    
    COULD_NOT_LOCALIZE_EXTENSION,
    INVALID_MANIFEST,

    
    UNPACKER_CLIENT_FAILED,

    
    UTILITY_PROCESS_CRASHED_WHILE_TRYING_TO_INSTALL,

    
    CRX_FILE_NOT_READABLE,
    CRX_HEADER_INVALID,
    CRX_MAGIC_NUMBER_INVALID,
    CRX_VERSION_NUMBER_INVALID,
    CRX_EXCESSIVELY_LARGE_KEY_OR_SIGNATURE,
    CRX_ZERO_KEY_LENGTH,
    CRX_ZERO_SIGNATURE_LENGTH,
    CRX_PUBLIC_KEY_INVALID,
    CRX_SIGNATURE_INVALID,
    CRX_SIGNATURE_VERIFICATION_INITIALIZATION_FAILED,
    CRX_SIGNATURE_VERIFICATION_FAILED,

    
    ERROR_SERIALIZING_MANIFEST_JSON,
    ERROR_SAVING_MANIFEST_JSON,

    
    COULD_NOT_READ_IMAGE_DATA_FROM_DISK,
    DECODED_IMAGES_DO_NOT_MATCH_THE_MANIFEST,
    INVALID_PATH_FOR_BROWSER_IMAGE,
    ERROR_REMOVING_OLD_IMAGE_FILE,
    INVALID_PATH_FOR_BITMAP_IMAGE,
    ERROR_RE_ENCODING_THEME_IMAGE,
    ERROR_SAVING_THEME_IMAGE,

    
    COULD_NOT_READ_CATALOG_DATA_FROM_DISK,
    INVALID_CATALOG_DATA,
    INVALID_PATH_FOR_CATALOG,
    ERROR_SERIALIZING_CATALOG,
    ERROR_SAVING_CATALOG,

    NUM_FAILURE_REASONS
  };

  friend class ProcessHostClient;
  friend class SandboxedExtensionUnpackerTest;

  virtual ~SandboxedExtensionUnpacker();

  
  
  virtual bool CreateTempDirectory();

  
  
  
  
  
  
  
  
  
  bool ValidateSignature();

  
  void StartProcessOnIOThread(const FilePath& temp_crx_path);

  
  virtual void OnUnpackExtensionSucceeded(const DictionaryValue& manifest);
  virtual void OnUnpackExtensionFailed(const std::string& error_message);
  virtual void OnProcessCrashed(int exit_code);

  void ReportFailure(FailureReason reason, const std::string& message);
  void ReportSuccess();

  
  
  DictionaryValue* RewriteManifestFile(const DictionaryValue& manifest);

  
  
  bool RewriteImageFiles();
  bool RewriteCatalogFiles();

  
  FilePath crx_path_;

  
  BrowserThread::ID thread_identifier_;

  
  ResourceDispatcherHost* rdh_;

  
  scoped_refptr<SandboxedExtensionUnpackerClient> client_;

  
  ScopedTempDir temp_dir_;

  
  FilePath extension_root_;

  
  scoped_refptr<Extension> extension_;

  
  bool got_response_;

  
  std::string public_key_;
};

#endif  
