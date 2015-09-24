// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_SERVICE_COMMON_DECODER_H_
#define GPU_COMMAND_BUFFER_SERVICE_COMMON_DECODER_H_

#include <map>
#include <stack>
#include <string>
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "gpu/command_buffer/common/buffer.h"
#include "gpu/command_buffer/service/cmd_parser.h"
#include "gpu/gpu_export.h"

namespace gpu {

class CommandBufferEngine;

class GPU_EXPORT CommonDecoder : NON_EXPORTED_BASE(public AsyncAPIInterface) {
 public:
  typedef error::Error Error;

  static const unsigned int kMaxStackDepth = 32;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  class GPU_EXPORT Bucket {
   public:
    Bucket();
    ~Bucket();

    size_t size() const {
      return size_;
    }

    
    
    void* GetData(size_t offset, size_t size) const;

    template <typename T>
    T GetDataAs(size_t offset, size_t size) const {
      return reinterpret_cast<T>(GetData(offset, size));
    }

    
    void SetSize(size_t size);

    
    
    bool SetData(const void* src, size_t offset, size_t size);

    
    
    void SetFromString(const char* str);

    
    
    
    bool GetAsString(std::string* str);

   private:
    bool OffsetSizeValid(size_t offset, size_t size) const {
      size_t temp = offset + size;
      return temp <= size_ && temp >= offset;
    }

    size_t size_;
    ::scoped_ptr<int8[]> data_;

    DISALLOW_COPY_AND_ASSIGN(Bucket);
  };

  CommonDecoder();
  virtual ~CommonDecoder();

  
  
  void set_engine(CommandBufferEngine* engine) {
    engine_ = engine;
  }
  CommandBufferEngine* engine() const { return engine_; }

  
  Bucket* CreateBucket(uint32 bucket_id);

  
  Bucket* GetBucket(uint32 bucket_id) const;

  
  
  
  
  
  
  
  
  
  
  void* GetAddressAndCheckSize(unsigned int shm_id,
                               unsigned int offset,
                               unsigned int size);

  
  template <typename T>
  T GetSharedMemoryAs(unsigned int shm_id, unsigned int offset,
                      unsigned int size) {
    return static_cast<T>(GetAddressAndCheckSize(shm_id, offset, size));
  }

  
  Buffer GetSharedMemoryBuffer(unsigned int shm_id);

 protected:
  
  
  
  
  
  
  
  
  error::Error DoCommonCommand(
      unsigned int command,
      unsigned int arg_count,
      const void* cmd_data);

  
  const char* GetCommonCommandName(cmd::CommandId command_id) const;

 private:
  
  
  #define COMMON_COMMAND_BUFFER_CMD_OP(name)             \
     error::Error Handle##name(                          \
       uint32 immediate_data_size,                       \
       const cmd::name& args);                           \

  COMMON_COMMAND_BUFFER_CMDS(COMMON_COMMAND_BUFFER_CMD_OP)

  #undef COMMON_COMMAND_BUFFER_CMD_OP

  CommandBufferEngine* engine_;

  typedef std::map<uint32, linked_ptr<Bucket> > BucketMap;
  BucketMap buckets_;
};

}  

#endif  

