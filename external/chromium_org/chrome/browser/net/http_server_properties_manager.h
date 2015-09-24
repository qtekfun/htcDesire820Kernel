// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_HTTP_SERVER_PROPERTIES_MANAGER_H_
#define CHROME_BROWSER_NET_HTTP_SERVER_PROPERTIES_MANAGER_H_

#include <string>
#include <vector>
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/timer/timer.h"
#include "base/values.h"
#include "net/base/host_port_pair.h"
#include "net/http/http_pipelined_host_capability.h"
#include "net/http/http_server_properties.h"
#include "net/http/http_server_properties_impl.h"

class PrefService;

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace chrome_browser_net {


class HttpServerPropertiesManager
    : public net::HttpServerProperties {
 public:
  
  
  
  explicit HttpServerPropertiesManager(PrefService* pref_service);
  virtual ~HttpServerPropertiesManager();

  
  
  
  void InitializeOnIOThread();

  
  void ShutdownOnUIThread();

  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  static void SetVersion(base::DictionaryValue* http_server_properties_dict,
                         int version_number);

  
  
  void Clear(const base::Closure& completion);

  
  
  

  
  virtual base::WeakPtr<net::HttpServerProperties> GetWeakPtr() OVERRIDE;

  
  virtual void Clear() OVERRIDE;

  
  
  virtual bool SupportsSpdy(const net::HostPortPair& server) const OVERRIDE;

  
  
  virtual void SetSupportsSpdy(const net::HostPortPair& server,
                               bool support_spdy) OVERRIDE;

  
  virtual bool HasAlternateProtocol(
      const net::HostPortPair& server) const OVERRIDE;

  
  
  virtual net::PortAlternateProtocolPair GetAlternateProtocol(
      const net::HostPortPair& server) const OVERRIDE;

  
  virtual void SetAlternateProtocol(
      const net::HostPortPair& server,
      uint16 alternate_port,
      net::AlternateProtocol alternate_protocol) OVERRIDE;

  
  virtual void SetBrokenAlternateProtocol(
      const net::HostPortPair& server) OVERRIDE;

  
  virtual const net::AlternateProtocolMap&
      alternate_protocol_map() const OVERRIDE;

  
  
  virtual const net::SettingsMap& GetSpdySettings(
      const net::HostPortPair& host_port_pair) const OVERRIDE;

  
  
  virtual bool SetSpdySetting(const net::HostPortPair& host_port_pair,
                              net::SpdySettingsIds id,
                              net::SpdySettingsFlags flags,
                              uint32 value) OVERRIDE;

  
  virtual void ClearSpdySettings(
      const net::HostPortPair& host_port_pair) OVERRIDE;

  
  virtual void ClearAllSpdySettings() OVERRIDE;

  
  virtual const net::SpdySettingsMap& spdy_settings_map() const OVERRIDE;

  virtual net::HttpPipelinedHostCapability GetPipelineCapability(
      const net::HostPortPair& origin) OVERRIDE;

  virtual void SetPipelineCapability(
      const net::HostPortPair& origin,
      net::HttpPipelinedHostCapability capability) OVERRIDE;

  virtual void ClearPipelineCapabilities() OVERRIDE;

  virtual net::PipelineCapabilityMap GetPipelineCapabilityMap() const OVERRIDE;

 protected:
  
  

  
  
  
  void ScheduleUpdateCacheOnUI();

  
  
  virtual void StartCacheUpdateTimerOnUI(base::TimeDelta delay);

  
  
  
  virtual void UpdateCacheFromPrefsOnUI();

  
  
  void UpdateCacheFromPrefsOnIO(
      std::vector<std::string>* spdy_servers,
      net::SpdySettingsMap* spdy_settings_map,
      net::AlternateProtocolMap* alternate_protocol_map,
      net::PipelineCapabilityMap* pipeline_capability_map,
      bool detected_corrupted_prefs);

  
  
  
  void ScheduleUpdatePrefsOnIO();

  
  
  virtual void StartPrefsUpdateTimerOnIO(base::TimeDelta delay);

  
  
  
  void UpdatePrefsFromCacheOnIO();

  
  
  virtual void UpdatePrefsFromCacheOnIO(const base::Closure& completion);

  
  
  void UpdatePrefsOnUI(
      base::ListValue* spdy_server_list,
      net::SpdySettingsMap* spdy_settings_map,
      net::AlternateProtocolMap* alternate_protocol_map,
      net::PipelineCapabilityMap* pipeline_capability_map,
      const base::Closure& completion);

 private:
  void OnHttpServerPropertiesChanged();

  
  
  

  
  scoped_ptr<base::WeakPtrFactory<HttpServerPropertiesManager> >
      ui_weak_ptr_factory_;

  base::WeakPtr<HttpServerPropertiesManager> ui_weak_ptr_;

  
  scoped_ptr<base::OneShotTimer<HttpServerPropertiesManager> >
      ui_cache_update_timer_;

  
  PrefChangeRegistrar pref_change_registrar_;
  PrefService* pref_service_;  
  bool setting_prefs_;

  
  
  

  
  scoped_ptr<base::WeakPtrFactory<HttpServerPropertiesManager> >
      io_weak_ptr_factory_;

  
  scoped_ptr<base::OneShotTimer<HttpServerPropertiesManager> >
      io_prefs_update_timer_;

  scoped_ptr<net::HttpServerPropertiesImpl> http_server_properties_impl_;

  DISALLOW_COPY_AND_ASSIGN(HttpServerPropertiesManager);
};

}  

#endif  
