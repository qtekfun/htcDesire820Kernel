// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_TEST_HELPERS_H_
#define MEDIA_BASE_TEST_HELPERS_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "media/base/pipeline_status.h"
#include "media/base/sample_format.h"
#include "media/base/video_decoder_config.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "ui/gfx/size.h"

namespace base {
class MessageLoop;
class TimeDelta;
}

namespace media {

class AudioBuffer;
class DecoderBuffer;

base::Closure NewExpectedClosure();
PipelineStatusCB NewExpectedStatusCB(PipelineStatus status);

class WaitableMessageLoopEvent {
 public:
  WaitableMessageLoopEvent();
  ~WaitableMessageLoopEvent();

  
  base::Closure GetClosure();
  PipelineStatusCB GetPipelineStatusCB();

  
  
  
  void RunAndWait();

  
  
  
  
  void RunAndWaitForStatus(PipelineStatus expected);

 private:
  void OnCallback(PipelineStatus status);
  void OnTimeout();

  base::MessageLoop* message_loop_;
  bool signaled_;
  PipelineStatus status_;

  DISALLOW_COPY_AND_ASSIGN(WaitableMessageLoopEvent);
};

class TestVideoConfig {
 public:
  
  static VideoDecoderConfig Invalid();

  static VideoDecoderConfig Normal();
  static VideoDecoderConfig NormalEncrypted();

  
  static VideoDecoderConfig Large();
  static VideoDecoderConfig LargeEncrypted();

  
  static gfx::Size NormalCodedSize();
  static gfx::Size LargeCodedSize();

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(TestVideoConfig);
};

template <class T>
scoped_refptr<AudioBuffer> MakeInterleavedAudioBuffer(
    SampleFormat format,
    int channels,
    T start,
    T increment,
    int frames,
    base::TimeDelta start_time,
    base::TimeDelta duration);

template <class T>
scoped_refptr<AudioBuffer> MakePlanarAudioBuffer(
    SampleFormat format,
    int channels,
    T start,
    T increment,
    int frames,
    base::TimeDelta start_time,
    base::TimeDelta duration);

scoped_refptr<DecoderBuffer> CreateFakeVideoBufferForTest(
    const VideoDecoderConfig& config,
    base::TimeDelta timestamp,
    base::TimeDelta duration);

bool VerifyFakeVideoBufferForTest(const scoped_refptr<DecoderBuffer>& buffer,
                                  const VideoDecoderConfig& config);

}  

#endif  
