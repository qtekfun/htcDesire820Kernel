// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_TEXT_INPUT_CLIENT_MESSAGE_FILTER_H_
#define CONTENT_BROWSER_RENDERER_HOST_TEXT_INPUT_CLIENT_MESSAGE_FILTER_H_

#include "content/common/mac/attributed_string_coder.h"
#include "content/public/browser/browser_message_filter.h"

namespace gfx {
class Range;
class Rect;
}

namespace content {

class CONTENT_EXPORT TextInputClientMessageFilter
    : public BrowserMessageFilter {
 public:
  explicit TextInputClientMessageFilter(int child_id);

  
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;

 protected:
  virtual ~TextInputClientMessageFilter();

 private:
  
  void OnGotCharacterIndexForPoint(size_t index);
  void OnGotFirstRectForRange(const gfx::Rect& rect);
  void OnGotStringFromRange(
      const mac::AttributedStringCoder::EncodedString& string);

  
  int child_process_id_;

  DISALLOW_COPY_AND_ASSIGN(TextInputClientMessageFilter);
};

}  

#endif  
