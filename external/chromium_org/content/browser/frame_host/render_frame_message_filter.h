// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_FRAME_HOST_RENDER_FRAME_MESSAGE_FILTER_H_
#define CONTENT_BROWSER_FRAME_HOST_RENDER_FRAME_MESSAGE_FILTER_H_

#include "content/public/browser/browser_message_filter.h"

namespace content {
class RenderWidgetHelper;

class RenderFrameMessageFilter : public BrowserMessageFilter {
 public:
  RenderFrameMessageFilter(int render_process_id,
                           RenderWidgetHelper* render_widget_helper);

  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;

 private:
  virtual ~RenderFrameMessageFilter();

  void OnCreateChildFrame(int parent_render_frame_id,
                          int64 parent_frame_id,
                          int64 frame_id,
                          const std::string& frame_name,
                          int* new_render_frame_id);

  const int render_process_id_;

  
  scoped_refptr<RenderWidgetHelper> render_widget_helper_;
};

}  

#endif  
