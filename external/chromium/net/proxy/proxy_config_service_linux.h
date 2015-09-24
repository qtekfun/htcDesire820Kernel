// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_CONFIG_SERVICE_LINUX_H_
#define NET_PROXY_PROXY_CONFIG_SERVICE_LINUX_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/environment.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop.h"
#include "base/observer_list.h"
#include "net/proxy/proxy_config.h"
#include "net/proxy/proxy_config_service.h"
#include "net/proxy/proxy_server.h"

namespace net {

class ProxyConfigServiceLinux : public ProxyConfigService {
 public:

  
  class Delegate;

  class GConfSettingGetter {
   public:
    
    
    static const size_t BUFFER_SIZE = 512;

    GConfSettingGetter() {}
    virtual ~GConfSettingGetter() {}

    
    
    
    
    
    
    virtual bool Init(MessageLoop* glib_default_loop,
                      MessageLoopForIO* file_loop) = 0;

    
    
    virtual void Shutdown() = 0;

    
    
    virtual bool SetupNotification(Delegate* delegate) = 0;

    
    
    
    virtual MessageLoop* GetNotificationLoop() = 0;

    
    
    virtual const char* GetDataSource() = 0;

    
    
    
    
    
    virtual bool GetString(const char* key, std::string* result) = 0;
    
    virtual bool GetBoolean(const char* key, bool* result) = 0;
    
    virtual bool GetInt(const char* key, int* result) = 0;
    
    virtual bool GetStringList(const char* key,
                               std::vector<std::string>* result) = 0;

    
    
    virtual bool BypassListIsReversed() = 0;

    
    
    virtual bool MatchHostsUsingSuffixMatching() = 0;

   private:
    DISALLOW_COPY_AND_ASSIGN(GConfSettingGetter);
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  class Delegate : public base::RefCountedThreadSafe<Delegate> {
   public:
    
    
    explicit Delegate(base::Environment* env_var_getter);
    
    
    Delegate(base::Environment* env_var_getter,
             GConfSettingGetter* gconf_getter);
    
    
    
    
    
    
    
    
    void SetupAndFetchInitialConfig(MessageLoop* glib_default_loop,
                                    MessageLoop* io_loop,
                                    MessageLoopForIO* file_loop);

    
    
    
    
    
    void OnCheckProxyConfigSettings();

    
    void AddObserver(Observer* observer);
    void RemoveObserver(Observer* observer);
    ProxyConfigService::ConfigAvailability GetLatestProxyConfig(
        ProxyConfig* config);

    
    
    void PostDestroyTask();
    
    void OnDestroy();

   private:
    friend class base::RefCountedThreadSafe<Delegate>;

    ~Delegate();

    
    
    
    bool GetProxyFromEnvVarForScheme(const char* variable,
                                     ProxyServer::Scheme scheme,
                                     ProxyServer* result_server);
    
    bool GetProxyFromEnvVar(const char* variable, ProxyServer* result_server);
    
    
    bool GetConfigFromEnv(ProxyConfig* config);

    
    
    
    bool GetProxyFromGConf(const char* key_prefix, bool is_socks,
                           ProxyServer* result_server);
    
    
    bool GetConfigFromGConf(ProxyConfig* config);

    
    
    void SetNewProxyConfig(const ProxyConfig& new_config);

    scoped_ptr<base::Environment> env_var_getter_;
    scoped_ptr<GConfSettingGetter> gconf_getter_;

    
    
    
    ProxyConfig cached_config_;

    
    
    
    ProxyConfig reference_config_;

    
    
    
    
    
    
    
    
    
    MessageLoop* glib_default_loop_;
    
    
    MessageLoop* io_loop_;

    ObserverList<Observer> observers_;

    DISALLOW_COPY_AND_ASSIGN(Delegate);
  };

  

  
  ProxyConfigServiceLinux();
  
  explicit ProxyConfigServiceLinux(base::Environment* env_var_getter);
  ProxyConfigServiceLinux(base::Environment* env_var_getter,
                          GConfSettingGetter* gconf_getter);

  virtual ~ProxyConfigServiceLinux();

  void SetupAndFetchInitialConfig(MessageLoop* glib_default_loop,
                                  MessageLoop* io_loop,
                                  MessageLoopForIO* file_loop) {
    delegate_->SetupAndFetchInitialConfig(glib_default_loop, io_loop,
                                          file_loop);
  }
  void OnCheckProxyConfigSettings() {
    delegate_->OnCheckProxyConfigSettings();
  }

  
  
  virtual void AddObserver(Observer* observer);
  virtual void RemoveObserver(Observer* observer);
  virtual ProxyConfigService::ConfigAvailability GetLatestProxyConfig(
      ProxyConfig* config);

 private:
  scoped_refptr<Delegate> delegate_;

  DISALLOW_COPY_AND_ASSIGN(ProxyConfigServiceLinux);
};

}  

#endif  
