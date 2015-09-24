// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_CHILD_PROCESS_SECURITY_POLICY_H_
#define CONTENT_PUBLIC_BROWSER_CHILD_PROCESS_SECURITY_POLICY_H_

#include <string>

#include "base/basictypes.h"
#include "content/common/content_export.h"

namespace base {
class FilePath;
}

namespace content {

class ChildProcessSecurityPolicy {
 public:
  virtual ~ChildProcessSecurityPolicy() {}

  
  
  
  static CONTENT_EXPORT ChildProcessSecurityPolicy* GetInstance();

  
  
  
  virtual void RegisterWebSafeScheme(const std::string& scheme) = 0;

  
  virtual bool IsWebSafeScheme(const std::string& scheme) = 0;

  
  
  
  
  virtual void GrantReadFile(int child_id, const base::FilePath& file) = 0;

  
  
  virtual void GrantCreateReadWriteFile(int child_id,
                                        const base::FilePath& file) = 0;

  
  

  
  
  
  virtual bool CanReadFile(int child_id, const base::FilePath& file) = 0;
  virtual bool CanCreateReadWriteFile(int child_id,
                                      const base::FilePath& file) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  virtual void GrantReadFileSystem(int child_id,
                                   const std::string& filesystem_id) = 0;

  
  
  
  
  
  
  
  virtual void GrantWriteFileSystem(int child_id,
                                    const std::string& filesystem_id) = 0;

  
  
  
  
  
  
  
  virtual void GrantCreateFileForFileSystem(
      int child_id,
      const std::string& filesystem_id) = 0;

  
  
  
  
  
  
  
  virtual void GrantCreateReadWriteFileSystem(
      int child_id,
      const std::string& filesystem_id) = 0;

  
  
  
  virtual void GrantCopyIntoFileSystem(int child_id,
                                       const std::string& filesystem_id) = 0;

  
  
  
  virtual void GrantDeleteFromFileSystem(int child_id,
                                         const std::string& filesystem_id) = 0;

  
  
  virtual void GrantScheme(int child_id, const std::string& scheme) = 0;

  
  virtual bool CanReadFileSystem(int child_id,
                                 const std::string& filesystem_id) = 0;

  
  virtual bool CanReadWriteFileSystem(int child_id,
                                      const std::string& filesystem_id) = 0;

  
  virtual bool CanCopyIntoFileSystem(int child_id,
                                     const std::string& filesystem_id) = 0;

  
  virtual bool CanDeleteFromFileSystem(int child_id,
                                       const std::string& filesystem_id) = 0;
};

}  

#endif  
