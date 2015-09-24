// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_FAKE_FREE_DISK_SPACE_GETTER_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_FAKE_FREE_DISK_SPACE_GETTER_H_

#include <list>

#include "base/basictypes.h"
#include "chrome/browser/chromeos/drive/file_cache.h"

namespace drive {

class FakeFreeDiskSpaceGetter : public internal::FreeDiskSpaceGetterInterface {
 public:
  FakeFreeDiskSpaceGetter();
  virtual ~FakeFreeDiskSpaceGetter();

  void set_default_value(int64 value) { default_value_ = value; }

  
  
  
  
  
  
  void PushFakeValue(int64 value);

  
  virtual int64 AmountOfFreeDiskSpace() OVERRIDE;

 private:
  std::list<int64> fake_values_;
  int64 default_value_;

  DISALLOW_COPY_AND_ASSIGN(FakeFreeDiskSpaceGetter);
};

}  

#endif  
