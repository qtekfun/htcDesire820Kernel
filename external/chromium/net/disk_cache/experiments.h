// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_EXPERIMENTS_H_
#define NET_DISK_CACHE_EXPERIMENTS_H_
#pragma once


namespace disk_cache {

enum {
  NO_EXPERIMENT = 0,
  EXPERIMENT_OLD_FILE1 = 3,
  EXPERIMENT_OLD_FILE2 = 4,
  EXPERIMENT_DELETED_LIST_OUT = 11,
  EXPERIMENT_DELETED_LIST_CONTROL = 12,
  EXPERIMENT_DELETED_LIST_IN = 13
};

}  

#endif  
