// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COMPONENT_UPDATER_COMPONENT_UNPACKER_H_
#define CHROME_BROWSER_COMPONENT_UPDATER_COMPONENT_UNPACKER_H_

#include <string>
#include <vector>
#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/json/json_file_value_serializer.h"
#include "base/memory/scoped_ptr.h"

class ComponentInstaller;
class ComponentPatcher;

scoped_ptr<base::DictionaryValue> ReadManifest(
    const base::FilePath& unpack_path);

class ComponentUnpacker {
 public:
  
  
  enum Error {
    kNone,
    kInvalidParams,
    kInvalidFile,
    kUnzipPathError,
    kUnzipFailed,
    kNoManifest,
    kBadManifest,
    kBadExtension,
    kInvalidId,
    kInstallerError,
    kIoError,
    kDeltaVerificationFailure,
    kDeltaBadCommands,
    kDeltaUnsupportedCommand,
    kDeltaOperationFailure,
    kDeltaPatchProcessFailure,
    kDeltaMissingExistingFile,
    kFingerprintWriteFailed,
  };
  
  
  ComponentUnpacker(const std::vector<uint8>& pk_hash,
                    const base::FilePath& path,
                    const std::string& fingerprint,
                    ComponentPatcher* patcher,
                    ComponentInstaller* installer);

  
  
  ~ComponentUnpacker();

  Error error() const { return error_; }

  int extended_error() const { return extended_error_; }

 private:
  base::FilePath unpack_path_;
  Error error_;
  int extended_error_;  
};

#endif  
