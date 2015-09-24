// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_ENGINE_SYNCER_TYPES_H_
#define SYNC_ENGINE_SYNCER_TYPES_H_

namespace syncer {

enum UpdateAttemptResponse {
  
  SUCCESS,

  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  
  
  
  CONFLICT_ENCRYPTION,

  
  
  
  CONFLICT_HIERARCHY,

  
  
  
  CONFLICT_SIMPLE
};

enum VerifyResult {
  VERIFY_FAIL,
  VERIFY_SUCCESS,
  VERIFY_UNDELETE,
  VERIFY_SKIP,
  VERIFY_UNDECIDED
};

enum VerifyCommitResult {
  VERIFY_UNSYNCABLE,
  VERIFY_OK,
};

}  

#endif  
