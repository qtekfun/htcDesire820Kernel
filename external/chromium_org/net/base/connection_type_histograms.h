// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_CONNECTION_TYPE_HISTOGRAMS_H_
#define NET_BASE_CONNECTION_TYPE_HISTOGRAMS_H_


namespace net {

enum ConnectionType {
  CONNECTION_ANY = 0,      
  CONNECTION_SSL = 1,      
  
  
  
  
  
  CONNECTION_HTTP = 7,     
  CONNECTION_SPDY = 8,     
  CONNECTION_SSL_SSL2 = 9,     
  CONNECTION_SSL_SSL3 = 10,    
  CONNECTION_SSL_TLS1 = 11,    
  CONNECTION_SSL_TLS1_1 = 12,  
  CONNECTION_SSL_TLS1_2 = 13,  
  NUM_OF_CONNECTION_TYPES
};

void UpdateConnectionTypeHistograms(ConnectionType type);

}  

#endif  
