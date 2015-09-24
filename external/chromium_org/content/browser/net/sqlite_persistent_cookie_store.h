// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_BROWSER_NET_SQLITE_PERSISTENT_COOKIE_STORE_H_
#define CONTENT_BROWSER_NET_SQLITE_PERSISTENT_COOKIE_STORE_H_

#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"
#include "net/cookies/cookie_monster.h"

class Task;

namespace base {
class FilePath;
class SequencedTaskRunner;
}

namespace net {
class CanonicalCookie;
}

namespace quota {
class SpecialStoragePolicy;
}

namespace content {
class CookieCryptoDelegate;

class CONTENT_EXPORT SQLitePersistentCookieStore
    : public net::CookieMonster::PersistentCookieStore {
 public:
  
  
  
  SQLitePersistentCookieStore(
      const base::FilePath& path,
      const scoped_refptr<base::SequencedTaskRunner>& client_task_runner,
      const scoped_refptr<base::SequencedTaskRunner>& background_task_runner,
      bool restore_old_session_cookies,
      quota::SpecialStoragePolicy* special_storage_policy,
      scoped_ptr<CookieCryptoDelegate> crypto_delegate);

  
  virtual void Load(const LoadedCallback& loaded_callback) OVERRIDE;
  virtual void LoadCookiesForKey(const std::string& key,
      const LoadedCallback& callback) OVERRIDE;
  virtual void AddCookie(const net::CanonicalCookie& cc) OVERRIDE;
  virtual void UpdateCookieAccessTime(const net::CanonicalCookie& cc) OVERRIDE;
  virtual void DeleteCookie(const net::CanonicalCookie& cc) OVERRIDE;
  virtual void SetForceKeepSessionState() OVERRIDE;
  virtual void Flush(const base::Closure& callback) OVERRIDE;

 protected:
   virtual ~SQLitePersistentCookieStore();

 private:
  class Backend;

  scoped_refptr<Backend> backend_;

  DISALLOW_COPY_AND_ASSIGN(SQLitePersistentCookieStore);
};

}  

#endif  
