// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_IMAGE_BURNER_CLIENT_H_
#define CHROMEOS_DBUS_IMAGE_BURNER_CLIENT_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_client.h"

namespace chromeos {

class CHROMEOS_EXPORT ImageBurnerClient : public DBusClient {
 public:
  virtual ~ImageBurnerClient();

  
  typedef base::Callback<void()> ErrorCallback;

  
  typedef base::Callback<void(const std::string& target_path,
                              bool success,
                              const std::string& error)> BurnFinishedHandler;

  
  typedef base::Callback<void(const std::string& target_path,
                              int64 num_bytes_burnt,
                              int64 total_size)> BurnProgressUpdateHandler;

  
  virtual void BurnImage(const std::string& from_path,
                         const std::string& to_path,
                         const ErrorCallback& error_callback) = 0;

  
  
  
  
  virtual void SetEventHandlers(
      const BurnFinishedHandler& burn_finished_handler,
      const BurnProgressUpdateHandler& burn_progress_update_handler) = 0;

  
  
  virtual void ResetEventHandlers() = 0;

  
  
  static ImageBurnerClient* Create();

 protected:
  
  ImageBurnerClient();

 private:
  DISALLOW_COPY_AND_ASSIGN(ImageBurnerClient);
};

}  

#endif  
