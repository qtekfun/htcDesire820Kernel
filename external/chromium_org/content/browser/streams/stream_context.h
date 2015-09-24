// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_STREAMS_STREAM_CONTEXT_H_
#define CONTENT_BROWSER_STREAMS_STREAM_CONTEXT_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/sequenced_task_runner_helpers.h"
#include "content/common/content_export.h"
#include "content/public/browser/browser_thread.h"

namespace content {
class BrowserContext;
class StreamRegistry;
struct StreamContextDeleter;

class StreamContext
    : public base::RefCountedThreadSafe<StreamContext,
                                        StreamContextDeleter> {
 public:
  StreamContext();

  CONTENT_EXPORT static StreamContext* GetFor(BrowserContext* browser_context);

  void InitializeOnIOThread();

  StreamRegistry* registry() const { return registry_.get(); }

 protected:
  virtual ~StreamContext();

 private:
  friend class base::DeleteHelper<StreamContext>;
  friend class base::RefCountedThreadSafe<StreamContext,
                                          StreamContextDeleter>;
  friend struct StreamContextDeleter;

  void DeleteOnCorrectThread() const;

  scoped_ptr<StreamRegistry> registry_;
};

struct StreamContextDeleter {
  static void Destruct(const StreamContext* context) {
    context->DeleteOnCorrectThread();
  }
};

}  

#endif  
