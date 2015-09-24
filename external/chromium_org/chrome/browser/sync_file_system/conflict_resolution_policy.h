// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_CONFLICT_RESOLUTION_POLICY_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_CONFLICT_RESOLUTION_POLICY_H_

namespace sync_file_system {

enum ConflictResolutionPolicy {
  
  CONFLICT_RESOLUTION_POLICY_UNKNOWN,

  
  
  CONFLICT_RESOLUTION_POLICY_LAST_WRITE_WIN,

  
  
  CONFLICT_RESOLUTION_POLICY_MANUAL,
};

}  

#endif  
