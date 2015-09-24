// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_SYSTEM_INFO_SYSTEM_INFO_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_SYSTEM_INFO_SYSTEM_INFO_API_H_

#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "extensions/browser/event_router.h"

namespace extensions {

class SystemInfoAPI : public ProfileKeyedAPI,
                      public EventRouter::Observer {
 public:
  
  static ProfileKeyedAPIFactory<SystemInfoAPI>* GetFactoryInstance();

  explicit SystemInfoAPI(Profile* profile);
  virtual ~SystemInfoAPI();

  
  virtual void Shutdown() OVERRIDE;

  
  virtual void OnListenerAdded(const EventListenerInfo& details) OVERRIDE;
  virtual void OnListenerRemoved(const EventListenerInfo& details) OVERRIDE;

 private:
  friend class ProfileKeyedAPIFactory<SystemInfoAPI>;

  
  static const char* service_name() {
    return "SystemInfoAPI";
  }
  static const bool kServiceIsNULLWhileTesting = true;

  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(SystemInfoAPI);
};

}  

#endif  
