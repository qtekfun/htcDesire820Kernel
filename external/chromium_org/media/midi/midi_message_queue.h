// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MIDI_MIDI_MESSAGE_QUEUE_H_
#define MEDIA_MIDI_MIDI_MESSAGE_QUEUE_H_

#include <deque>
#include <vector>

#include "base/basictypes.h"
#include "media/base/media_export.h"

namespace media {

class MEDIA_EXPORT MIDIMessageQueue {
 public:
  
  
  explicit MIDIMessageQueue(bool allow_running_status);
  ~MIDIMessageQueue();

  
  void Add(const std::vector<uint8>& data);
  void Add(const uint8* data, size_t length);

  
  
  
  
  
  
  
  
  
  void Get(std::vector<uint8>* message);

 private:
  std::deque<uint8> queue_;
  std::vector<uint8> next_message_;
  const bool allow_running_status_;
  DISALLOW_COPY_AND_ASSIGN(MIDIMessageQueue);
};

}  

#endif  
