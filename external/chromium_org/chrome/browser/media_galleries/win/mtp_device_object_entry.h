// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_WIN_MTP_DEVICE_OBJECT_ENTRY_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_WIN_MTP_DEVICE_OBJECT_ENTRY_H_

#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "base/time/time.h"

struct MTPDeviceObjectEntry {
  MTPDeviceObjectEntry();  
  MTPDeviceObjectEntry(const base::string16& object_id,
                       const base::string16& object_name,
                       bool is_directory,
                       int64 size,
                       const base::Time& last_modified_time);

  
  
  base::string16 object_id;

  
  base::string16 name;

  
  bool is_directory;

  
  int64 size;

  
  base::Time last_modified_time;
};

typedef std::vector<MTPDeviceObjectEntry> MTPDeviceObjectEntries;

#endif  
