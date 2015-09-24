// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_SYNCABLE_AUTOFILL_MIGRATION_H_
#define CHROME_BROWSER_SYNC_SYNCABLE_AUTOFILL_MIGRATION_H_
#pragma once

namespace syncable {
enum AutofillMigrationState {

  
  
  NOT_DETERMINED,

  
  
  
  NOT_MIGRATED,

  
  
  MIGRATED,

  
  
  
  
  
  INSUFFICIENT_INFO_TO_DETERMINE
};

struct AutofillMigrationDebugInfo {
  enum PropertyToSet {
    MIGRATION_TIME,
    ENTRIES_ADDED,
    PROFILES_ADDED
  };
  int64 autofill_migration_time;
  
  
  
  
  int bookmarks_added_during_migration;
  int autofill_entries_added_during_migration;
  int autofill_profile_added_during_migration;
};

}  

#endif  

