// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_CHILD_PROCESS_SECURITY_POLICY_IMPL_H_
#define CONTENT_BROWSER_CHILD_PROCESS_SECURITY_POLICY_IMPL_H_


#include <map>
#include <set>
#include <string>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/singleton.h"
#include "base/synchronization/lock.h"
#include "content/public/browser/child_process_security_policy.h"
#include "webkit/common/fileapi/file_system_types.h"
#include "webkit/common/resource_type.h"

class GURL;

namespace base {
class FilePath;
}

namespace fileapi {
class FileSystemURL;
}

namespace content {

class CONTENT_EXPORT ChildProcessSecurityPolicyImpl
    : NON_EXPORTED_BASE(public ChildProcessSecurityPolicy) {
 public:
  
  
  virtual ~ChildProcessSecurityPolicyImpl();

  static ChildProcessSecurityPolicyImpl* GetInstance();

  
  virtual void RegisterWebSafeScheme(const std::string& scheme) OVERRIDE;
  virtual bool IsWebSafeScheme(const std::string& scheme) OVERRIDE;
  virtual void GrantReadFile(int child_id, const base::FilePath& file) OVERRIDE;
  virtual void GrantCreateReadWriteFile(int child_id,
                                        const base::FilePath& file) OVERRIDE;
  virtual void GrantReadFileSystem(
      int child_id,
      const std::string& filesystem_id) OVERRIDE;
  virtual void GrantWriteFileSystem(
      int child_id,
      const std::string& filesystem_id) OVERRIDE;
  virtual void GrantCreateFileForFileSystem(
      int child_id,
      const std::string& filesystem_id) OVERRIDE;
  virtual void GrantCreateReadWriteFileSystem(
      int child_id,
      const std::string& filesystem_id) OVERRIDE;
  virtual void GrantCopyIntoFileSystem(
      int child_id,
      const std::string& filesystem_id) OVERRIDE;
  virtual void GrantDeleteFromFileSystem(
      int child_id,
      const std::string& filesystem_id) OVERRIDE;
  virtual void GrantScheme(int child_id, const std::string& scheme) OVERRIDE;
  virtual bool CanReadFile(int child_id, const base::FilePath& file) OVERRIDE;
  virtual bool CanCreateReadWriteFile(int child_id,
                                      const base::FilePath& file) OVERRIDE;
  virtual bool CanReadFileSystem(int child_id,
                                 const std::string& filesystem_id) OVERRIDE;
  virtual bool CanReadWriteFileSystem(
      int child_id,
      const std::string& filesystem_id) OVERRIDE;
  virtual bool CanCopyIntoFileSystem(int child_id,
                                     const std::string& filesystem_id) OVERRIDE;
  virtual bool CanDeleteFromFileSystem(
      int child_id,
      const std::string& filesystem_id) OVERRIDE;

  
  
  
  void RegisterPseudoScheme(const std::string& scheme);

  
  bool IsPseudoScheme(const std::string& scheme);

  
  
  void Add(int child_id);

  
  
  
  
  void AddWorker(int worker_child_id, int main_render_process_id);

  
  
  void Remove(int child_id);

  
  
  
  
  void GrantRequestURL(int child_id, const GURL& url);

  
  
  
  void GrantRequestSpecificFileURL(int child_id, const GURL& url);

  
  void RevokeAllPermissionsForFile(int child_id, const base::FilePath& file);

  
  void GrantWebUIBindings(int child_id);

  
  void GrantReadRawCookies(int child_id);

  
  void RevokeReadRawCookies(int child_id);

  
  void GrantSendMIDISysExMessage(int child_id);

  
  
  
  bool CanRequestURL(int child_id, const GURL& url);

  
  
  
  bool CanLoadPage(int child_id,
                   const GURL& url,
                   ResourceType::Type resource_type);

  
  bool CanReadFileSystemFile(int child_id, const fileapi::FileSystemURL& url);
  bool CanWriteFileSystemFile(int child_id, const fileapi::FileSystemURL& url);
  bool CanCreateFileSystemFile(int child_id, const fileapi::FileSystemURL& url);
  bool CanCreateReadWriteFileSystemFile(int child_id,
                                        const fileapi::FileSystemURL& url);
  bool CanCopyIntoFileSystemFile(int child_id,
                                 const fileapi::FileSystemURL& url);
  bool CanDeleteFileSystemFile(int child_id,
                               const fileapi::FileSystemURL& url);

  
  
  
  bool HasWebUIBindings(int child_id);

  
  bool CanReadRawCookies(int child_id);

  
  
  
  
  
  bool CanAccessCookiesForOrigin(int child_id, const GURL& gurl);

  
  
  
  
  bool CanSendCookiesForOrigin(int child_id, const GURL& gurl);

  
  
  
  
  void LockToOrigin(int child_id, const GURL& gurl);

  
  
  
  void RegisterFileSystemPermissionPolicy(
      fileapi::FileSystemType type,
      int policy);

  
  bool CanSendMIDISysExMessage(int child_id);

 private:
  friend class ChildProcessSecurityPolicyInProcessBrowserTest;
  friend class ChildProcessSecurityPolicyTest;
  FRIEND_TEST_ALL_PREFIXES(ChildProcessSecurityPolicyInProcessBrowserTest,
                           NoLeak);
  FRIEND_TEST_ALL_PREFIXES(ChildProcessSecurityPolicyTest, FilePermissions);

  class SecurityState;

  typedef std::set<std::string> SchemeSet;
  typedef std::map<int, SecurityState*> SecurityStateMap;
  typedef std::map<int, int> WorkerToMainProcessMap;
  typedef std::map<fileapi::FileSystemType, int> FileSystemPermissionPolicyMap;

  
  ChildProcessSecurityPolicyImpl();
  friend struct DefaultSingletonTraits<ChildProcessSecurityPolicyImpl>;

  
  void AddChild(int child_id);

  
  
  bool ChildProcessHasPermissionsForFile(int child_id,
                                         const base::FilePath& file,
                                         int permissions);

  
  
  void GrantPermissionsForFile(int child_id,
                               const base::FilePath& file,
                               int permissions);

  
  
  
  void GrantPermissionsForFileSystem(
      int child_id,
      const std::string& filesystem_id,
      int permission);

  
  
  
  
  bool HasPermissionsForFile(int child_id,
                             const base::FilePath& file,
                             int permissions);

  
  
  bool HasPermissionsForFileSystemFile(int child_id,
                                       const fileapi::FileSystemURL& url,
                                       int permissions);

  
  
  bool HasPermissionsForFileSystem(
      int child_id,
      const std::string& filesystem_id,
      int permission);

  
  
  base::Lock lock_;

  
  
  SchemeSet web_safe_schemes_;

  
  
  
  SchemeSet pseudo_schemes_;

  
  
  
  
  SecurityStateMap security_state_;

  
  
  WorkerToMainProcessMap worker_map_;

  FileSystemPermissionPolicyMap file_system_policy_map_;

  DISALLOW_COPY_AND_ASSIGN(ChildProcessSecurityPolicyImpl);
};

}  

#endif  
