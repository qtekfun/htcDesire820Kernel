// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_INPUT_INPUT_HANDLER_MANAGER_H_
#define CONTENT_RENDERER_INPUT_INPUT_HANDLER_MANAGER_H_

#include "base/containers/scoped_ptr_hash_map.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "content/port/common/input_event_ack_state.h"
#include "content/renderer/render_view_impl.h"

namespace base {
class MessageLoopProxy;
}

namespace cc {
class InputHandler;
struct DidOverscrollParams;
}

namespace blink {
class WebInputEvent;
}

namespace content {

class InputHandlerWrapper;
class InputHandlerManagerClient;

class InputHandlerManager {
 public:
  
  
  InputHandlerManager(
      const scoped_refptr<base::MessageLoopProxy>& message_loop_proxy,
      InputHandlerManagerClient* client);
  ~InputHandlerManager();

  
  void AddInputHandler(
      int routing_id,
      const base::WeakPtr<cc::InputHandler>& input_handler,
      const base::WeakPtr<RenderViewImpl>& render_view_impl);

  
  void RemoveInputHandler(int routing_id);

  
  InputEventAckState HandleInputEvent(int routing_id,
                                      const blink::WebInputEvent* input_event,
                                      ui::LatencyInfo* latency_info);

  
  void DidOverscroll(int routing_id, const cc::DidOverscrollParams& params);

 private:
  
  void AddInputHandlerOnCompositorThread(
      int routing_id,
      const scoped_refptr<base::MessageLoopProxy>& main_loop,
      const base::WeakPtr<cc::InputHandler>& input_handler,
      const base::WeakPtr<RenderViewImpl>& render_view_impl);

  typedef base::ScopedPtrHashMap<int,  
                                 InputHandlerWrapper> InputHandlerMap;
  InputHandlerMap input_handlers_;

  scoped_refptr<base::MessageLoopProxy> message_loop_proxy_;
  InputHandlerManagerClient* client_;
};

}  

#endif  
