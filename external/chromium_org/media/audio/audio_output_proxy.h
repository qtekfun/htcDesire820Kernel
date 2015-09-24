// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_AUDIO_OUTPUT_PROXY_H_
#define MEDIA_AUDIO_AUDIO_OUTPUT_PROXY_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/threading/non_thread_safe.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"

namespace media {

class AudioOutputDispatcher;

class MEDIA_EXPORT AudioOutputProxy
  : public AudioOutputStream,
    public NON_EXPORTED_BASE(base::NonThreadSafe) {
 public:
  
  explicit AudioOutputProxy(AudioOutputDispatcher* dispatcher);

  
  virtual bool Open() OVERRIDE;
  virtual void Start(AudioSourceCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual void GetVolume(double* volume) OVERRIDE;
  virtual void Close() OVERRIDE;

 private:
  enum State {
    kCreated,
    kOpened,
    kPlaying,
    kClosed,
    kOpenError,
    kStartError,
  };

  virtual ~AudioOutputProxy();

  scoped_refptr<AudioOutputDispatcher> dispatcher_;
  State state_;

  
  
  double volume_;

  DISALLOW_COPY_AND_ASSIGN(AudioOutputProxy);
};

}  

#endif  
