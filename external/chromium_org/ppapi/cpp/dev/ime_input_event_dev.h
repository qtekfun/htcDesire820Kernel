// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DEV_IME_INPUT_EVENT_DEV_H_
#define PPAPI_CPP_DEV_IME_INPUT_EVENT_DEV_H_

#include <utility>
#include <vector>

#include "ppapi/c/dev/ppb_ime_input_event_dev.h"
#include "ppapi/cpp/input_event.h"


namespace pp {

class Var;

class IMEInputEvent_Dev : public InputEvent {
 public:
  
  IMEInputEvent_Dev();

  
  
  
  
  
  explicit IMEInputEvent_Dev(const InputEvent& event);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  IMEInputEvent_Dev(const InstanceHandle& instance,
                    PP_InputEvent_Type type,
                    PP_TimeTicks time_stamp,
                    const Var& text,
                    const std::vector<uint32_t>& segment_offsets,
                    int32_t target_segment,
                    const std::pair<uint32_t, uint32_t>& selection);

  
  
  
  
  Var GetText() const;

  
  
  
  
  uint32_t GetSegmentNumber() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  uint32_t GetSegmentOffset(uint32_t index) const;

  
  
  
  
  
  int32_t GetTargetSegment() const;

  
  
  
  std::pair<uint32_t, uint32_t> GetSelection() const;
};

}  

#endif  
