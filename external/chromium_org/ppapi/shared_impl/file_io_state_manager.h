// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_FILE_IO_STATE_MANAGER_H_
#define PPAPI_SHARED_IMPL_FILE_IO_STATE_MANAGER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"

namespace ppapi {

class PPAPI_SHARED_EXPORT FileIOStateManager {
 public:
  FileIOStateManager();
  ~FileIOStateManager();

  enum OperationType {
    
    OPERATION_NONE,

    
    
    OPERATION_READ,

    
    
    OPERATION_WRITE,

    
    
    OPERATION_EXCLUSIVE
  };

  OperationType get_pending_operation() const { return pending_op_; }

  void SetOpenSucceed();

  
  
  
  
  
  int32_t CheckOperationState(OperationType new_op, bool should_be_open);

  
  void SetPendingOperation(OperationType op);
  void SetOperationFinished();

 private:
  int num_pending_ops_;
  OperationType pending_op_;

  
  bool file_open_;

  DISALLOW_COPY_AND_ASSIGN(FileIOStateManager);
};

}  

#endif  

