// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_MEDIA_AUDIO_INPUT_RENDERER_HOST_H_
#define CONTENT_BROWSER_RENDERER_HOST_MEDIA_AUDIO_INPUT_RENDERER_HOST_H_

#include <map>
#include <string>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/process/process.h"
#include "base/sequenced_task_runner_helpers.h"
#include "content/common/media/audio_messages.h"
#include "content/public/browser/browser_message_filter.h"
#include "content/public/browser/browser_thread.h"
#include "media/audio/audio_input_controller.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_logging.h"
#include "media/audio/simple_sources.h"

namespace media {
class AudioManager;
class AudioParameters;
class UserInputMonitor;
}

namespace content {
class AudioMirroringManager;
class MediaStreamManager;

class CONTENT_EXPORT AudioInputRendererHost
    : public BrowserMessageFilter,
      public media::AudioInputController::EventHandler {
 public:
  
  
  AudioInputRendererHost(media::AudioManager* audio_manager,
                         MediaStreamManager* media_stream_manager,
                         AudioMirroringManager* audio_mirroring_manager,
                         media::UserInputMonitor* user_input_monitor);

  
  virtual void OnChannelClosing() OVERRIDE;
  virtual void OnDestruct() const OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;

  
  virtual void OnCreated(media::AudioInputController* controller) OVERRIDE;
  virtual void OnRecording(media::AudioInputController* controller) OVERRIDE;
  virtual void OnError(media::AudioInputController* controller) OVERRIDE;
  virtual void OnData(media::AudioInputController* controller,
                      const uint8* data,
                      uint32 size) OVERRIDE;

 private:
  
  friend class BrowserThread;
  friend class TestAudioInputRendererHost;
  friend class base::DeleteHelper<AudioInputRendererHost>;

  struct AudioEntry;
  typedef std::map<int, AudioEntry*> AudioEntryMap;

  virtual ~AudioInputRendererHost();

  

  

  
  
  
  
  
  void OnCreateStream(int stream_id,
                      int render_view_id,
                      int session_id,
                      const AudioInputHostMsg_CreateStream_Config& config);

  
  void OnRecordStream(int stream_id);

  
  void OnCloseStream(int stream_id);

  
  void OnSetVolume(int stream_id, double volume);

  
  
  
  void DoCompleteCreation(media::AudioInputController* controller);

  
  void DoSendRecordingMessage(media::AudioInputController* controller);

  
  void DoHandleError(media::AudioInputController* controller);

  
  void SendErrorMessage(int stream_id);

  
  void DeleteEntries();

  
  
  void CloseAndDeleteStream(AudioEntry* entry);

  
  void DeleteEntry(AudioEntry* entry);

  
  void DeleteEntryOnError(AudioEntry* entry);

  
  
  AudioEntry* LookupById(int stream_id);

  
  
  
  AudioEntry* LookupByController(media::AudioInputController* controller);

  
  media::AudioManager* audio_manager_;

  
  MediaStreamManager* media_stream_manager_;

  AudioMirroringManager* audio_mirroring_manager_;

  
  AudioEntryMap audio_entries_;

  
  media::UserInputMonitor* user_input_monitor_;

  scoped_ptr<media::AudioLog> audio_log_;

  DISALLOW_COPY_AND_ASSIGN(AudioInputRendererHost);
};

}  

#endif  
