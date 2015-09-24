// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_TRACE_H__
#define NET_DISK_CACHE_TRACE_H__

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "net/base/net_export.h"

namespace disk_cache {

void InitTrace(void);
void DestroyTrace(void);

class TraceObject : public base::RefCounted<TraceObject> {
  friend class base::RefCounted<TraceObject>;
 public:
  static TraceObject* GetTraceObject();
  void EnableTracing(bool enable);

 private:
  TraceObject();
  ~TraceObject();
  DISALLOW_COPY_AND_ASSIGN(TraceObject);
};

NET_EXPORT_PRIVATE void Trace(const char* format, ...);

}  

#endif  
