// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_LINUX_MTP_DEVICE_OBJECT_ENUMERATOR_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_LINUX_MTP_DEVICE_OBJECT_ENUMERATOR_H_

#include <vector>

#include "base/files/file_path.h"
#include "base/time/time.h"
#include "device/media_transfer_protocol/mtp_file_entry.pb.h"

class MTPDeviceObjectEnumerator {
 public:
  explicit MTPDeviceObjectEnumerator(const std::vector<MtpFileEntry>& entries);

  ~MTPDeviceObjectEnumerator();

  base::FilePath Next();
  int64 Size();
  bool IsDirectory();
  base::Time LastModifiedTime();

  
  
  bool GetEntryId(uint32_t* entry_id) const;

 private:
  
  
  bool HasMoreEntries() const;

  
  
  bool IsIndexReadyAndInRange() const;

  
  const std::vector<MtpFileEntry> file_entries_;

  
  
  size_t index_;

  
  bool is_index_ready_;

  DISALLOW_COPY_AND_ASSIGN(MTPDeviceObjectEnumerator);
};

#endif  
