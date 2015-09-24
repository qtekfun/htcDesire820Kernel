// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_COMMON_BLOB_SHAREABLE_FILE_REFERENCE_H_
#define WEBKIT_COMMON_BLOB_SHAREABLE_FILE_REFERENCE_H_

#include <vector>

#include "webkit/common/blob/scoped_file.h"

namespace webkit_blob {

class WEBKIT_STORAGE_COMMON_EXPORT ShareableFileReference
    : public base::RefCounted<ShareableFileReference> {
 public:
  typedef ScopedFile::ScopeOutCallback FinalReleaseCallback;

  enum FinalReleasePolicy {
    DELETE_ON_FINAL_RELEASE = ScopedFile::DELETE_ON_SCOPE_OUT,
    DONT_DELETE_ON_FINAL_RELEASE = ScopedFile::DONT_DELETE_ON_SCOPE_OUT,
  };

  
  
  static scoped_refptr<ShareableFileReference> Get(const base::FilePath& path);

  
  
  
  static scoped_refptr<ShareableFileReference> GetOrCreate(
      const base::FilePath& path,
      FinalReleasePolicy policy,
      base::TaskRunner* file_task_runner);

  
  
  
  
  
  
  
  
  
  
  static scoped_refptr<ShareableFileReference> GetOrCreate(
      ScopedFile scoped_file);

  
  const base::FilePath& path() const { return scoped_file_.path(); }

  
  
  
  void AddFinalReleaseCallback(const FinalReleaseCallback& callback);

 private:
  friend class base::RefCounted<ShareableFileReference>;

  ShareableFileReference(ScopedFile scoped_file);
  ~ShareableFileReference();

  ScopedFile scoped_file_;

  DISALLOW_COPY_AND_ASSIGN(ShareableFileReference);
};

}  

#endif  
