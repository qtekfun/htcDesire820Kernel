// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SYSTEM_MESSAGE_IN_TRANSIT_H_
#define MOJO_SYSTEM_MESSAGE_IN_TRANSIT_H_

#include <stdint.h>
#include <stdlib.h>  

#include "base/basictypes.h"
#include "mojo/system/system_impl_export.h"

namespace mojo {
namespace system {

class MOJO_SYSTEM_IMPL_EXPORT MessageInTransit {
 public:
  typedef uint16_t Type;
  
  static const Type kTypeMessagePipeEndpoint = 0;
  
  static const Type kTypeMessagePipe = 1;
  
  static const Type TYPE_CHANNEL = 2;

  typedef uint16_t Subtype;
  
  static const Subtype kSubtypeMessagePipeEndpointData = 0;
  
  static const Subtype kSubtypeMessagePipePeerClosed = 0;

  typedef uint32_t EndpointId;
  
  static const EndpointId kInvalidEndpointId = 0;

  
  
  static const size_t kMessageAlignment = 8;

  
  
  static MessageInTransit* Create(Type type, Subtype subtype,
                                  const void* bytes, uint32_t num_bytes);

  
  inline void Destroy() {
    
    free(this);
  }

  
  uint32_t data_size() const {
    return size_;
  }

  
  const void* data() const {
    return reinterpret_cast<const char*>(this) + sizeof(*this);
  }

  size_t size_with_header_and_padding() const {
    return RoundUpMessageAlignment(sizeof(*this) + size_);
  }

  Type type() const { return type_; }
  Subtype subtype() const { return subtype_; }
  EndpointId source_id() const { return source_id_; }
  EndpointId destination_id() const { return destination_id_; }

  void set_source_id(EndpointId source_id) { source_id_ = source_id; }
  void set_destination_id(EndpointId destination_id) {
    destination_id_ = destination_id;
  }

  

  
  static inline size_t RoundUpMessageAlignment(size_t n) {
    return (n + kMessageAlignment - 1) & ~(kMessageAlignment - 1);
  }

 private:
  explicit MessageInTransit(uint32_t size, Type type, Subtype subtype)
      : size_(size),
        type_(type),
        subtype_(subtype),
        source_id_(kInvalidEndpointId),
        destination_id_(kInvalidEndpointId) {}

  
  uint32_t size_;
  Type type_;
  Subtype subtype_;
  EndpointId source_id_;
  EndpointId destination_id_;

  
  
  ~MessageInTransit();

  DISALLOW_COPY_AND_ASSIGN(MessageInTransit);
};

COMPILE_ASSERT(sizeof(MessageInTransit) == 16, MessageInTransit_has_wrong_size);

}  
}  

#endif  
