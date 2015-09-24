// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PAPPI_TESTS_TEST_AUDIO_H_
#define PAPPI_TESTS_TEST_AUDIO_H_

#include <string>

#include "ppapi/c/ppb_audio.h"
#include "ppapi/c/ppb_audio_config.h"
#include "ppapi/c/ppb_core.h"
#include "ppapi/tests/test_case.h"

class TestAudio : public TestCase {
 public:
  explicit TestAudio(TestingInstance* instance);
  ~TestAudio();

  
  virtual bool Init();
  virtual void RunTests(const std::string& filter);

 private:
  std::string TestCreation();
  std::string TestDestroyNoStop();
  std::string TestFailures();
  std::string TestAudioCallback1();
  std::string TestAudioCallback2();
  std::string TestAudioCallback3();
  std::string TestAudioCallback4();

  
  static void AudioCallbackTrampoline(void* sample_buffer,
                                      uint32_t buffer_size_in_bytes,
                                      PP_TimeDelta latency,
                                      void* user_data);
  static void AudioCallbackTrampoline1_0(void* sample_buffer,
                                         uint32_t buffer_size_in_bytes,
                                         void* user_data);

  typedef void (TestAudio::*AudioCallbackMethod)(void* sample_buffer,
                                                 uint32_t buffer_size_in_bytes,
                                                 PP_TimeDelta latency);

  
  
  
  AudioCallbackMethod audio_callback_method_;

  
  void AudioCallbackTrivial(void* sample_buffer,
                            uint32_t buffer_size_in_bytes,
                            PP_TimeDelta latency);

  
  void AudioCallbackTest(void* sample_buffer,
                         uint32_t buffer_size_in_bytes,
                         PP_TimeDelta latency);

  PP_Resource CreateAudioConfig(PP_AudioSampleRate sample_rate,
                                uint32_t requested_sample_frame_count);

  
  NestedEvent audio_callback_event_;

  bool test_done_;

  
  const PPB_Audio_1_1* audio_interface_;
  const PPB_Audio_1_0* audio_interface_1_0_;
  const PPB_AudioConfig* audio_config_interface_;
  const PPB_Core* core_interface_;
};

#endif  
