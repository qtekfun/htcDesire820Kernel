// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CAST_CAST_ENVIRONMENT_H_
#define MEDIA_CAST_CAST_ENVIRONMENT_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/task_runner.h"
#include "base/time/tick_clock.h"
#include "base/time/time.h"
#include "media/cast/logging/logging_defines.h"
#include "media/cast/logging/logging_impl.h"

namespace media {
namespace cast {

class CastEnvironment : public base::RefCountedThreadSafe<CastEnvironment> {
 public:
  
  enum ThreadId {
    
    
    MAIN,
    
    
    AUDIO_ENCODER,
    
    
    AUDIO_DECODER,
    
    VIDEO_ENCODER,
    
    VIDEO_DECODER,
  };

  CastEnvironment(base::TickClock* clock,
                  scoped_refptr<base::TaskRunner> main_thread_proxy,
                  scoped_refptr<base::TaskRunner> audio_encode_thread_proxy,
                  scoped_refptr<base::TaskRunner> audio_decode_thread_proxy,
                  scoped_refptr<base::TaskRunner> video_encode_thread_proxy,
                  scoped_refptr<base::TaskRunner> video_decode_thread_proxy,
                  const CastLoggingConfig& config);

  
  
  
  
  
  bool PostTask(ThreadId identifier,
                const tracked_objects::Location& from_here,
                const base::Closure& task);

  bool PostDelayedTask(ThreadId identifier,
                       const tracked_objects::Location& from_here,
                       const base::Closure& task,
                       base::TimeDelta delay);

  bool CurrentlyOn(ThreadId identifier);

  base::TickClock* Clock() const;

  
  LoggingImpl* Logging();

 protected:
  virtual ~CastEnvironment();

 private:
  friend class base::RefCountedThreadSafe<CastEnvironment>;

  scoped_refptr<base::TaskRunner> GetMessageTaskRunnerForThread(
      ThreadId identifier);

  base::TickClock* const clock_;  
  scoped_refptr<base::TaskRunner> main_thread_proxy_;
  scoped_refptr<base::TaskRunner> audio_encode_thread_proxy_;
  scoped_refptr<base::TaskRunner> audio_decode_thread_proxy_;
  scoped_refptr<base::TaskRunner> video_encode_thread_proxy_;
  scoped_refptr<base::TaskRunner> video_decode_thread_proxy_;

  scoped_ptr<LoggingImpl> logging_;

  DISALLOW_COPY_AND_ASSIGN(CastEnvironment);
};

}  
}  

#endif  
