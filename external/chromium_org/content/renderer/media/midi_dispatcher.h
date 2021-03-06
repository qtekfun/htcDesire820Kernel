// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_MIDI_DISPATCHER_H_
#define CONTENT_RENDERER_MEDIA_MIDI_DISPATCHER_H_

#include "base/id_map.h"
#include "content/public/renderer/render_view_observer.h"
#include "third_party/WebKit/public/web/WebMIDIClient.h"

namespace blink {
class WebMIDIPermissionRequest;
}

namespace content {

class RenderViewImpl;

class MIDIDispatcher : public RenderViewObserver,
                       public blink::WebMIDIClient {
 public:
  explicit MIDIDispatcher(RenderViewImpl* render_view);
  virtual ~MIDIDispatcher();

 private:
  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  virtual void requestSysExPermission(
      const blink::WebMIDIPermissionRequest& request) OVERRIDE;
  virtual void cancelSysExPermissionRequest(
      const blink::WebMIDIPermissionRequest& request) OVERRIDE;

  
  void OnSysExPermissionApproved(int client_id, bool is_allowed);

  
  
  
  IDMap<blink::WebMIDIPermissionRequest> requests_;

  DISALLOW_COPY_AND_ASSIGN(MIDIDispatcher);
};

}  

#endif  
