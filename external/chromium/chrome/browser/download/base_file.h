// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_BASE_FILE_H_
#define CHROME_BROWSER_DOWNLOAD_BASE_FILE_H_
#pragma once

#include <string>

#include "base/file_path.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/power_save_blocker.h"
#include "googleurl/src/gurl.h"

namespace crypto {
class SecureHash;
}
namespace net {
class FileStream;
}

class BaseFile {
 public:
  BaseFile(const FilePath& full_path,
           const GURL& source_url,
           const GURL& referrer_url,
           int64 received_bytes,
           const linked_ptr<net::FileStream>& file_stream);
  ~BaseFile();

  
  bool Initialize(bool calculate_hash);

  
  // written to the file).
  bool AppendDataToFile(const char* data, size_t data_len);

  
  virtual bool Rename(const FilePath& full_path);

  
  virtual void Detach();

  
  void Cancel();

  
  void Finish();

  
  void AnnotateWithSourceInformation();

  FilePath full_path() const { return full_path_; }
  bool in_progress() const { return file_stream_ != NULL; }
  int64 bytes_so_far() const { return bytes_so_far_; }

  
  
  virtual bool GetSha256Hash(std::string* hash);

  virtual std::string DebugString() const;

 protected:
  bool Open();
  void Close();

  
  FilePath full_path_;

 private:
  static const size_t kSha256HashLen = 32;

  
  GURL source_url_;

  
  GURL referrer_url_;

  
  linked_ptr<net::FileStream> file_stream_;

  
  int64 bytes_so_far_;

  
  PowerSaveBlocker power_save_blocker_;

  
  bool calculate_hash_;

  
  
  scoped_ptr<crypto::SecureHash> secure_hash_;

  unsigned char sha256_hash_[kSha256HashLen];

  
  
  bool detached_;

  DISALLOW_COPY_AND_ASSIGN(BaseFile);
};

#endif  
