// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_SYNCABLE_DIR_OPEN_RESULT_H_
#define SYNC_SYNCABLE_DIR_OPEN_RESULT_H_

namespace syncer {
namespace syncable {
enum DirOpenResult { NOT_INITIALIZED,
                     OPENED,   
                     FAILED_NEWER_VERSION,  
                     FAILED_MAKE_REPOSITORY,  
                     FAILED_OPEN_DATABASE,  
                     FAILED_DISK_FULL,  
                     FAILED_DATABASE_CORRUPT,  
                     FAILED_LOGICAL_CORRUPTION, 
                     FAILED_IN_UNITTEST, 
                     FAILED_INITIAL_WRITE,  
};
}  
}  

#endif  
