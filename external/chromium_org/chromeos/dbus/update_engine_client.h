// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_UPDATE_ENGINE_CLIENT_H_
#define CHROMEOS_DBUS_UPDATE_ENGINE_CLIENT_H_

#include "base/callback.h"
#include "base/observer_list.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"
#include "chromeos/dbus/dbus_client_implementation_type.h"

#include <string>

namespace chromeos {

class CHROMEOS_EXPORT UpdateEngineClient : public DBusClient {
 public:
  
  
  
  
  
  
  
  enum UpdateStatusOperation {
    UPDATE_STATUS_ERROR = -1,
    UPDATE_STATUS_IDLE = 0,
    UPDATE_STATUS_CHECKING_FOR_UPDATE,
    UPDATE_STATUS_UPDATE_AVAILABLE,
    UPDATE_STATUS_DOWNLOADING,
    UPDATE_STATUS_VERIFYING,
    UPDATE_STATUS_FINALIZING,
    UPDATE_STATUS_UPDATED_NEED_REBOOT,
    UPDATE_STATUS_REPORTING_ERROR_EVENT
  };

  
  struct Status {
    Status() : status(UPDATE_STATUS_IDLE),
               download_progress(0.0),
               last_checked_time(0),
               new_size(0) {
    }

    UpdateStatusOperation status;
    double download_progress;   
    int64_t last_checked_time;  
    std::string new_version;
    int64_t new_size;           
  };

  
  enum UpdateCheckResult {
    UPDATE_RESULT_SUCCESS,
    UPDATE_RESULT_FAILED,
    UPDATE_RESULT_NOTIMPLEMENTED,
  };

  
  class Observer {
   public:
    
    virtual void UpdateStatusChanged(const Status& status) {}
  };

  virtual ~UpdateEngineClient();

  
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;
  
  virtual bool HasObserver(Observer* observer) = 0;

  
  
  typedef base::Callback<void(UpdateCheckResult)> UpdateCheckCallback;

  
  virtual void RequestUpdateCheck(const UpdateCheckCallback& callback) = 0;

  
  virtual void RebootAfterUpdate() = 0;

  
  
  typedef base::Callback<void(const std::string& channel_name)>
      GetChannelCallback;

  
  
  
  
  virtual Status GetLastStatus() = 0;

  
  
  
  
  
  
  
  
  
  
  virtual void SetChannel(const std::string& target_channel,
                          bool is_powerwash_allowed) = 0;

  
  
  
  
  
  virtual void GetChannel(bool get_current_channel,
                          const GetChannelCallback& callback) = 0;

  
  static UpdateCheckCallback EmptyUpdateCheckCallback();

  
  static UpdateEngineClient* Create(DBusClientImplementationType type);

 protected:
  
  UpdateEngineClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(UpdateEngineClient);
};

}  

#endif  
