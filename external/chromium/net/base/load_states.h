// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_LOAD_STATES_H__
#define NET_BASE_LOAD_STATES_H__
#pragma once

namespace net {

enum LoadState {
  
  
  
  
  LOAD_STATE_IDLE,

  
  
  
  
  
  LOAD_STATE_WAITING_FOR_CACHE,

  
  
  
  
  LOAD_STATE_RESOLVING_PROXY_FOR_URL,

  
  
  LOAD_STATE_ESTABLISHING_PROXY_TUNNEL,

  
  
  
  
  LOAD_STATE_RESOLVING_HOST,

  
  
  
  LOAD_STATE_CONNECTING,

  
  
  LOAD_STATE_SSL_HANDSHAKE,

  
  
  
  
  LOAD_STATE_SENDING_REQUEST,

  
  
  
  
  LOAD_STATE_WAITING_FOR_RESPONSE,

  
  
  
  
  
  LOAD_STATE_READING_RESPONSE,
};

}  

#endif  
