// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_SYNCED_DEVICE_TRACKER_H_
#define CHROME_BROWSER_SYNC_GLUE_SYNCED_DEVICE_TRACKER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list_threadsafe.h"
#include "chrome/browser/sync/glue/change_processor.h"

namespace syncer {
struct UserShare;
}

namespace browser_sync {

class DeviceInfo;

class SyncedDeviceTracker : public ChangeProcessor {
 public:
  SyncedDeviceTracker(syncer::UserShare* user_share,
                      const std::string& cache_guid);
  virtual ~SyncedDeviceTracker();

  
  class Observer {
   public:
    virtual void OnDeviceInfoChange() = 0;
  };

  
  virtual void StartImpl(Profile* profile) OVERRIDE;
  virtual void ApplyChangesFromSyncModel(
      const syncer::BaseTransaction* trans,
      int64 model_version,
      const syncer::ImmutableChangeRecordList& changes) OVERRIDE;
  virtual void CommitChangesFromSyncModel() OVERRIDE;

  
  
  virtual scoped_ptr<DeviceInfo> ReadLocalDeviceInfo(
      const syncer::BaseTransaction &trans) const;
  virtual scoped_ptr<DeviceInfo> ReadLocalDeviceInfo() const;
  virtual void InitLocalDeviceInfo(const base::Closure& callback);
  virtual scoped_ptr<DeviceInfo> ReadDeviceInfo(
      const std::string& client_id) const;
  virtual void GetAllSyncedDeviceInfo(
      ScopedVector<DeviceInfo>* device_info) const;

  virtual std::string cache_guid() const;

  
  
  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

 private:
  friend class SyncedDeviceTrackerTest;

  void InitLocalDeviceInfoContinuation(const base::Closure& callback,
                                       const DeviceInfo& local_info);

  
  void WriteLocalDeviceInfo(const DeviceInfo& info);

  
  
  void WriteDeviceInfo(const sync_pb::DeviceInfoSpecifics& specifics,
                       const std::string& tag);

  syncer::UserShare* user_share_;
  const std::string cache_guid_;
  const std::string local_device_info_tag_;

  
  
  typedef ObserverListThreadSafe<Observer> ObserverList;
  scoped_refptr<ObserverList> observers_;

  base::WeakPtrFactory<SyncedDeviceTracker> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(SyncedDeviceTracker);
};

}  

#endif  
