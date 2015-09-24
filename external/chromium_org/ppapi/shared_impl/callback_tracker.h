// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_CALLBACK_TRACKER_H_
#define PPAPI_SHARED_IMPL_CALLBACK_TRACKER_H_

#include <map>
#include <set>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"

namespace ppapi {

class TrackedCallback;


class PPAPI_SHARED_EXPORT CallbackTracker
    : public base::RefCountedThreadSafe<CallbackTracker> {
 public:
  CallbackTracker();

  
  void AbortAll();

  
  
  void PostAbortForResource(PP_Resource resource_id);

 private:
  friend class base::RefCountedThreadSafe<CallbackTracker>;
  ~CallbackTracker();

  
  
  friend class TrackedCallback;
  void Add(const scoped_refptr<TrackedCallback>& tracked_callback);
  void Remove(const scoped_refptr<TrackedCallback>& tracked_callback);

  
  
  
  
  
  typedef std::set<scoped_refptr<TrackedCallback> > CallbackSet;
  typedef std::map<PP_Resource, CallbackSet> CallbackSetMap;
  CallbackSetMap pending_callbacks_;

  DISALLOW_COPY_AND_ASSIGN(CallbackTracker);
};

}  

#endif  
