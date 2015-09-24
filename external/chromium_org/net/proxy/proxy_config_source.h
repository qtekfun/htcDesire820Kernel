// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_CONFIG_SOURCE_H_
#define NET_PROXY_PROXY_CONFIG_SOURCE_H_

namespace net {


enum ProxyConfigSource {
  PROXY_CONFIG_SOURCE_UNKNOWN,       
  PROXY_CONFIG_SOURCE_SYSTEM,        
  PROXY_CONFIG_SOURCE_SYSTEM_FAILED, 
                                     
  PROXY_CONFIG_SOURCE_GCONF,         
  PROXY_CONFIG_SOURCE_GSETTINGS,     
  PROXY_CONFIG_SOURCE_KDE,           
  PROXY_CONFIG_SOURCE_ENV,           
  PROXY_CONFIG_SOURCE_CUSTOM,        
                                     
                                     
                                     
  PROXY_CONFIG_SOURCE_TEST,          
  NUM_PROXY_CONFIG_SOURCES
};

const char* ProxyConfigSourceToString(ProxyConfigSource source);

}  

#endif  
