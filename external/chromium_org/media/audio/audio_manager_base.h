// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_AUDIO_MANAGER_BASE_H_
#define MEDIA_AUDIO_AUDIO_MANAGER_BASE_H_

#include <string>
#include <utility>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/observer_list.h"
#include "base/threading/thread.h"
#include "media/audio/audio_manager.h"

#include "media/audio/audio_output_dispatcher.h"

#if defined(OS_WIN)
#include "base/win/scoped_com_initializer.h"
#endif

namespace media {

class AudioOutputDispatcher;

class MEDIA_EXPORT AudioManagerBase : public AudioManager {
 public:
  
  

  
  static const char kDefaultDeviceName[];
  
  static const char kDefaultDeviceId[];

  
  
  
  
  
  
  
  
  static const char kLoopbackInputDeviceId[];

  virtual ~AudioManagerBase();

  virtual scoped_refptr<base::MessageLoopProxy> GetMessageLoop() OVERRIDE;
  virtual scoped_refptr<base::MessageLoopProxy> GetWorkerLoop() OVERRIDE;

  virtual base::string16 GetAudioInputDeviceModel() OVERRIDE;

  virtual void ShowAudioInputSettings() OVERRIDE;

  virtual void GetAudioInputDeviceNames(
      AudioDeviceNames* device_names) OVERRIDE;

  virtual void GetAudioOutputDeviceNames(
      AudioDeviceNames* device_names) OVERRIDE;

  virtual AudioOutputStream* MakeAudioOutputStream(
      const AudioParameters& params,
      const std::string& device_id,
      const std::string& input_device_id) OVERRIDE;

  virtual AudioInputStream* MakeAudioInputStream(
      const AudioParameters& params, const std::string& device_id) OVERRIDE;

  virtual AudioOutputStream* MakeAudioOutputStreamProxy(
      const AudioParameters& params,
      const std::string& device_id,
      const std::string& input_device_id) OVERRIDE;

  
  virtual void ReleaseOutputStream(AudioOutputStream* stream);
  virtual void ReleaseInputStream(AudioInputStream* stream);

  
  
  virtual AudioOutputStream* MakeLinearOutputStream(
      const AudioParameters& params) = 0;

  
  
  virtual AudioOutputStream* MakeLowLatencyOutputStream(
      const AudioParameters& params,
      const std::string& device_id,
      const std::string& input_device_id) = 0;

  
  
  virtual AudioInputStream* MakeLinearInputStream(
      const AudioParameters& params, const std::string& device_id) = 0;

  
  virtual AudioInputStream* MakeLowLatencyInputStream(
      const AudioParameters& params, const std::string& device_id) = 0;

  
  virtual void AddOutputDeviceChangeListener(
      AudioDeviceListener* listener) OVERRIDE;
  virtual void RemoveOutputDeviceChangeListener(
      AudioDeviceListener* listener) OVERRIDE;

  virtual AudioParameters GetDefaultOutputStreamParameters() OVERRIDE;
  virtual AudioParameters GetOutputStreamParameters(
      const std::string& device_id) OVERRIDE;

  virtual AudioParameters GetInputStreamParameters(
      const std::string& device_id) OVERRIDE;

  virtual std::string GetAssociatedOutputDeviceID(
      const std::string& input_device_id) OVERRIDE;

  virtual scoped_ptr<AudioLog> CreateAudioLog(
      AudioLogFactory::AudioComponent component) OVERRIDE;

  virtual void FixWedgedAudio() OVERRIDE;

 protected:
  AudioManagerBase(AudioLogFactory* audio_log_factory);

  
  
  
  
  void Shutdown();

  void SetMaxOutputStreamsAllowed(int max) { max_num_output_streams_ = max; }

  
  
  
  void NotifyAllOutputDeviceChangeListeners();

  
  
  int GetUserBufferSize();

  
  
  
  
  
  
  
  virtual AudioParameters GetPreferredOutputStreamParameters(
      const std::string& output_device_id,
      const AudioParameters& input_params) = 0;

  
  
  virtual std::string GetDefaultOutputDeviceID();

  
  int input_stream_count() { return num_input_streams_; }
  int output_stream_count() { return num_output_streams_; }

 private:
  struct DispatcherParams;
  typedef ScopedVector<DispatcherParams> AudioOutputDispatchers;

  class CompareByParams;

  
  void ShutdownOnAudioThread();

  
  
  int max_num_output_streams_;

  
  int max_num_input_streams_;

  
  int num_output_streams_;

  
  int num_input_streams_;

  
  ObserverList<AudioDeviceListener> output_listeners_;

  
  base::Thread audio_thread_;

  
  
  
  scoped_refptr<base::MessageLoopProxy> message_loop_;

  
  
  AudioOutputDispatchers output_dispatchers_;

  
  AudioLogFactory* const audio_log_factory_;

  DISALLOW_COPY_AND_ASSIGN(AudioManagerBase);
};

}  

#endif  
