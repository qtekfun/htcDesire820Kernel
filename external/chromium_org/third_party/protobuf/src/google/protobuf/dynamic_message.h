// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_DYNAMIC_MESSAGE_H__
#define GOOGLE_PROTOBUF_DYNAMIC_MESSAGE_H__

#include <google/protobuf/message.h>
#include <google/protobuf/stubs/common.h>

namespace google {
namespace protobuf {

class Descriptor;        
class DescriptorPool;    

class LIBPROTOBUF_EXPORT DynamicMessageFactory : public MessageFactory {
 public:
  
  
  DynamicMessageFactory();

  
  
  
  
  
  
  
  DynamicMessageFactory(const DescriptorPool* pool);

  ~DynamicMessageFactory();

  
  
  
  
  
  
  void SetDelegateToGeneratedFactory(bool enable) {
    delegate_to_generated_factory_ = enable;
  }

  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  const Message* GetPrototype(const Descriptor* type);

 private:
  const DescriptorPool* pool_;
  bool delegate_to_generated_factory_;

  
  
  
  
  
  struct PrototypeMap;
  scoped_ptr<PrototypeMap> prototypes_;
  mutable Mutex prototypes_mutex_;

  friend class DynamicMessage;
  const Message* GetPrototypeNoLock(const Descriptor* type);

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(DynamicMessageFactory);
};

}  

}  
#endif  
