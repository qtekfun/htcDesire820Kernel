// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_INSTALLER_TEST_RESOURCE_UPDATER_H_
#define CHROME_INSTALLER_TEST_RESOURCE_UPDATER_H_

#include <windows.h>

#include <string>
#include <utility>

#include "base/basictypes.h"

namespace base {
class FilePath;
}

namespace upgrade_test {

class ResourceUpdater {
 public:
  ResourceUpdater();
  ~ResourceUpdater();

  
  bool Initialize(const base::FilePath& pe_image_path);

  
  
  bool Update(const std::wstring& name, const std::wstring& type,
              WORD language_id, const base::FilePath& input_file);

  
  bool Commit();

 private:
  HANDLE handle_;
  DISALLOW_COPY_AND_ASSIGN(ResourceUpdater);
};  

}  

#endif  
