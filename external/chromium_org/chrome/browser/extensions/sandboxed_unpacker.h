// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_SANDBOXED_UNPACKER_H_
#define CHROME_BROWSER_EXTENSIONS_SANDBOXED_UNPACKER_H_

#include <string>

#include "base/files/file_path.h"
#include "base/files/scoped_temp_dir.h"
#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "content/public/browser/utility_process_host_client.h"
#include "extensions/common/manifest.h"

class SkBitmap;

namespace base {
class DictionaryValue;
class SequencedTaskRunner;
}

namespace extensions {
class Extension;

class SandboxedUnpackerClient
    : public base::RefCountedThreadSafe<SandboxedUnpackerClient> {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void OnUnpackSuccess(const base::FilePath& temp_dir,
                               const base::FilePath& extension_root,
                               const base::DictionaryValue* original_manifest,
                               const Extension* extension,
                               const SkBitmap& install_icon) = 0;
  virtual void OnUnpackFailure(const base::string16& error) = 0;

 protected:
  friend class base::RefCountedThreadSafe<SandboxedUnpackerClient>;

  virtual ~SandboxedUnpackerClient() {}
};

class SandboxedUnpacker : public content::UtilityProcessHostClient {
 public:
  
  
  
  SandboxedUnpacker(const base::FilePath& crx_path,
                    Manifest::Location location,
                    int creation_flags,
                    const base::FilePath& extensions_dir,
                    base::SequencedTaskRunner* unpacker_io_task_runner,
                    SandboxedUnpackerClient* client);

  
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
    ABORTED_DUE_TO_SHUTDOWN,

    
    COULD_NOT_READ_CATALOG_DATA_FROM_DISK,
    INVALID_CATALOG_DATA,
    INVALID_PATH_FOR_CATALOG,
    ERROR_SERIALIZING_CATALOG,
    ERROR_SAVING_CATALOG,

    NUM_FAILURE_REASONS
  };

  friend class ProcessHostClient;
  friend class SandboxedUnpackerTest;

  virtual ~SandboxedUnpacker();

  
  
  virtual bool CreateTempDirectory();

  
  
  
  
  
  
  
  
  
  bool ValidateSignature();

  
  void StartProcessOnIOThread(const base::FilePath& temp_crx_path);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnProcessCrashed(int exit_code) OVERRIDE;

  
  void OnUnpackExtensionSucceeded(const base::DictionaryValue& manifest);
  void OnUnpackExtensionFailed(const base::string16& error_message);

  void ReportFailure(FailureReason reason, const base::string16& message);
  void ReportSuccess(const base::DictionaryValue& original_manifest,
                     const SkBitmap& install_icon);

  
  
  base::DictionaryValue* RewriteManifestFile(
      const base::DictionaryValue& manifest);

  
  
  bool RewriteImageFiles(SkBitmap* install_icon);
  bool RewriteCatalogFiles();

  
  void Cleanup();

  
  base::FilePath crx_path_;

  
  scoped_refptr<SandboxedUnpackerClient> client_;

  
  base::FilePath extensions_dir_;

  
  base::ScopedTempDir temp_dir_;

  
  base::FilePath extension_root_;

  
  scoped_refptr<Extension> extension_;

  
  bool got_response_;

  
  std::string public_key_;

  
  
  std::string extension_id_;

  
  base::TimeTicks unpack_start_time_;

  
  Manifest::Location location_;

  
  
  int creation_flags_;

  
  scoped_refptr<base::SequencedTaskRunner> unpacker_io_task_runner_;
};

}  

#endif  
