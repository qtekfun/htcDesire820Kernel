// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_EVENT_RECORDER_H_
#define BASE_EVENT_RECORDER_H_

#include "base/base_export.h"
#include "base/basictypes.h"
#include "build/build_config.h"

#if defined(OS_WIN)
#include <stdio.h>
#include <string.h>
#include <windows.h>
#endif

namespace base {

class FilePath;

class BASE_EXPORT EventRecorder {
 public:
  
  
  static EventRecorder* current() {
    if (!current_)
      current_ = new EventRecorder();
    return current_;
  }

  
  
  
  bool StartRecording(const FilePath& filename);

  
  void StopRecording();

  
  bool is_recording() const { return is_recording_; }

  
  
  bool StartPlayback(const FilePath& filename);

  
  void StopPlayback();

  
  bool is_playing() const { return is_playing_; }

#if defined(OS_WIN)
  
  
  LRESULT RecordWndProc(int nCode, WPARAM wParam, LPARAM lParam);
  LRESULT PlaybackWndProc(int nCode, WPARAM wParam, LPARAM lParam);
#endif

 private:
  
  
  
  EventRecorder()
      : is_recording_(false),
        is_playing_(false),
#if defined(OS_WIN)
        journal_hook_(NULL),
        file_(NULL),
#endif
        playback_first_msg_time_(0),
        playback_start_time_(0) {
#if defined(OS_WIN)
    memset(&playback_msg_, 0, sizeof(playback_msg_));
#endif
  }
  ~EventRecorder();

  static EventRecorder* current_;  

  bool is_recording_;
  bool is_playing_;
#if defined(OS_WIN)
  HHOOK journal_hook_;
  FILE* file_;
  EVENTMSG playback_msg_;
#endif
  int playback_first_msg_time_;
  int playback_start_time_;

  DISALLOW_COPY_AND_ASSIGN(EventRecorder);
};

}  

#endif 
