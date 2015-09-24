// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_AUDIO_INPUT_CONTROLLER_H_
#define MEDIA_AUDIO_AUDIO_INPUT_CONTROLLER_H_

#include <string>
#include "base/atomicops.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/thread.h"
#include "base/timer/timer.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_manager_base.h"

namespace media {

class UserInputMonitor;

class MEDIA_EXPORT AudioInputController
    : public base::RefCountedThreadSafe<AudioInputController>,
      public AudioInputStream::AudioInputCallback {
 public:
  
  
  class MEDIA_EXPORT EventHandler {
   public:
    virtual void OnCreated(AudioInputController* controller) = 0;
    virtual void OnRecording(AudioInputController* controller) = 0;
    virtual void OnError(AudioInputController* controller) = 0;
    virtual void OnData(AudioInputController* controller, const uint8* data,
                        uint32 size) = 0;

   protected:
    virtual ~EventHandler() {}
  };

  
  
  class SyncWriter {
   public:
    virtual ~SyncWriter() {}

    
    
    virtual void UpdateRecordedBytes(uint32 bytes) = 0;

    
    // number of written bytes.
    virtual uint32 Write(const void* data,
                         uint32 size,
                         double volume,
                         bool key_pressed) = 0;

    
    virtual void Close() = 0;
  };

  
  
  
  class Factory {
   public:
    virtual AudioInputController* Create(
        AudioManager* audio_manager,
        EventHandler* event_handler,
        AudioParameters params,
        UserInputMonitor* user_input_monitor) = 0;

   protected:
    virtual ~Factory() {}
  };

  
  
  
  
  
  static scoped_refptr<AudioInputController> Create(
      AudioManager* audio_manager,
      EventHandler* event_handler,
      const AudioParameters& params,
      const std::string& device_id,
      UserInputMonitor* user_input_monitor);

  
  
  
  static void set_factory_for_testing(Factory* factory) { factory_ = factory; }
  AudioInputStream* stream_for_testing() { return stream_; }

  
  
  
  
  static scoped_refptr<AudioInputController> CreateLowLatency(
      AudioManager* audio_manager,
      EventHandler* event_handler,
      const AudioParameters& params,
      const std::string& device_id,
      
      SyncWriter* sync_writer,
      UserInputMonitor* user_input_monitor);

  
  
  
  
  
  static scoped_refptr<AudioInputController> CreateForStream(
      const scoped_refptr<base::MessageLoopProxy>& message_loop,
      EventHandler* event_handler,
      AudioInputStream* stream,
      
      SyncWriter* sync_writer,
      UserInputMonitor* user_input_monitor);

  
  
  virtual void Record();

  
  
  
  
  
  
  
  
  virtual void Close(const base::Closure& closed_task);

  
  
  virtual void SetVolume(double volume);

  
  
  virtual void SetAutomaticGainControl(bool enabled);

  
  
  virtual void OnData(AudioInputStream* stream, const uint8* src, uint32 size,
                      uint32 hardware_delay_bytes, double volume) OVERRIDE;
  virtual void OnClose(AudioInputStream* stream) OVERRIDE;
  virtual void OnError(AudioInputStream* stream) OVERRIDE;

  bool LowLatencyMode() const { return sync_writer_ != NULL; }

 protected:
  friend class base::RefCountedThreadSafe<AudioInputController>;

  
  enum State {
    kEmpty,
    kCreated,
    kRecording,
    kClosed,
    kError
  };

  AudioInputController(EventHandler* handler,
                       SyncWriter* sync_writer,
                       UserInputMonitor* user_input_monitor);
  virtual ~AudioInputController();

  
  void DoCreate(AudioManager* audio_manager, const AudioParameters& params,
                const std::string& device_id);
  void DoCreateForStream(AudioInputStream* stream_to_control,
                         bool enable_nodata_timer);
  void DoRecord();
  void DoClose();
  void DoReportError();
  void DoSetVolume(double volume);
  void DoSetAutomaticGainControl(bool enabled);

  
  
  void DoCheckForNoData();

  
  
  void DoStopCloseAndClearStream(base::WaitableEvent* done);

  void SetDataIsActive(bool enabled);
  bool GetDataIsActive();

  
  scoped_refptr<base::MessageLoopProxy> creator_loop_;

  
  scoped_refptr<base::MessageLoopProxy> message_loop_;

  
  
  EventHandler* handler_;

  
  AudioInputStream* stream_;

  
  
  
  
  
  scoped_ptr<base::Timer> no_data_timer_;

  
  
  
  
  base::subtle::Atomic32 data_is_active_;

  // |state_| is written on the audio thread and is read on the hardware audio
  
  
  State state_;

  base::Lock lock_;

  
  SyncWriter* sync_writer_;

  static Factory* factory_;

  double max_volume_;

  UserInputMonitor* user_input_monitor_;

  size_t prev_key_down_count_;

  DISALLOW_COPY_AND_ASSIGN(AudioInputController);
};

}  

#endif  
