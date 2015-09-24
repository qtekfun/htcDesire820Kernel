// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_VERSION_LOADER_H_
#define CHROME_BROWSER_CHROMEOS_VERSION_LOADER_H_

#include <string>

#include "base/callback.h"
#include "base/gtest_prod_util.h"
#include "chrome/common/cancelable_task_tracker.h"

namespace chromeos {

class VersionLoader {
 public:
  VersionLoader();
  virtual ~VersionLoader();

  enum VersionFormat {
    VERSION_SHORT,
    VERSION_SHORT_WITH_DATE,
    VERSION_FULL,
  };

  
  typedef base::Callback<void(const std::string&)> GetVersionCallback;
  typedef base::Callback<void(const std::string&)> GetFirmwareCallback;

  
  
  
  CancelableTaskTracker::TaskId GetVersion(VersionFormat format,
                                           const GetVersionCallback& callback,
                                           CancelableTaskTracker* tracker);

  CancelableTaskTracker::TaskId GetFirmware(const GetFirmwareCallback& callback,
                                            CancelableTaskTracker* tracker);

 private:
  FRIEND_TEST_ALL_PREFIXES(VersionLoaderTest, ParseFirmware);

  
  
  class Backend : public base::RefCountedThreadSafe<Backend> {
   public:
    Backend() {}

    
    
    void GetVersion(VersionFormat format, std::string* version);

    
    
    void GetFirmware(std::string* firmware);

   private:
    friend class base::RefCountedThreadSafe<Backend>;

    ~Backend() {}

    DISALLOW_COPY_AND_ASSIGN(Backend);
  };

  
  static std::string ParseFirmware(const std::string& contents);

  scoped_refptr<Backend> backend_;

  DISALLOW_COPY_AND_ASSIGN(VersionLoader);
};

}  

#endif  
