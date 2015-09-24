// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_SOCKET_STREAM_HANDLE_DATA_H_
#define CONTENT_RENDERER_SOCKET_STREAM_HANDLE_DATA_H_

#include "base/supports_user_data.h"
#include "content_export.h"

namespace blink {
class WebSocketStreamHandle;
}

namespace content {

class SocketStreamHandleData : public base::SupportsUserData::Data {
 public:
  virtual ~SocketStreamHandleData() {}

  int render_view_id() const { return render_view_id_; }

  
  
  static void AddToHandle(
      blink::WebSocketStreamHandle* handle, int render_view_id);

  
  
  CONTENT_EXPORT static const SocketStreamHandleData* ForHandle(
      blink::WebSocketStreamHandle* handle);

 private:
  explicit SocketStreamHandleData(int render_view_id)
      : render_view_id_(render_view_id) {
  }

  int render_view_id_;

  DISALLOW_COPY_AND_ASSIGN(SocketStreamHandleData);
};

}  

#endif  
