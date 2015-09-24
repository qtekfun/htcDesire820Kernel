// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_DRIVE_BACKEND_V1_REMOTE_CHANGE_HANDLER_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_DRIVE_BACKEND_V1_REMOTE_CHANGE_HANDLER_H_

#include <map>
#include <set>
#include <string>
#include <utility>

#include "base/files/file_path.h"
#include "base/stl_util.h"
#include "base/time/time.h"
#include "chrome/browser/sync_file_system/file_change.h"
#include "webkit/browser/fileapi/file_system_url.h"

namespace sync_file_system {

class RemoteChangeHandler {
 public:
  struct RemoteChange {
    int64 changestamp;
    std::string resource_id;
    std::string md5_checksum;
    base::Time updated_time;
    fileapi::FileSystemURL url;
    FileChange change;

    RemoteChange();
    RemoteChange(int64 changestamp,
                 const std::string& resource_id,
                 const std::string& md5_checksum,
                 const base::Time& updated_time,
                 const fileapi::FileSystemURL& url,
                 const FileChange& change);
    ~RemoteChange();
  };

  struct RemoteChangeComparator {
    bool operator()(const RemoteChange& left, const RemoteChange& right);
  };

  RemoteChangeHandler();
  virtual ~RemoteChangeHandler();

  
  bool GetChange(RemoteChange* remote_change);

  
  bool GetChangeForURL(const fileapi::FileSystemURL& url,
                       RemoteChange* remote_change);

  
  
  void AppendChange(const RemoteChange& remote_change);

  
  bool RemoveChangeForURL(const fileapi::FileSystemURL& url);

  
  void RemoveChangesForOrigin(const GURL& origin);

  bool HasChangesForOrigin(const GURL& origin) const;
  bool HasChanges() const { return !changes_.empty(); }
  size_t ChangesSize() const { return changes_.size(); }

 private:
  struct ChangeQueueItem {
    int64 changestamp;
    fileapi::FileSystemURL url;

    ChangeQueueItem();
    ChangeQueueItem(int64 changestamp,
                    const fileapi::FileSystemURL& url);
  };

  struct ChangeQueueComparator {
    bool operator()(const ChangeQueueItem& left, const ChangeQueueItem& right);
  };

  typedef std::set<ChangeQueueItem, ChangeQueueComparator> PendingChangeQueue;

  struct ChangeMapItem {
    RemoteChange remote_change;
    PendingChangeQueue::iterator position_in_queue;

    ChangeMapItem();
    ChangeMapItem(const RemoteChange& remote_change,
                  PendingChangeQueue::iterator position_in_queue);
    ~ChangeMapItem();
  };

  
  
  typedef std::map<base::FilePath::StringType, ChangeMapItem> PathToChangeMap;
  typedef std::map<GURL, PathToChangeMap> OriginToChangesMap;

  PendingChangeQueue changes_;
  OriginToChangesMap origin_to_changes_map_;

  DISALLOW_COPY_AND_ASSIGN(RemoteChangeHandler);
};

}  

#endif  
