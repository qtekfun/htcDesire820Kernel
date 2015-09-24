// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_INPUT_INPUT_HANDLER_MANAGER_CLIENT_H_
#define CONTENT_RENDERER_INPUT_INPUT_HANDLER_MANAGER_CLIENT_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/callback_forward.h"
#include "content/common/content_export.h"
#include "ui/gfx/vector2d_f.h"

namespace ui {
struct LatencyInfo;
}

namespace cc {
class InputHandler;
struct DidOverscrollParams;
}

namespace blink {
class WebInputEvent;
}

namespace content {

class CONTENT_EXPORT InputHandlerManagerClient {
 public:
  virtual ~InputHandlerManagerClient() {}

  
  
  
  typedef base::Callback<
      InputEventAckState(int ,
                         const blink::WebInputEvent*,
                         ui::LatencyInfo* latency_info)> Handler;

  
  virtual void SetBoundHandler(const Handler& handler) = 0;

  
  virtual void DidAddInputHandler(int routing_id,
                                  cc::InputHandler* input_handler) = 0;
  virtual void DidRemoveInputHandler(int routing_id) = 0;
  virtual void DidOverscroll(int routing_id,
                             const cc::DidOverscrollParams& params) = 0;

 protected:
  InputHandlerManagerClient() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(InputHandlerManagerClient);
};

}  

#endif  
