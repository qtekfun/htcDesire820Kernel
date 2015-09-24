// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_LINUX_AUDIO_PIPE_READER_H_
#define REMOTING_HOST_LINUX_AUDIO_PIPE_READER_H_

#include "base/memory/ref_counted.h"
#include "base/memory/ref_counted_memory.h"
#include "base/message_loop/message_loop.h"
#include "base/observer_list_threadsafe.h"
#include "base/time/time.h"
#include "base/timer/timer.h"

namespace base {
class FilePath;
}

namespace remoting {

struct AudioPipeReaderTraits;

class AudioPipeReader
  : public base::RefCountedThreadSafe<AudioPipeReader, AudioPipeReaderTraits>,
      public base::MessageLoopForIO::Watcher {
 public:
  class StreamObserver {
   public:
    virtual void OnDataRead(scoped_refptr<base::RefCountedString> data) = 0;
  };

  
  static scoped_refptr<AudioPipeReader> Create(
      scoped_refptr<base::SingleThreadTaskRunner> task_runner,
      const base::FilePath& pipe_name);

  
  
  
  void AddObserver(StreamObserver* observer);
  void RemoveObserver(StreamObserver* observer);

  
  virtual void OnFileCanReadWithoutBlocking(int fd) OVERRIDE;
  virtual void OnFileCanWriteWithoutBlocking(int fd) OVERRIDE;

 private:
  friend class base::DeleteHelper<AudioPipeReader>;
  friend class base::RefCountedThreadSafe<AudioPipeReader>;
  friend struct AudioPipeReaderTraits;

  AudioPipeReader(scoped_refptr<base::SingleThreadTaskRunner> task_runner);
  virtual ~AudioPipeReader();

  void StartOnAudioThread(const base::FilePath& pipe_name);
  void StartTimer();
  void DoCapture();
  void WaitForPipeReadable();

  scoped_refptr<base::SingleThreadTaskRunner> task_runner_;

  int pipe_fd_;
  base::RepeatingTimer<AudioPipeReader> timer_;
  scoped_refptr<ObserverListThreadSafe<StreamObserver> > observers_;

  
  base::TimeTicks started_time_;

  
  
  
  int64 last_capture_position_;

  
  std::string left_over_bytes_;

  base::MessageLoopForIO::FileDescriptorWatcher file_descriptor_watcher_;

  DISALLOW_COPY_AND_ASSIGN(AudioPipeReader);
};

struct AudioPipeReaderTraits {
  static void Destruct(const AudioPipeReader* audio_pipe_reader);
};

}  

#endif  
