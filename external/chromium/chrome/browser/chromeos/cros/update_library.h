// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CROS_UPDATE_LIBRARY_H_
#define CHROME_BROWSER_CHROMEOS_CROS_UPDATE_LIBRARY_H_
#pragma once

#include <string>

#include "base/memory/singleton.h"
#include "base/observer_list.h"
#include "base/time.h"
#include "third_party/cros/chromeos_update_engine.h"

namespace chromeos {


class UpdateLibrary {
 public:
  
  
  
  
  

  struct Status {
    Status()
        : status(UPDATE_STATUS_IDLE),
          download_progress(0.0),
          last_checked_time(0),
          new_size(0) {
    }

    explicit Status(const UpdateProgress& x) :
        status(x.status_),
        download_progress(x.download_progress_),
        last_checked_time(x.last_checked_time_),
        new_version(x.new_version_),
        new_size(x.new_size_) {
    }

    UpdateStatusOperation status;
    double download_progress;  
    int64_t last_checked_time;  
    std::string new_version;
    int64_t new_size;  
  };

  class Observer {
   public:
    virtual ~Observer() { }
    virtual void UpdateStatusChanged(UpdateLibrary* library) = 0;
  };


  virtual ~UpdateLibrary() {}
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;
  virtual bool HasObserver(Observer* observer) = 0;

  
  virtual void RequestUpdateCheck(chromeos::UpdateCallback callback,
                                  void* user_data) = 0;

  
  virtual bool RebootAfterUpdate() = 0;

  
  
  virtual void SetReleaseTrack(const std::string& track) = 0;

  
  
  virtual void GetReleaseTrack(chromeos::UpdateTrackCallback callback,
                               void* user_data) = 0;

  virtual const Status& status() const = 0;

  
  
  static UpdateLibrary* GetImpl(bool stub);
};

}  

#endif  
