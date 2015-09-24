// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_MEDIA_DELEGATE_H_
#define ASH_MEDIA_DELEGATE_H_

namespace ash {

class MediaDelegate {
 public:
  virtual ~MediaDelegate() {}

  
  virtual void HandleMediaNextTrack() = 0;

  
  virtual void HandleMediaPlayPause() = 0;

  
  virtual void HandleMediaPrevTrack() = 0;
};

}  

#endif  
