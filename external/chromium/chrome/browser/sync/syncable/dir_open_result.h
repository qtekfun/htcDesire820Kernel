// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_SYNCABLE_DIR_OPEN_RESULT_H_
#define CHROME_BROWSER_SYNC_SYNCABLE_DIR_OPEN_RESULT_H_
#pragma once

namespace syncable {
enum DirOpenResult { OPENED,   
                     FAILED_NEWER_VERSION,  
                     FAILED_MAKE_REPOSITORY,  
                     FAILED_OPEN_DATABASE,  
                     FAILED_DISK_FULL,  
                     FAILED_DATABASE_CORRUPT,  
};
}  

#endif  
