// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_WIN_MTP_DEVICE_OBJECT_ENUMERATOR_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_WIN_MTP_DEVICE_OBJECT_ENUMERATOR_H_

#include "base/files/file_path.h"
#include "base/threading/thread_checker.h"
#include "base/time/time.h"
#include "chrome/browser/media_galleries/win/mtp_device_object_entry.h"

class MTPDeviceObjectEnumerator {
 public:
  explicit MTPDeviceObjectEnumerator(const MTPDeviceObjectEntries& entries);
  ~MTPDeviceObjectEnumerator();

  base::FilePath Next();
  int64 Size();
  bool IsDirectory();
  base::Time LastModifiedTime();

  
  
  base::string16 GetObjectId() const;

 private:
  
  
  bool HasMoreEntries() const;

  
  
  bool IsIndexReadyAndInRange() const;

  
  MTPDeviceObjectEntries object_entries_;

  
  
  size_t index_;

  
  bool is_index_ready_;

  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(MTPDeviceObjectEnumerator);
};

#endif  
