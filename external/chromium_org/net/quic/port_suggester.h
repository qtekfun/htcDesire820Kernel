// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_PORT_SUGGESTER_H_
#define NET_QUIC_PORT_SUGGESTER_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/sha1.h"
#include "net/base/net_export.h"

namespace net {

class HostPortPair;

class NET_EXPORT_PRIVATE PortSuggester
    : public base::RefCounted<PortSuggester> {
 public:
  PortSuggester(const HostPortPair& server, uint64 seed);

  
  
  int SuggestPort(int min, int max);

  uint32 call_count() const { return call_count_; }
  int previous_suggestion() const;

 private:
  friend class base::RefCounted<PortSuggester>;

  virtual ~PortSuggester() {}

  
  uint64 seed_;
  uint32 call_count_;  
  int previous_suggestion_;

  DISALLOW_COPY_AND_ASSIGN(PortSuggester);
};

}  

#endif  
