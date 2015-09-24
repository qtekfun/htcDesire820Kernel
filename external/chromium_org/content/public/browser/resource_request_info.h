// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_RESOURCE_REQUEST_INFO_H_
#define CONTENT_PUBLIC_BROWSER_RESOURCE_REQUEST_INFO_H_

#include "base/basictypes.h"
#include "content/common/content_export.h"
#include "content/public/common/page_transition_types.h"
#include "third_party/WebKit/public/platform/WebReferrerPolicy.h"
#include "webkit/common/resource_type.h"

namespace net {
class URLRequest;
}

namespace content {
class ResourceContext;

class ResourceRequestInfo {
 public:
  
  CONTENT_EXPORT static const ResourceRequestInfo* ForRequest(
      const net::URLRequest* request);

  
  
  
  CONTENT_EXPORT static void AllocateForTesting(
      net::URLRequest* request,
      ResourceType::Type resource_type,
      ResourceContext* context,
      int render_process_id,
      int render_view_id,
      bool is_async);

  
  
  
  
  CONTENT_EXPORT static bool GetRenderViewForRequest(
      const net::URLRequest* request,
      int* render_process_id,
      int* render_view_id);

  
  virtual ResourceContext* GetContext() const = 0;

  
  virtual int GetChildID() const = 0;

  
  
  virtual int GetRouteID() const = 0;

  
  
  virtual int GetOriginPID() const = 0;

  
  virtual int GetRequestID() const = 0;

  
  
  
  
  virtual int GetRenderFrameID() const = 0;

  
  virtual bool IsMainFrame() const = 0;

  
  virtual int64 GetFrameID() const = 0;

  
  virtual bool ParentIsMainFrame() const = 0;

  
  
  virtual int64 GetParentFrameID() const = 0;

  
  virtual ResourceType::Type GetResourceType() const = 0;

  
  virtual blink::WebReferrerPolicy GetReferrerPolicy() const = 0;

  
  virtual PageTransition GetPageTransition() const = 0;

  
  
  virtual bool HasUserGesture() const = 0;

  
  
  virtual bool WasIgnoredByHandler() const = 0;

  
  virtual bool GetAssociatedRenderView(int* render_process_id,
                                       int* render_view_id) const = 0;

  
  virtual bool IsAsync() const = 0;

  
  virtual bool IsDownload() const = 0;

 protected:
  virtual ~ResourceRequestInfo() {}
};

}  

#endif  
