// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CAST_LOGGING_LOGGING_RAW_H_
#define MEDIA_CAST_LOGGING_LOGGING_RAW_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/tick_clock.h"
#include "media/cast/logging/logging_defines.h"

namespace media {
namespace cast {

class LoggingRaw : public base::NonThreadSafe,
                   public base::SupportsWeakPtr<LoggingRaw> {
 public:
  explicit LoggingRaw(base::TickClock* clock);
  ~LoggingRaw();

  
  
  void InsertFrameEvent(CastLoggingEvent event,
                        uint32 rtp_timestamp,
                        uint32 frame_id);

  
  void InsertFrameEventWithSize(CastLoggingEvent event,
                                uint32 rtp_timestamp,
                                uint32 frame_id,
                                int frame_size);

  
  void InsertFrameEventWithDelay(CastLoggingEvent event,
                                 uint32 rtp_timestamp,
                                 uint32 frame_id,
                                 base::TimeDelta delay);

  
  void InsertPacketEvent(CastLoggingEvent event,
                         uint32 rtp_timestamp,
                         uint32 frame_id,
                         uint16 packet_id,
                         uint16 max_packet_id,
                         size_t size);

  void InsertGenericEvent(CastLoggingEvent event, int value);

  
  FrameRawMap GetFrameData() const;
  PacketRawMap GetPacketData() const;
  GenericRawMap GetGenericData() const;


  
  void Reset();

 private:
  void InsertBaseFrameEvent(CastLoggingEvent event,
                            uint32 frame_id,
                            uint32 rtp_timestamp);

  base::TickClock* const clock_;  
  FrameRawMap frame_map_;
  PacketRawMap packet_map_;
  GenericRawMap generic_map_;
  base::WeakPtrFactory<LoggingRaw> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(LoggingRaw);
};

}  
}  

#endif  

