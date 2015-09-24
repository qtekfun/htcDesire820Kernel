// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_LOCAL_ROOT_DELETE_HELPER_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_LOCAL_ROOT_DELETE_HELPER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/platform_file.h"
#include "webkit/browser/fileapi/file_system_url.h"

class GURL;

namespace fileapi {
class FileSystemContext;
}

namespace sync_file_system {

class LocalFileSyncStatus;

class RootDeleteHelper {
 public:
  typedef base::Callback<void(base::PlatformFileError)> FileStatusCallback;

  RootDeleteHelper(fileapi::FileSystemContext* file_system_context,
                   LocalFileSyncStatus* sync_status,
                   const fileapi::FileSystemURL& url,
                   const FileStatusCallback& callback);
  ~RootDeleteHelper();

  void Run();

 private:
  void DidDeleteFileSystem(base::PlatformFileError error);
  void DidResetFileChangeTracker();
  void DidOpenFileSystem(const GURL& root,
                         const std::string& name,
                         base::PlatformFileError error);

  scoped_refptr<fileapi::FileSystemContext> file_system_context_;
  const fileapi::FileSystemURL url_;
  FileStatusCallback callback_;

  
  LocalFileSyncStatus* sync_status_;

  base::WeakPtrFactory<RootDeleteHelper> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(RootDeleteHelper);
};

}  

#endif  
