// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_STATE_STORE_H_
#define CHROME_BROWSER_EXTENSIONS_STATE_STORE_H_

#include <set>
#include <string>

#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/value_store/value_store_frontend.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class Profile;

namespace extensions {

class StateStore
    : public base::SupportsWeakPtr<StateStore>,
      public content::NotificationObserver {
 public:
  typedef ValueStoreFrontend::ReadCallback ReadCallback;

  
  
  StateStore(Profile* profile, const base::FilePath& db_path,
             bool deferred_load);
  
  StateStore(Profile* profile, scoped_ptr<ValueStore> store);
  virtual ~StateStore();

  
  
  void RegisterKey(const std::string& key);

  
  
  void GetExtensionValue(const std::string& extension_id,
                         const std::string& key,
                         ReadCallback callback);

  
  void SetExtensionValue(const std::string& extension_id,
                         const std::string& key,
                         scoped_ptr<base::Value> value);

  
  void RemoveExtensionValue(const std::string& extension_id,
                            const std::string& key);

 private:
  class DelayedTaskQueue;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  void Init();

  
  void RemoveKeysForExtension(const std::string& extension_id);

  
  base::FilePath db_path_;

  
  ValueStoreFrontend store_;

  
  
  std::set<std::string> registered_keys_;

  
  scoped_ptr<DelayedTaskQueue> task_queue_;

  content::NotificationRegistrar registrar_;
};

}  

#endif  
