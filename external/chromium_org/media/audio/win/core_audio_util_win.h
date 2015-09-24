// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_AUDIO_WIN_CORE_AUDIO_UTIL_WIN_H_
#define MEDIA_AUDIO_WIN_CORE_AUDIO_UTIL_WIN_H_

#include <audioclient.h>
#include <mmdeviceapi.h>
#include <string>

#include "base/basictypes.h"
#include "base/time/time.h"
#include "base/win/scoped_comptr.h"
#include "media/audio/audio_device_name.h"
#include "media/audio/audio_parameters.h"
#include "media/base/media_export.h"

using base::win::ScopedComPtr;

namespace media {


typedef uint32 ChannelConfig;

class MEDIA_EXPORT CoreAudioUtil {
 public:
  
  
  
  static bool IsSupported();

  
  
  
  static base::TimeDelta RefererenceTimeToTimeDelta(REFERENCE_TIME time);

  
  
  static AUDCLNT_SHAREMODE GetShareMode();

  
  

  
  
  
  static int NumberOfActiveDevices(EDataFlow data_flow);

  
  
  static ScopedComPtr<IMMDeviceEnumerator> CreateDeviceEnumerator();

  
  
  static ScopedComPtr<IMMDevice> CreateDefaultDevice(
      EDataFlow data_flow, ERole role);

  
  
  static std::string GetDefaultOutputDeviceID();

  
  
  static ScopedComPtr<IMMDevice> CreateDevice(const std::string& device_id);

  
  
  
  static HRESULT GetDeviceName(IMMDevice* device, AudioDeviceName* name);

  
  
  
  
  
  
  
  
  
  static std::string GetAudioControllerID(IMMDevice* device,
      IMMDeviceEnumerator* enumerator);

  
  
  
  static std::string GetMatchingOutputDeviceID(
      const std::string& input_device_id);

  
  
  static std::string GetFriendlyName(const std::string& device_id);

  
  
  static bool DeviceIsDefault(
      EDataFlow flow, ERole role, const std::string& device_id);

  
  static EDataFlow GetDataFlow(IMMDevice* device);

  
  
  

  
  
  
  
  
  
  static ScopedComPtr<IAudioClient> CreateDefaultClient(EDataFlow data_flow,
                                                        ERole role);

  
  
  static ScopedComPtr<IAudioClient> CreateClient(const std::string& device_id,
                                                 EDataFlow data_flow,
                                                 ERole role);

  
  
  static ScopedComPtr<IAudioClient> CreateClient(IMMDevice* audio_device);

  
  
  
  
  static HRESULT GetSharedModeMixFormat(IAudioClient* client,
                                        WAVEFORMATPCMEX* format);

  
  
  static bool IsFormatSupported(IAudioClient* client,
                                AUDCLNT_SHAREMODE share_mode,
                                const WAVEFORMATPCMEX* format);

  
  
  
  
  
  static bool IsChannelLayoutSupported(const std::string& device_id,
                                       EDataFlow data_flow,
                                       ERole role,
                                       ChannelLayout channel_layout);

  
  
  
  
  
  
  
  static HRESULT GetDevicePeriod(IAudioClient* client,
                                 AUDCLNT_SHAREMODE share_mode,
                                 REFERENCE_TIME* device_period);

  
  
  
  
  
  static HRESULT GetPreferredAudioParameters(IAudioClient* client,
                                             AudioParameters* params);
  static HRESULT GetPreferredAudioParameters(EDataFlow data_flow, ERole role,
                                             AudioParameters* params);
  static HRESULT GetPreferredAudioParameters(const std::string& device_id,
                                             AudioParameters* params);

  
  
  
  
  
  
  
  
  
  
  static ChannelConfig GetChannelConfig(const std::string& device_id,
                                        EDataFlow data_flow);

  
  
  
  
  
  
  
  
  static HRESULT SharedModeInitialize(IAudioClient* client,
                                      const WAVEFORMATPCMEX* format,
                                      HANDLE event_handle,
                                      uint32* endpoint_buffer_size);
  

  
  
  
  static ScopedComPtr<IAudioRenderClient> CreateRenderClient(
      IAudioClient* client);

  
  
  
  static ScopedComPtr<IAudioCaptureClient> CreateCaptureClient(
      IAudioClient* client);

  
  
  
  static bool FillRenderEndpointBufferWithSilence(
      IAudioClient* client, IAudioRenderClient* render_client);

 private:
  CoreAudioUtil() {}
  ~CoreAudioUtil() {}
  DISALLOW_COPY_AND_ASSIGN(CoreAudioUtil);
};

}  

#endif  
