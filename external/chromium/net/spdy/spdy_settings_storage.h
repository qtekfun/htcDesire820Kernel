// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_SETTING_STORAGE_H_
#define NET_SPDY_SPDY_SETTING_STORAGE_H_
#pragma once

#include <map>
#include "base/basictypes.h"
#include "net/base/host_port_pair.h"
#include "net/spdy/spdy_framer.h"

namespace net {

class SpdySettingsStorage {
 public:
  SpdySettingsStorage();
  ~SpdySettingsStorage();

  
  
  const spdy::SpdySettings& Get(const HostPortPair& host_port_pair) const;

  
  void Set(const HostPortPair& host_port_pair,
           const spdy::SpdySettings& settings);

 private:
  typedef std::map<HostPortPair, spdy::SpdySettings> SettingsMap;

  SettingsMap settings_map_;

  DISALLOW_COPY_AND_ASSIGN(SpdySettingsStorage);
};

}  

#endif  

