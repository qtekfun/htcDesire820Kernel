// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_CONFIG_SERVICE_LINUX_H_
#define NET_PROXY_PROXY_CONFIG_SERVICE_LINUX_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/environment.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "net/base/net_export.h"
#include "net/proxy/proxy_config.h"
#include "net/proxy/proxy_config_service.h"
#include "net/proxy/proxy_server.h"

namespace base {
class MessageLoopForIO;
class SingleThreadTaskRunner;
}  

namespace net {

class NET_EXPORT_PRIVATE ProxyConfigServiceLinux : public ProxyConfigService {
 public:

  
  class Delegate;

  class SettingGetter {
   public:
    
    
    static const size_t BUFFER_SIZE = 512;

    SettingGetter() {}
    virtual ~SettingGetter() {}

    
    
    
    
    
    
    
    virtual bool Init(base::SingleThreadTaskRunner* glib_thread_task_runner,
                      base::MessageLoopForIO* file_loop) = 0;

    
    
    virtual void ShutDown() = 0;

    
    
    virtual bool SetUpNotifications(Delegate* delegate) = 0;

    
    
    
    virtual base::SingleThreadTaskRunner* GetNotificationTaskRunner() = 0;

    
    virtual ProxyConfigSource GetConfigSource() = 0;

    
    
    
    
    enum StringSetting {
      PROXY_MODE,
      PROXY_AUTOCONF_URL,
      PROXY_HTTP_HOST,
      PROXY_HTTPS_HOST,
      PROXY_FTP_HOST,
      PROXY_SOCKS_HOST,
    };
    enum BoolSetting {
      PROXY_USE_HTTP_PROXY,
      PROXY_USE_SAME_PROXY,
      PROXY_USE_AUTHENTICATION,
    };
    enum IntSetting {
      PROXY_HTTP_PORT,
      PROXY_HTTPS_PORT,
      PROXY_FTP_PORT,
      PROXY_SOCKS_PORT,
    };
    enum StringListSetting {
      PROXY_IGNORE_HOSTS,
    };

    
    static IntSetting HostSettingToPortSetting(StringSetting host) {
      switch (host) {
        case PROXY_HTTP_HOST:
          return PROXY_HTTP_PORT;
        case PROXY_HTTPS_HOST:
          return PROXY_HTTPS_PORT;
        case PROXY_FTP_HOST:
          return PROXY_FTP_PORT;
        case PROXY_SOCKS_HOST:
          return PROXY_SOCKS_PORT;
        default:
          NOTREACHED();
          return PROXY_HTTP_PORT;  
      }
    }

    
    
    
    
    virtual bool GetString(StringSetting key, std::string* result) = 0;
    
    virtual bool GetBool(BoolSetting key, bool* result) = 0;
    
    virtual bool GetInt(IntSetting key, int* result) = 0;
    
    virtual bool GetStringList(StringListSetting key,
                               std::vector<std::string>* result) = 0;

    
    
    virtual bool BypassListIsReversed() = 0;

    
    
    virtual bool MatchHostsUsingSuffixMatching() = 0;

   private:
    DISALLOW_COPY_AND_ASSIGN(SettingGetter);
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  class Delegate : public base::RefCountedThreadSafe<Delegate> {
   public:
    
    
    explicit Delegate(base::Environment* env_var_getter);
    
    
    Delegate(base::Environment* env_var_getter, SettingGetter* setting_getter);

    
    
    
    
    
    
    
    
    void SetUpAndFetchInitialConfig(
        base::SingleThreadTaskRunner* glib_thread_task_runner,
        base::SingleThreadTaskRunner* io_thread_task_runner,
        base::MessageLoopForIO* file_loop);

    
    
    
    
    
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

    
    
    
    bool GetProxyFromSettings(SettingGetter::StringSetting host_key,
                              ProxyServer* result_server);
    
    
    bool GetConfigFromSettings(ProxyConfig* config);

    
    
    void SetNewProxyConfig(const ProxyConfig& new_config);

    
    void SetUpNotifications();

    scoped_ptr<base::Environment> env_var_getter_;
    scoped_ptr<SettingGetter> setting_getter_;

    
    
    
    ProxyConfig cached_config_;

    
    
    
    ProxyConfig reference_config_;

    
    
    
    
    
    
    
    
    
    scoped_refptr<base::SingleThreadTaskRunner> glib_thread_task_runner_;
    
    
    scoped_refptr<base::SingleThreadTaskRunner> io_thread_task_runner_;

    ObserverList<Observer> observers_;

    DISALLOW_COPY_AND_ASSIGN(Delegate);
  };

  

  
  ProxyConfigServiceLinux();
  
  explicit ProxyConfigServiceLinux(base::Environment* env_var_getter);
  ProxyConfigServiceLinux(base::Environment* env_var_getter,
                          SettingGetter* setting_getter);

  virtual ~ProxyConfigServiceLinux();

  void SetupAndFetchInitialConfig(
      base::SingleThreadTaskRunner* glib_thread_task_runner,
      base::SingleThreadTaskRunner* io_thread_task_runner,
      base::MessageLoopForIO* file_loop) {
    delegate_->SetUpAndFetchInitialConfig(glib_thread_task_runner,
                                          io_thread_task_runner, file_loop);
  }
  void OnCheckProxyConfigSettings() {
    delegate_->OnCheckProxyConfigSettings();
  }

  
  
  virtual void AddObserver(Observer* observer) OVERRIDE;
  virtual void RemoveObserver(Observer* observer) OVERRIDE;
  virtual ProxyConfigService::ConfigAvailability GetLatestProxyConfig(
      ProxyConfig* config) OVERRIDE;

 private:
  scoped_refptr<Delegate> delegate_;

  DISALLOW_COPY_AND_ASSIGN(ProxyConfigServiceLinux);
};

}  

#endif  
