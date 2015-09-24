// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_TEST_AUDIO_INPUT_CONTROLLER_FACTORY_H_
#define MEDIA_AUDIO_TEST_AUDIO_INPUT_CONTROLLER_FACTORY_H_

#include "base/bind.h"
#include "media/audio/audio_input_controller.h"

namespace media {

class UserInputMonitor;
class TestAudioInputControllerFactory;


class TestAudioInputController : public AudioInputController {
 public:
  class Delegate {
   public:
    virtual void TestAudioControllerOpened(
        TestAudioInputController* controller) = 0;
    virtual void TestAudioControllerClosed(
        TestAudioInputController* controller) = 0;
  };

  TestAudioInputController(TestAudioInputControllerFactory* factory,
                           AudioManager* audio_manager,
                           const AudioParameters& audio_parameters,
                           EventHandler* event_handler,
                           SyncWriter* sync_writer,
                           UserInputMonitor* user_input_monitor);

  
  EventHandler* event_handler() const { return event_handler_; }

  
  virtual void Record() OVERRIDE;

  
  virtual void Close(const base::Closure& closed_task) OVERRIDE;

  const AudioParameters& audio_parameters() const {
    return audio_parameters_;
  }

 protected:
  virtual ~TestAudioInputController();

 private:
  AudioParameters audio_parameters_;

  
  
  TestAudioInputControllerFactory* factory_;
  EventHandler* event_handler_;

  DISALLOW_COPY_AND_ASSIGN(TestAudioInputController);
};

typedef TestAudioInputController::Delegate TestAudioInputControllerDelegate;

class TestAudioInputControllerFactory : public AudioInputController::Factory {
 public:
  TestAudioInputControllerFactory();
  virtual ~TestAudioInputControllerFactory();

  
  virtual AudioInputController* Create(
      AudioManager* audio_manager,
      AudioInputController::EventHandler* event_handler,
      AudioParameters params,
      UserInputMonitor* user_input_monitor) OVERRIDE;

  void set_delegate(TestAudioInputControllerDelegate* delegate) {
    delegate_ = delegate;
  }

  TestAudioInputController* controller() const { return controller_; }

 private:
  friend class TestAudioInputController;

  
  void OnTestAudioInputControllerDestroyed(
      TestAudioInputController* controller);

  
  TestAudioInputController* controller_;

  
  TestAudioInputControllerDelegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(TestAudioInputControllerFactory);
};

}  

#endif  
