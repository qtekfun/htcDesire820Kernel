// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_SPEECH_RECOGNITION_SESSION_CONTEXT_H_
#define CONTENT_PUBLIC_BROWSER_SPEECH_RECOGNITION_SESSION_CONTEXT_H_

#include <string>

#include "content/common/content_export.h"
#include "content/public/common/media_stream_request.h"
#include "ui/gfx/rect.h"

namespace content {

struct CONTENT_EXPORT SpeechRecognitionSessionContext {
  SpeechRecognitionSessionContext();
  ~SpeechRecognitionSessionContext();

  int render_process_id;
  int render_view_id;

  
  
  
  
  
  
  
  
  int guest_render_view_id;

  
  
  
  
  int embedder_render_process_id;
  int embedder_render_view_id;

  int request_id;

  
  
  bool requested_by_page_element;

  
  
  gfx::Rect element_rect;

  
  
  std::string context_name;

  
  std::string label;

  
  MediaStreamDevices devices;
};

}  

#endif  
