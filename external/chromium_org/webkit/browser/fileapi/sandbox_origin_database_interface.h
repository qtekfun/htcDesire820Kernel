// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_SANDBOX_ORIGIN_DATABASE_INTERFACE_H_
#define WEBKIT_BROWSER_FILEAPI_SANDBOX_ORIGIN_DATABASE_INTERFACE_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "webkit/browser/webkit_storage_browser_export.h"

namespace fileapi {

class WEBKIT_STORAGE_BROWSER_EXPORT_PRIVATE SandboxOriginDatabaseInterface {
 public:
  struct WEBKIT_STORAGE_BROWSER_EXPORT_PRIVATE OriginRecord {
    std::string origin;
    base::FilePath path;

    OriginRecord();
    OriginRecord(const std::string& origin, const base::FilePath& path);
    ~OriginRecord();
  };

  virtual ~SandboxOriginDatabaseInterface() {}

  
  virtual bool HasOriginPath(const std::string& origin) = 0;

  
  
  virtual bool GetPathForOrigin(const std::string& origin,
                                base::FilePath* directory) = 0;

  
  
  
  
  virtual bool RemovePathForOrigin(const std::string& origin) = 0;

  
  virtual bool ListAllOrigins(std::vector<OriginRecord>* origins) = 0;

  
  virtual void DropDatabase() = 0;

 protected:
  SandboxOriginDatabaseInterface() {}
};

}  

#endif  
