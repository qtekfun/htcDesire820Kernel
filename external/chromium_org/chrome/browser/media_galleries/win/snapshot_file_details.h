// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_WIN_SNAPSHOT_FILE_DETAILS_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_WIN_SNAPSHOT_FILE_DETAILS_H_

#include "base/files/file_path.h"
#include "base/platform_file.h"
#include "base/win/scoped_comptr.h"
#include "chrome/browser/media_galleries/fileapi/mtp_device_async_delegate.h"

struct SnapshotRequestInfo {
  SnapshotRequestInfo(
      const base::FilePath& device_file_path,
      const base::FilePath& snapshot_file_path,
      const MTPDeviceAsyncDelegate::CreateSnapshotFileSuccessCallback&
          success_callback,
      const MTPDeviceAsyncDelegate::ErrorCallback& error_callback);

  
  base::FilePath device_file_path;

  
  base::FilePath snapshot_file_path;

  
  MTPDeviceAsyncDelegate::CreateSnapshotFileSuccessCallback
      success_callback;

  
  MTPDeviceAsyncDelegate::ErrorCallback error_callback;
};

class SnapshotFileDetails {
 public:
  explicit SnapshotFileDetails(const SnapshotRequestInfo& request_info);
  ~SnapshotFileDetails();

  void set_file_info(const base::PlatformFileInfo& file_info);
  void set_device_file_stream(IStream* file_stream);
  void set_optimal_transfer_size(DWORD optimal_transfer_size);

  SnapshotRequestInfo request_info() const {
    return request_info_;
  }

  base::PlatformFileInfo file_info() const {
    return file_info_;
  }

  IStream* device_file_stream() const {
    return file_stream_.get();
  }

  DWORD optimal_transfer_size() const {
    return optimal_transfer_size_;
  }

  // Returns true if the data contents of the device file is written to the
  
  bool IsSnapshotFileWriteComplete() const;

  // Adds |bytes_written| to |bytes_written_|.
  // |bytes_written| specifies the total number of bytes transferred during
  
  // If |bytes_written| is valid, returns true and adds |bytes_written| to
  // |bytes_written_|.
  // If |bytes_written| is invalid, returns false and does not add
  // |bytes_written| to |bytes_written_|.
  bool AddBytesWritten(DWORD bytes_written);

 private:
  
  SnapshotRequestInfo request_info_;

  
  base::PlatformFileInfo file_info_;

  
  base::win::ScopedComPtr<IStream> file_stream_;

  
  
  DWORD optimal_transfer_size_;

  // Total number of bytes written into the snapshot file.
  DWORD bytes_written_;
};

#endif  
