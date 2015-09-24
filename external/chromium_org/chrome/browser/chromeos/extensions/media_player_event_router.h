// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTENSIONS_MEDIA_PLAYER_EVENT_ROUTER_H_
#define CHROME_BROWSER_CHROMEOS_EXTENSIONS_MEDIA_PLAYER_EVENT_ROUTER_H_

#include "base/basictypes.h"

class Profile;

namespace extensions {

class MediaPlayerEventRouter {
 public:
  explicit MediaPlayerEventRouter(Profile* profile);
  virtual ~MediaPlayerEventRouter();

  
  void NotifyNextTrack();

 
  void NotifyPrevTrack();

  
  void NotifyTogglePlayState();

 private:
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(MediaPlayerEventRouter);
};

}  

#endif  
