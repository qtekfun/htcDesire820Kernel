// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DIAL_DIAL_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_DIAL_DIAL_API_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/api/api_function.h"
#include "chrome/browser/extensions/api/dial/dial_device_data.h"
#include "chrome/browser/extensions/api/dial/dial_registry.h"
#include "components/browser_context_keyed_service/refcounted_browser_context_keyed_service.h"
#include "extensions/browser/event_router.h"

namespace extensions {

class DialRegistry;

class DialAPI : public RefcountedBrowserContextKeyedService,
                public EventRouter::Observer,
                public DialRegistry::Observer {
 public:
  explicit DialAPI(Profile* profile);

  
  
  DialRegistry* dial_registry();

  
  
  void SendEventOnUIThread(const DialRegistry::DeviceList& devices);
  void SendErrorOnUIThread(const DialRegistry::DialErrorCode type);

 private:
  virtual ~DialAPI();

  
  virtual void ShutdownOnUIThread() OVERRIDE;

  
  virtual void OnListenerAdded(const EventListenerInfo& details) OVERRIDE;
  virtual void OnListenerRemoved(const EventListenerInfo& details) OVERRIDE;

  
  virtual void OnDialDeviceEvent(
      const DialRegistry::DeviceList& devices) OVERRIDE;
  virtual void OnDialError(DialRegistry::DialErrorCode type) OVERRIDE;

  
  
  void NotifyListenerAddedOnIOThread();
  void NotifyListenerRemovedOnIOThread();

  Profile* profile_;

  
  scoped_ptr<DialRegistry> dial_registry_;

  DISALLOW_COPY_AND_ASSIGN(DialAPI);
};

namespace api {

class DialDiscoverNowFunction : public AsyncApiFunction {
 public:
  DialDiscoverNowFunction();

 protected:
  virtual ~DialDiscoverNowFunction() {}

  
  virtual bool Prepare() OVERRIDE;
  virtual void Work() OVERRIDE;
  virtual bool Respond() OVERRIDE;

 private:
  DECLARE_EXTENSION_FUNCTION("dial.discoverNow", DIAL_DISCOVERNOW)

  
  DialAPI* dial_;

  
  
  
  bool result_;

  DISALLOW_COPY_AND_ASSIGN(DialDiscoverNowFunction);
};

}  

}  

#endif  
