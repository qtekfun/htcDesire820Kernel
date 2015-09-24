// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_INSTANT_IO_CONTEXT_H_
#define CHROME_BROWSER_SEARCH_INSTANT_IO_CONTEXT_H_

#include <set>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"

class GURL;

namespace content {
class ResourceContext;
}

namespace net {
class URLRequest;
}

class InstantIOContext : public base::RefCountedThreadSafe<InstantIOContext> {
 public:
  InstantIOContext();

  
  
  static const char kInstantIOContextKeyName[];

  
  
  static void SetUserDataOnIO(
      content::ResourceContext* resource_context,
      scoped_refptr<InstantIOContext> instant_io_context);

  
  
  static void AddInstantProcessOnIO(
      scoped_refptr<InstantIOContext> instant_io_context,
      int process_id);
  static void RemoveInstantProcessOnIO(
      scoped_refptr<InstantIOContext> instant_io_context,
      int process_id);
  static void ClearInstantProcessesOnIO(
      scoped_refptr<InstantIOContext> instant_io_context);

  
  
  static bool ShouldServiceRequest(const net::URLRequest* request);

 protected:
   virtual ~InstantIOContext();

 private:
  friend class base::RefCountedThreadSafe<InstantIOContext>;

  
  
  bool IsInstantProcess(int process_id) const;

  
  
  
  std::set<int> process_ids_;

  DISALLOW_COPY_AND_ASSIGN(InstantIOContext);
};

#endif  
