// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_LINUX_MTP_READ_FILE_WORKER_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_LINUX_MTP_READ_FILE_WORKER_H_

#include <string>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/platform_file.h"

namespace base {
class FilePath;
}

class SnapshotFileDetails;
struct SnapshotRequestInfo;

class MTPReadFileWorker {
 public:
  explicit MTPReadFileWorker(const std::string& device_handle);
  ~MTPReadFileWorker();

  
  
  
  
  
  void WriteDataIntoSnapshotFile(
      const SnapshotRequestInfo& request_info,
      const base::PlatformFileInfo& snapshot_file_info);

 private:
  
  
  
  // (such as how many bytes written to the snapshot file, media device file
  
  
  
  
  
  
  
  void OnDidWriteIntoSnapshotFile(
      scoped_ptr<SnapshotFileDetails> snapshot_file_details);

  
  
  void ReadDataChunkFromDeviceFile(
      scoped_ptr<SnapshotFileDetails> snapshot_file_details);

  
  
  
  
  
  
  void OnDidReadDataChunkFromDeviceFile(
      scoped_ptr<SnapshotFileDetails> snapshot_file_details,
      const std::string& data,
      bool error);

  // Called when the data chunk is written to the
  
  
  // If the write operation succeeds, |bytes_written| is set to a non-zero
  
  
  // If the write operation fails, |bytes_written| is set to zero.
  void OnDidWriteDataChunkIntoSnapshotFile(
      scoped_ptr<SnapshotFileDetails> snapshot_file_details,
      uint32 bytes_written);

  
  const std::string device_handle_;

  
  base::WeakPtrFactory<MTPReadFileWorker> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(MTPReadFileWorker);
};

#endif  
