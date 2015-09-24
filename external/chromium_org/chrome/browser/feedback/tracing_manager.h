// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FEEDBACK_TRACING_MANAGER_H_
#define CHROME_BROWSER_FEEDBACK_TRACING_MANAGER_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"

namespace base {

class RefCountedString;
class FilePath;

}
typedef base::Callback<void(scoped_refptr<base::RefCountedString> trace_data)>
    TraceDataCallback;

class TracingManager {
 public:
  virtual ~TracingManager();

  
  static scoped_ptr<TracingManager> Create();

  
  static TracingManager* Get();

  
  
  int RequestTrace();

  
  
  bool GetTraceData(int id, const TraceDataCallback& callback);

  
  void DiscardTraceData(int id);

 private:
  TracingManager();

  void StartTracing();
  void OnTraceDataCollected(const base::FilePath& path);

  
  int current_trace_id_;

  
  std::map<int, scoped_refptr<base::RefCountedString> > trace_data_;

  
  TraceDataCallback trace_callback_;

  base::WeakPtrFactory<TracingManager> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(TracingManager);
};

#endif  

