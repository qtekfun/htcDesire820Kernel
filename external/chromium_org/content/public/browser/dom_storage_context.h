// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_DOM_STORAGE_CONTEXT_H_
#define CONTENT_PUBLIC_BROWSER_DOM_STORAGE_CONTEXT_H_

#include <string>
#include <vector>

#include "base/callback.h"

class GURL;

namespace content {

class BrowserContext;
struct LocalStorageUsageInfo;
class SessionStorageNamespace;
struct SessionStorageUsageInfo;

class DOMStorageContext {
 public:
  typedef base::Callback<
      void(const std::vector<LocalStorageUsageInfo>&)>
          GetLocalStorageUsageCallback;

  typedef base::Callback<
      void(const std::vector<SessionStorageUsageInfo>&)>
          GetSessionStorageUsageCallback;

  
  virtual void GetLocalStorageUsage(
      const GetLocalStorageUsageCallback& callback) = 0;

  
  
  virtual void GetSessionStorageUsage(
      const GetSessionStorageUsageCallback& callback) = 0;

  
  virtual void DeleteLocalStorage(const GURL& origin) = 0;

  
  virtual void DeleteSessionStorage(
      const SessionStorageUsageInfo& usage_info) = 0;

  
  
  
  
  virtual void SetSaveSessionStorageOnDisk() = 0;

  
  
  
  
  virtual scoped_refptr<SessionStorageNamespace> RecreateSessionStorage(
      const std::string& persistent_id) = 0;

  
  
  
  virtual void StartScavengingUnusedSessionStorage() = 0;

 protected:
  virtual ~DOMStorageContext() {}
};

}  

#endif  
