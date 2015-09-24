// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_BLOB_BLOB_STORAGE_HOST_H_
#define WEBKIT_BROWSER_BLOB_BLOB_STORAGE_HOST_H_

#include <map>
#include <set>
#include <string>

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "webkit/browser/webkit_storage_browser_export.h"
#include "webkit/common/blob/blob_data.h"

class GURL;

namespace webkit_blob {

class BlobDataHandle;
class BlobStorageHost;
class BlobStorageContext;

class WEBKIT_STORAGE_BROWSER_EXPORT BlobStorageHost {
 public:
  explicit BlobStorageHost(BlobStorageContext* context);
  ~BlobStorageHost();

  
  
  
  bool StartBuildingBlob(const std::string& uuid) WARN_UNUSED_RESULT;
  bool AppendBlobDataItem(const std::string& uuid,
                          const BlobData::Item& data_item) WARN_UNUSED_RESULT;
  bool CancelBuildingBlob(const std::string& uuid) WARN_UNUSED_RESULT;
  bool FinishBuildingBlob(const std::string& uuid,
                          const std::string& type) WARN_UNUSED_RESULT;
  bool IncrementBlobRefCount(const std::string& uuid) WARN_UNUSED_RESULT;
  bool DecrementBlobRefCount(const std::string& uuid) WARN_UNUSED_RESULT;
  bool RegisterPublicBlobURL(const GURL& blob_url,
                             const std::string& uuid) WARN_UNUSED_RESULT;
  bool RevokePublicBlobURL(const GURL& blob_url) WARN_UNUSED_RESULT;

 private:
  typedef std::map<std::string, int> BlobReferenceMap;

  bool IsInUseInHost(const std::string& uuid);
  bool IsBeingBuiltInHost(const std::string& uuid);
  bool IsUrlRegisteredInHost(const GURL& blob_url);

  
  
  BlobReferenceMap blobs_inuse_map_;

  
  std::set<GURL> public_blob_urls_;

  base::WeakPtr<BlobStorageContext> context_;

  DISALLOW_COPY_AND_ASSIGN(BlobStorageHost);
};

}  

#endif  
