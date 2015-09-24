// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_WORKER_PERMISSION_CLIENT_PROXY_H_
#define CHROME_RENDERER_WORKER_PERMISSION_CLIENT_PROXY_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "third_party/WebKit/public/web/WebWorkerPermissionClientProxy.h"
#include "url/gurl.h"

namespace IPC {
class SyncMessageFilter;
}

namespace content {
class RenderView;
}

namespace blink {
class WebFrame;
}

class WorkerPermissionClientProxy
    : public blink::WebWorkerPermissionClientProxy {
 public:
  WorkerPermissionClientProxy(content::RenderView* render_view,
                              blink::WebFrame* frame);
  virtual ~WorkerPermissionClientProxy();

  
  virtual bool allowDatabase(const blink::WebString& name,
                             const blink::WebString& display_name,
                             unsigned long estimated_size);
  virtual bool allowFileSystem();
  virtual bool allowIndexedDB(const blink::WebString& name);

 private:
  
  const int routing_id_;
  bool is_unique_origin_;
  GURL document_origin_url_;
  GURL top_frame_origin_url_;
  scoped_refptr<IPC::SyncMessageFilter> sync_message_filter_;

  DISALLOW_COPY_AND_ASSIGN(WorkerPermissionClientProxy);
};

#endif  
