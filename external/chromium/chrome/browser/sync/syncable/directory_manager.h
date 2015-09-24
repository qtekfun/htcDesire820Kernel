// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef CHROME_BROWSER_SYNC_SYNCABLE_DIRECTORY_MANAGER_H_
#define CHROME_BROWSER_SYNC_SYNCABLE_DIRECTORY_MANAGER_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/sync/syncable/dir_open_result.h"
#include "chrome/browser/sync/syncable/syncable.h"
#include "chrome/browser/sync/util/cryptographer.h"
#include "chrome/common/deprecated/event_sys.h"

namespace sync_api { class BaseTransaction; }
namespace syncable { class BaseTransaction; }

namespace syncable {

struct DirectoryManagerEvent {
  enum {
    CLOSED,
    CLOSED_ALL,
    SHUTDOWN,
  } what_happened;
  std::string dirname;
  typedef DirectoryManagerEvent EventType;
  static inline bool IsChannelShutdownEvent(const EventType& event) {
    return SHUTDOWN == event.what_happened;
  }
};

DirectoryManagerEvent DirectoryManagerShutdownEvent();

class DirectoryManager {
 public:
  typedef EventChannel<DirectoryManagerEvent> Channel;

  
  explicit DirectoryManager(const FilePath& root_path);
  virtual ~DirectoryManager();

  static const FilePath GetSyncDataDatabaseFilename();
  const FilePath GetSyncDataDatabasePath() const;

  
  
  
  bool Open(const std::string& name);

  
  
  void Close(const std::string& name);

  
  void FinalSaveChangesForAll();

  
  typedef std::vector<std::string> DirNames;
  void GetOpenDirectories(DirNames* result);

  Channel* channel() const { return channel_; }

  
  
  
  
  browser_sync::Cryptographer* GetCryptographer(
      const sync_api::BaseTransaction* trans) const { return cryptographer(); }
  browser_sync::Cryptographer* GetCryptographer(
      const syncable::BaseTransaction* trans) const { return cryptographer(); }

 protected:
  browser_sync::Cryptographer* cryptographer() const {
    return cryptographer_.get();
  }

  DirOpenResult OpenImpl(const std::string& name, const FilePath& path,
                         bool* was_open);

  
  friend class ScopedDirLookup;

  const FilePath root_path_;

  
  base::Lock lock_;
  Directory* managed_directory_;

  Channel* const channel_;

  scoped_ptr<browser_sync::Cryptographer> cryptographer_;

 private:
  DISALLOW_COPY_AND_ASSIGN(DirectoryManager);
};


class ScopedDirLookup {
 public:
  ScopedDirLookup(DirectoryManager* dirman, const std::string& name);
  ~ScopedDirLookup();

  inline bool good() {
    good_checked_ = true;
    return good_;
  }
  Directory* operator -> () const;
  operator Directory* () const;

 protected:  
  friend class sync_api::BaseTransaction;
  void* operator new(size_t size) { return (::operator new)(size); }

  Directory* dir_;
  bool good_;
  
  
  bool good_checked_;
  DirectoryManager* const dirman_;
};

}  

#endif  
