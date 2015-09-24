// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_WEBMEDIAPLAYER_DELEGATE_H_
#define CONTENT_RENDERER_MEDIA_WEBMEDIAPLAYER_DELEGATE_H_

namespace blink {
class WebMediaPlayer;
}
namespace content {

class WebMediaPlayerDelegate {
 public:
  WebMediaPlayerDelegate() {}

  
  virtual void DidPlay(blink::WebMediaPlayer* player) = 0;

  
  virtual void DidPause(blink::WebMediaPlayer* player) = 0;

  
  virtual void PlayerGone(blink::WebMediaPlayer* player) = 0;

 protected:
  virtual ~WebMediaPlayerDelegate() {}
};

}  

#endif  
