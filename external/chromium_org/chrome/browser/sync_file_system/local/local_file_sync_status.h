// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_LOCAL_LOCAL_FILE_SYNC_STATUS_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_LOCAL_LOCAL_FILE_SYNC_STATUS_H_

#include <map>
#include <set>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/observer_list.h"
#include "base/threading/non_thread_safe.h"
#include "webkit/browser/fileapi/file_system_url.h"

namespace fileapi {
class FileSystemURL;
}

namespace sync_file_system {

class LocalFileSyncStatus
    : public base::NonThreadSafe {
 public:
  class Observer {
   public:
    Observer() {}
    virtual ~Observer() {}
    virtual void OnSyncEnabled(const fileapi::FileSystemURL& url) = 0;
    virtual void OnWriteEnabled(const fileapi::FileSystemURL& url) = 0;
   private:
    DISALLOW_COPY_AND_ASSIGN(Observer);
  };

  LocalFileSyncStatus();
  ~LocalFileSyncStatus();

  
  
  void StartWriting(const fileapi::FileSystemURL& url);

  
  void EndWriting(const fileapi::FileSystemURL& url);

  
  
  void StartSyncing(const fileapi::FileSystemURL& url);

  
  void EndSyncing(const fileapi::FileSystemURL& url);

  
  bool IsWriting(const fileapi::FileSystemURL& url) const;

  
  bool IsWritable(const fileapi::FileSystemURL& url) const;

  
  
  bool IsSyncable(const fileapi::FileSystemURL& url) const;

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

 private:
  typedef std::map<fileapi::FileSystemURL, int64,
                   fileapi::FileSystemURL::Comparator> URLCountMap;

  bool IsChildOrParentWriting(const fileapi::FileSystemURL& url) const;
  bool IsChildOrParentSyncing(const fileapi::FileSystemURL& url) const;

  
  URLCountMap writing_;

  
  fileapi::FileSystemURLSet syncing_;

  ObserverList<Observer> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(LocalFileSyncStatus);
};

}  

#endif  
