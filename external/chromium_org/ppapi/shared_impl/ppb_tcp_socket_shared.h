// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_PPB_TCP_SOCKET_SHARED_H_
#define PPAPI_SHARED_IMPL_PPB_TCP_SOCKET_SHARED_H_

#include "ppapi/shared_impl/ppapi_shared_export.h"

namespace ppapi {

class PPAPI_SHARED_EXPORT TCPSocketState {
 public:
  enum StateType {
    
    INITIAL,
    
    BOUND,
    
    CONNECTED,
    
    SSL_CONNECTED,
    
    LISTENING,
    
    CLOSED
  };

  
  
  
  enum TransitionType {
    NONE,
    BIND,
    CONNECT,
    SSL_CONNECT,
    LISTEN,
    CLOSE
  };

  explicit TCPSocketState(StateType state);
  ~TCPSocketState();

  StateType state() const { return state_; }

  void SetPendingTransition(TransitionType pending_transition);
  void CompletePendingTransition(bool success);

  void DoTransition(TransitionType transition, bool success);

  bool IsValidTransition(TransitionType transition) const;
  bool IsPending(TransitionType transition) const;

  bool IsConnected() const;
  bool IsBound() const;

 private:
  StateType state_;
  TransitionType pending_transition_;
};

enum PPAPI_SHARED_EXPORT TCPSocketVersion {
  
  TCP_SOCKET_VERSION_PRIVATE,
  
  TCP_SOCKET_VERSION_1_0,
  
  TCP_SOCKET_VERSION_1_1_OR_ABOVE
};

}  

#endif  
