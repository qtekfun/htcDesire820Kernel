// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_SERVER_PROPERTIES_H_
#define NET_HTTP_HTTP_SERVER_PROPERTIES_H_

#include <map>
#include <string>
#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "net/base/host_port_pair.h"
#include "net/base/net_export.h"
#include "net/http/http_pipelined_host_capability.h"
#include "net/socket/next_proto.h"
#include "net/spdy/spdy_framer.h"  

namespace net {

enum AlternateProtocol {
  DEPRECATED_NPN_SPDY_2 = 0,
  ALTERNATE_PROTOCOL_MINIMUM_VALID_VERSION = DEPRECATED_NPN_SPDY_2,
  NPN_SPDY_MINIMUM_VERSION = DEPRECATED_NPN_SPDY_2,
  NPN_SPDY_3,
  NPN_SPDY_3_1,
  NPN_SPDY_4A2,
  
  NPN_HTTP2_DRAFT_04,
  NPN_SPDY_MAXIMUM_VERSION = NPN_HTTP2_DRAFT_04,
  QUIC,
  ALTERNATE_PROTOCOL_MAXIMUM_VALID_VERSION = QUIC,
  ALTERNATE_PROTOCOL_BROKEN,  
  UNINITIALIZED_ALTERNATE_PROTOCOL,
};

NET_EXPORT bool IsAlternateProtocolValid(AlternateProtocol protocol);

enum AlternateProtocolSize {
  NUM_VALID_ALTERNATE_PROTOCOLS =
    ALTERNATE_PROTOCOL_MAXIMUM_VALID_VERSION -
    ALTERNATE_PROTOCOL_MINIMUM_VALID_VERSION + 1,
};

NET_EXPORT const char* AlternateProtocolToString(AlternateProtocol protocol);
NET_EXPORT AlternateProtocol AlternateProtocolFromString(
    const std::string& str);
NET_EXPORT_PRIVATE AlternateProtocol AlternateProtocolFromNextProto(
    NextProto next_proto);

struct NET_EXPORT PortAlternateProtocolPair {
  bool Equals(const PortAlternateProtocolPair& other) const {
    return port == other.port && protocol == other.protocol;
  }

  std::string ToString() const;

  uint16 port;
  AlternateProtocol protocol;
};

typedef std::map<HostPortPair, PortAlternateProtocolPair> AlternateProtocolMap;
typedef std::map<HostPortPair, SettingsMap> SpdySettingsMap;
typedef std::map<HostPortPair,
        HttpPipelinedHostCapability> PipelineCapabilityMap;

extern const char kAlternateProtocolHeader[];

class NET_EXPORT HttpServerProperties {
 public:
  HttpServerProperties() {}
  virtual ~HttpServerProperties() {}

  
  virtual base::WeakPtr<HttpServerProperties> GetWeakPtr() = 0;

  
  virtual void Clear() = 0;

  
  virtual bool SupportsSpdy(const HostPortPair& server) const = 0;

  
  
  virtual void SetSupportsSpdy(const HostPortPair& server,
                               bool support_spdy) = 0;

  
  virtual bool HasAlternateProtocol(const HostPortPair& server) const = 0;

  
  
  virtual PortAlternateProtocolPair GetAlternateProtocol(
      const HostPortPair& server) const = 0;

  
  virtual void SetAlternateProtocol(const HostPortPair& server,
                                    uint16 alternate_port,
                                    AlternateProtocol alternate_protocol) = 0;

  
  virtual void SetBrokenAlternateProtocol(const HostPortPair& server) = 0;

  
  virtual const AlternateProtocolMap& alternate_protocol_map() const = 0;

  
  
  virtual const SettingsMap& GetSpdySettings(
      const HostPortPair& host_port_pair) const = 0;

  
  
  virtual bool SetSpdySetting(const HostPortPair& host_port_pair,
                              SpdySettingsIds id,
                              SpdySettingsFlags flags,
                              uint32 value) = 0;

  
  virtual void ClearSpdySettings(const HostPortPair& host_port_pair) = 0;

  
  virtual void ClearAllSpdySettings() = 0;

  
  virtual const SpdySettingsMap& spdy_settings_map() const = 0;

  virtual HttpPipelinedHostCapability GetPipelineCapability(
      const HostPortPair& origin) = 0;

  virtual void SetPipelineCapability(
      const HostPortPair& origin,
      HttpPipelinedHostCapability capability) = 0;

  virtual void ClearPipelineCapabilities() = 0;

  virtual PipelineCapabilityMap GetPipelineCapabilityMap() const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(HttpServerProperties);
};

}  

#endif  
