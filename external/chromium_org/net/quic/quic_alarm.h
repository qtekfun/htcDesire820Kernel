// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_ALARM_H_
#define NET_QUIC_QUIC_ALARM_H_

#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"
#include "net/quic/quic_time.h"

namespace net {

class NET_EXPORT_PRIVATE QuicAlarm {
 public:
  class NET_EXPORT_PRIVATE Delegate {
   public:
    virtual ~Delegate() {}

    
    
    
    virtual QuicTime OnAlarm() = 0;
  };

  explicit QuicAlarm(Delegate* delegate);
  virtual ~QuicAlarm();

  
  
  
  void Set(QuicTime deadline);

  
  
  
  
  void Cancel();

  bool IsSet() const;

  QuicTime deadline() const { return deadline_; }

 protected:
  
  
  
  virtual void SetImpl() = 0;

  
  
  virtual void CancelImpl() = 0;

  
  
  
  
  
  
  
  void Fire();

 private:
  scoped_ptr<Delegate> delegate_;
  QuicTime deadline_;

  DISALLOW_COPY_AND_ASSIGN(QuicAlarm);
};

}  

#endif  
