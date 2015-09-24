// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_WIN_DEVICE_ENUMERATION_WIN_H_
#define MEDIA_AUDIO_WIN_DEVICE_ENUMERATION_WIN_H_

#include <string>

#include "media/audio/audio_device_name.h"

namespace media {

bool GetInputDeviceNamesWin(media::AudioDeviceNames* device_names);
bool GetOutputDeviceNamesWin(media::AudioDeviceNames* device_names);

bool GetInputDeviceNamesWinXP(media::AudioDeviceNames* device_names);
bool GetOutputDeviceNamesWinXP(media::AudioDeviceNames* device_names);

std::string ConvertToWinXPInputDeviceId(const std::string& device_id);

}  

#endif  

