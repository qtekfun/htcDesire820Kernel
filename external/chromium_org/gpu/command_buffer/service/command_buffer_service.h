// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_SERVICE_COMMAND_BUFFER_SERVICE_H_
#define GPU_COMMAND_BUFFER_SERVICE_COMMAND_BUFFER_SERVICE_H_

#include "base/callback.h"
#include "base/memory/shared_memory.h"
#include "gpu/command_buffer/common/command_buffer.h"
#include "gpu/command_buffer/common/command_buffer_shared.h"

namespace gpu {

class TransferBufferManagerInterface;

class GPU_EXPORT CommandBufferService : public CommandBuffer {
 public:
  typedef base::Callback<bool(int32)> GetBufferChangedCallback;
  explicit CommandBufferService(
      TransferBufferManagerInterface* transfer_buffer_manager);
  virtual ~CommandBufferService();

  
  virtual bool Initialize() OVERRIDE;
  virtual State GetState() OVERRIDE;
  virtual State GetLastState() OVERRIDE;
  virtual int32 GetLastToken() OVERRIDE;
  virtual void Flush(int32 put_offset) OVERRIDE;
  virtual State FlushSync(int32 put_offset, int32 last_known_get) OVERRIDE;
  virtual void SetGetBuffer(int32 transfer_buffer_id) OVERRIDE;
  virtual void SetGetOffset(int32 get_offset) OVERRIDE;
  virtual Buffer CreateTransferBuffer(size_t size, int32* id) OVERRIDE;
  virtual void DestroyTransferBuffer(int32 id) OVERRIDE;
  virtual Buffer GetTransferBuffer(int32 id) OVERRIDE;
  virtual void SetToken(int32 token) OVERRIDE;
  virtual void SetParseError(error::Error error) OVERRIDE;
  virtual void SetContextLostReason(error::ContextLostReason) OVERRIDE;

  
  
  
  
  
  
  
  
  virtual void SetPutOffsetChangeCallback(const base::Closure& callback);
  
  virtual void SetGetBufferChangeCallback(
      const GetBufferChangedCallback& callback);
  virtual void SetParseErrorCallback(const base::Closure& callback);

  
  bool SetSharedStateBuffer(scoped_ptr<base::SharedMemory> shared_state_shm);

  
  void UpdateState();

  
  
  
  bool RegisterTransferBuffer(int32 id,
                              base::SharedMemory* shared_memory,
                              size_t size);

 private:
  int32 ring_buffer_id_;
  Buffer ring_buffer_;
  scoped_ptr<base::SharedMemory> shared_state_shm_;
  CommandBufferSharedState* shared_state_;
  int32 num_entries_;
  int32 get_offset_;
  int32 put_offset_;
  base::Closure put_offset_change_callback_;
  GetBufferChangedCallback get_buffer_change_callback_;
  base::Closure parse_error_callback_;
  TransferBufferManagerInterface* transfer_buffer_manager_;
  int32 token_;
  uint32 generation_;
  error::Error error_;
  error::ContextLostReason context_lost_reason_;

  DISALLOW_COPY_AND_ASSIGN(CommandBufferService);
};

}  

#endif  
