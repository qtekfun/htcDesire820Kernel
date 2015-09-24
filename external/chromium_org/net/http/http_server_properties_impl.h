// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_SERVER_PROPERTIES_IMPL_H_
#define NET_HTTP_HTTP_SERVER_PROPERTIES_IMPL_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/containers/mru_cache.h"
#include "base/gtest_prod_util.h"
#include "base/threading/non_thread_safe.h"
#include "base/values.h"
#include "net/base/host_port_pair.h"
#include "net/base/net_export.h"
#include "net/http/http_pipelined_host_capability.h"
#include "net/http/http_server_properties.h"

namespace base {
class ListValue;
}

namespace net {

class NET_EXPORT HttpServerPropertiesImpl
    : public HttpServerProperties,
      NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  HttpServerPropertiesImpl();
  virtual ~HttpServerPropertiesImpl();

  
  
  void InitializeSpdyServers(std::vector<std::string>* spdy_servers,
                             bool support_spdy);

  void InitializeAlternateProtocolServers(
      AlternateProtocolMap* alternate_protocol_servers);

  void InitializeSpdySettingsServers(SpdySettingsMap* spdy_settings_map);

  
  
  void InitializePipelineCapabilities(
      const PipelineCapabilityMap* pipeline_capability_map);

  
  void GetSpdyServerList(base::ListValue* spdy_server_list) const;

  
  
  static std::string GetFlattenedSpdyServer(
      const net::HostPortPair& host_port_pair);

  
  
  
  static void ForceAlternateProtocol(const PortAlternateProtocolPair& pair);
  static void DisableForcedAlternateProtocol();

  
  
  
  
  void SetNumPipelinedHostsToRemember(int max_size);

  
  
  

  
  virtual base::WeakPtr<HttpServerProperties> GetWeakPtr() OVERRIDE;

  
  virtual void Clear() OVERRIDE;

  
  virtual bool SupportsSpdy(const HostPortPair& server) const OVERRIDE;

  
  virtual void SetSupportsSpdy(const HostPortPair& server,
                               bool support_spdy) OVERRIDE;

  
  virtual bool HasAlternateProtocol(const HostPortPair& server) const OVERRIDE;

  
  
  virtual PortAlternateProtocolPair GetAlternateProtocol(
      const HostPortPair& server) const OVERRIDE;

  
  virtual void SetAlternateProtocol(
      const HostPortPair& server,
      uint16 alternate_port,
      AlternateProtocol alternate_protocol) OVERRIDE;

  
  virtual void SetBrokenAlternateProtocol(const HostPortPair& server) OVERRIDE;

  
  virtual const AlternateProtocolMap& alternate_protocol_map() const OVERRIDE;

  
  
  virtual const SettingsMap& GetSpdySettings(
      const HostPortPair& host_port_pair) const OVERRIDE;

  
  
  virtual bool SetSpdySetting(const HostPortPair& host_port_pair,
                              SpdySettingsIds id,
                              SpdySettingsFlags flags,
                              uint32 value) OVERRIDE;

  
  virtual void ClearSpdySettings(const HostPortPair& host_port_pair) OVERRIDE;

  
  virtual void ClearAllSpdySettings() OVERRIDE;

  
  virtual const SpdySettingsMap& spdy_settings_map() const OVERRIDE;

  virtual HttpPipelinedHostCapability GetPipelineCapability(
      const HostPortPair& origin) OVERRIDE;

  virtual void SetPipelineCapability(
      const HostPortPair& origin,
      HttpPipelinedHostCapability capability) OVERRIDE;

  virtual void ClearPipelineCapabilities() OVERRIDE;

  virtual PipelineCapabilityMap GetPipelineCapabilityMap() const OVERRIDE;

 private:
  typedef base::MRUCache<
      HostPortPair, HttpPipelinedHostCapability> CachedPipelineCapabilityMap;
  
  
  typedef base::hash_map<std::string, bool> SpdyServerHostPortTable;

  SpdyServerHostPortTable spdy_servers_table_;

  AlternateProtocolMap alternate_protocol_map_;
  SpdySettingsMap spdy_settings_map_;
  scoped_ptr<CachedPipelineCapabilityMap> pipeline_capability_map_;

  base::WeakPtrFactory<HttpServerPropertiesImpl> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(HttpServerPropertiesImpl);
};

}  

#endif  
