// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_JINGLE_GLUE_NETWORK_SETTINGS_H_
#define REMOTING_JINGLE_GLUE_NETWORK_SETTINGS_H_

namespace remoting {

struct NetworkSettings {

  
  
  
  static const int kDefaultMinPort = 12400;
  static const int kDefaultMaxPort = 12409;

  enum NatTraversalMode {
    
    NAT_TRAVERSAL_ENABLED,

    
    
    
    
    NAT_TRAVERSAL_DISABLED,

    
    NAT_TRAVERSAL_OUTGOING,
  };

  NetworkSettings()
      : nat_traversal_mode(NAT_TRAVERSAL_DISABLED),
        min_port(0),
        max_port(0) {
  }

  explicit NetworkSettings(NatTraversalMode nat_traversal_mode)
      : nat_traversal_mode(nat_traversal_mode),
        min_port(0),
        max_port(0) {
  }

  NatTraversalMode nat_traversal_mode;

  
  
  int min_port;
  int max_port;
};

}  

#endif  
