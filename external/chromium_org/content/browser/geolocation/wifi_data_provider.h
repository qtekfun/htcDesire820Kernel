// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_BROWSER_GEOLOCATION_WIFI_DATA_PROVIDER_H_
#define CONTENT_BROWSER_GEOLOCATION_WIFI_DATA_PROVIDER_H_

#include <set>

#include "base/basictypes.h"
#include "base/bind.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/message_loop/message_loop.h"
#include "base/strings/string16.h"
#include "base/strings/string_util.h"
#include "content/browser/geolocation/wifi_data.h"
#include "content/common/content_export.h"

namespace content {

class WifiDataProvider;

class CONTENT_EXPORT WifiDataProviderImplBase
    : public base::RefCountedThreadSafe<WifiDataProviderImplBase> {
 public:
  WifiDataProviderImplBase();

  
  
  virtual void StartDataProvider() = 0;

  
  
  virtual void StopDataProvider() = 0;

  
  
  
  virtual bool GetData(WifiData* data) = 0;

  
  
  void SetContainer(WifiDataProvider* container);

  typedef base::Callback<void(WifiDataProvider*)> WifiDataUpdateCallback;

  void AddCallback(WifiDataUpdateCallback* callback);

  bool RemoveCallback(WifiDataUpdateCallback* callback);

  bool has_callbacks() const;

 protected:
  friend class base::RefCountedThreadSafe<WifiDataProviderImplBase>;
  virtual ~WifiDataProviderImplBase();

  typedef std::set<WifiDataUpdateCallback*> CallbackSet;

  
  
  void RunCallbacks();

  bool CalledOnClientThread() const;

  base::MessageLoop* client_loop() const;

 private:
  void DoRunCallbacks();

  WifiDataProvider* container_;

  
  
  base::MessageLoop* client_loop_;

  CallbackSet callbacks_;

  DISALLOW_COPY_AND_ASSIGN(WifiDataProviderImplBase);
};

class CONTENT_EXPORT WifiDataProvider {
 public:
  
  
  
  
  typedef WifiDataProviderImplBase* (*ImplFactoryFunction)(void);
  static void SetFactory(ImplFactoryFunction factory_function_in);

  
  static void ResetFactory();

  typedef base::Callback<void(WifiDataProvider*)> WifiDataUpdateCallback;

  
  
  static WifiDataProvider* Register(WifiDataUpdateCallback* callback);

  
  
  static bool Unregister(WifiDataUpdateCallback* callback);

  
  
  
  bool GetData(WifiData* data);

 private:
  
  
  WifiDataProvider();
  virtual ~WifiDataProvider();

  void AddCallback(WifiDataUpdateCallback* callback);
  bool RemoveCallback(WifiDataUpdateCallback* callback);
  bool has_callbacks() const;

  void StartDataProvider();
  void StopDataProvider();

  static WifiDataProviderImplBase* DefaultFactoryFunction();

  
  
  
  static WifiDataProvider* instance_;

  
  static ImplFactoryFunction factory_function_;

  
  scoped_refptr<WifiDataProviderImplBase> impl_;

  DISALLOW_COPY_AND_ASSIGN(WifiDataProvider);
};

}  

#endif  
