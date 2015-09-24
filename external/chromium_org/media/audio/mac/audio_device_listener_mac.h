// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_MAC_AUDIO_DEVICE_LISTENER_MAC_H_
#define MEDIA_AUDIO_MAC_AUDIO_DEVICE_LISTENER_MAC_H_

#include <CoreAudio/AudioHardware.h>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/threading/thread_checker.h"
#include "media/base/media_export.h"

namespace media {

class MEDIA_EXPORT AudioDeviceListenerMac {
 public:
  
  
  
  explicit AudioDeviceListenerMac(const base::Closure& listener_cb);
  ~AudioDeviceListenerMac();

 private:
  friend class AudioDeviceListenerMacTest;
  static const AudioObjectPropertyAddress kDeviceChangePropertyAddress;

  static OSStatus OnDefaultDeviceChanged(
      AudioObjectID object, UInt32 num_addresses,
      const AudioObjectPropertyAddress addresses[], void* context);

  base::Closure listener_cb_;

  
  
  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(AudioDeviceListenerMac);
};

}  

#endif  
