// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_RENDERER_HOST_PEPPER_DEVICE_ID_FETCHER_H_
#define CHROME_BROWSER_RENDERER_HOST_PEPPER_DEVICE_ID_FETCHER_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "ppapi/c/pp_instance.h"

class Profile;

namespace content {
class BrowserPpapiHost;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace chrome {

class DeviceIDFetcher : public base::RefCountedThreadSafe<DeviceIDFetcher> {
 public:
  typedef base::Callback<void(const std::string&, int32_t)> IDCallback;

  explicit DeviceIDFetcher(int render_process_id);

  
  
  bool Start(const IDCallback& callback);

  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* prefs);

  
  static base::FilePath GetLegacyDeviceIDPath(
      const base::FilePath& profile_path);

 private:
  ~DeviceIDFetcher();

  
  
  void CheckPrefsOnUIThread();

  
  void ComputeOnUIThread(const std::string& salt,
                         const std::string& machine_id);

  
  void LegacyComputeOnBlockingPool(const base::FilePath& profile_path,
                                   const std::string& salt);

  
  
  void RunCallbackOnIOThread(const std::string& id,
                             int32_t result);

  friend class base::RefCountedThreadSafe<DeviceIDFetcher>;

  
  IDCallback callback_;

  
  bool in_progress_;

  int render_process_id_;

  DISALLOW_COPY_AND_ASSIGN(DeviceIDFetcher);
};

}  

#endif  
