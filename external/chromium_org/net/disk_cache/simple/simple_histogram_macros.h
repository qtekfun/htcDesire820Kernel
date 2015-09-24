// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_SIMPLE_SIMPLE_HISTOGRAM_MACROS_H_
#define NET_DISK_CACHE_SIMPLE_SIMPLE_HISTOGRAM_MACROS_H_

#include "base/metrics/histogram.h"
#include "base/metrics/sparse_histogram.h"
#include "net/base/cache_type.h"


#define SIMPLE_CACHE_THUNK(uma_type, args) UMA_HISTOGRAM_##uma_type args

#define SIMPLE_CACHE_UMA(uma_type, uma_name, cache_type, ...)          \
  do {                                                                 \
    switch (cache_type) {                                              \
      case net::DISK_CACHE:                                            \
        SIMPLE_CACHE_THUNK(                                            \
            uma_type, ("SimpleCache.Http." uma_name, ##__VA_ARGS__));  \
        break;                                                         \
      case net::APP_CACHE:                                             \
        SIMPLE_CACHE_THUNK(                                            \
            uma_type, ("SimpleCache.App." uma_name, ##__VA_ARGS__));   \
        break;                                                         \
      case net::MEDIA_CACHE:                                           \
        SIMPLE_CACHE_THUNK(                                            \
            uma_type, ("SimpleCache.Media." uma_name, ##__VA_ARGS__)); \
        break;                                                         \
      default:                                                         \
        NOTREACHED();                                                  \
        break;                                                         \
    }                                                                  \
  } while (0)

#endif  
