// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_AUDIO_DEVICE_FACTORY_H_
#define CONTENT_RENDERER_MEDIA_AUDIO_DEVICE_FACTORY_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"

namespace media {
class AudioInputDevice;
class AudioOutputDevice;
}

namespace content {

class AudioDeviceFactory {
 public:
  
  
  
  static scoped_refptr<media::AudioOutputDevice> NewOutputDevice(
      int render_view_id);

  
  
  
  static scoped_refptr<media::AudioInputDevice> NewInputDevice(
      int render_view_id);

 protected:
  AudioDeviceFactory();
  virtual ~AudioDeviceFactory();

  
  
  
  
  virtual media::AudioOutputDevice* CreateOutputDevice(int render_view_id) = 0;
  virtual media::AudioInputDevice* CreateInputDevice(int render_view_id) = 0;

 private:
  
  
  static AudioDeviceFactory* factory_;

  DISALLOW_COPY_AND_ASSIGN(AudioDeviceFactory);
};

}  

#endif  
