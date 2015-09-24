// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GCM_BASE_MCS_MESSAGE_H_
#define GOOGLE_APIS_GCM_BASE_MCS_MESSAGE_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "google_apis/gcm/base/gcm_export.h"

namespace google {
namespace protobuf {
class MessageLite;
}  
}  

namespace gcm {

class GCM_EXPORT MCSMessage {
 public:
  
  MCSMessage();
  
  explicit MCSMessage(const google::protobuf::MessageLite& protobuf);
  
  MCSMessage(uint8 tag, const google::protobuf::MessageLite& protobuf);
  
  MCSMessage(uint8 tag,
             scoped_ptr<const google::protobuf::MessageLite> protobuf);
  ~MCSMessage();

  
  
  bool IsValid() const;

  
  uint8 tag() const { return tag_; }
  int size() const {return size_; }
  std::string SerializeAsString() const;

  
  const google::protobuf::MessageLite& GetProtobuf() const;

  
  scoped_ptr<google::protobuf::MessageLite> CloneProtobuf() const;

 private:
  class Core : public base::RefCountedThreadSafe<MCSMessage::Core> {
   public:
    Core();
    Core(uint8 tag, const google::protobuf::MessageLite& protobuf);
    Core(uint8 tag, scoped_ptr<const google::protobuf::MessageLite> protobuf);

    const google::protobuf::MessageLite& Get() const;

   private:
    friend class base::RefCountedThreadSafe<MCSMessage::Core>;
    ~Core();

    
    scoped_ptr<const google::protobuf::MessageLite> protobuf_;

    DISALLOW_COPY_AND_ASSIGN(Core);
  };

  
  const uint8 tag_;
  const int size_;

  
  scoped_refptr<const Core> core_;
};

}  

#endif  
