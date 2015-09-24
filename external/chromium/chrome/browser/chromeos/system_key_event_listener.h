// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_SYSTEM_KEY_EVENT_LISTENER_H_
#define CHROME_BROWSER_CHROMEOS_SYSTEM_KEY_EVENT_LISTENER_H_
#pragma once

#include <gdk/gdk.h>

#include "base/memory/singleton.h"
#include "chrome/browser/chromeos/wm_message_listener.h"

namespace chromeos {

class AudioHandler;



class SystemKeyEventListener : public WmMessageListener::Observer {
 public:
  static SystemKeyEventListener* GetInstance();

  
  virtual void ProcessWmMessage(const WmIpc::Message& message,
                                GdkWindow* window);

  void Stop();

 private:
  
  
  friend struct DefaultSingletonTraits<SystemKeyEventListener>;

  SystemKeyEventListener();
  virtual ~SystemKeyEventListener();

  
  
  
  static GdkFilterReturn GdkEventFilter(GdkXEvent* gxevent,
                                        GdkEvent* gevent,
                                        gpointer data);

  
  
  void GrabKey(int32 key, uint32 mask);

  void OnVolumeMute();
  void OnVolumeDown();
  void OnVolumeUp();

  int32 key_volume_mute_;
  int32 key_volume_down_;
  int32 key_volume_up_;
  int32 key_f8_;
  int32 key_f9_;
  int32 key_f10_;

  bool stopped_;

  
  
  AudioHandler* const audio_handler_;

  DISALLOW_COPY_AND_ASSIGN(SystemKeyEventListener);
};

}  

#endif  

