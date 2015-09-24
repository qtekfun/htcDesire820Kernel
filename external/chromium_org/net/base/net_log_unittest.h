// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_NET_LOG_UNITTEST_H_
#define NET_BASE_NET_LOG_UNITTEST_H_

#include <cstddef>

#include "net/base/capturing_net_log.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace net {

inline base::TimeTicks MakeTime(int t) {
  base::TimeTicks ticks;  
  ticks += base::TimeDelta::FromMilliseconds(t);
  return ticks;
}

inline ::testing::AssertionResult LogContainsEventHelper(
    const CapturingNetLog::CapturedEntryList& entries,
    int i,  
    const base::TimeTicks& expected_time,
    bool check_time,
    NetLog::EventType expected_event,
    NetLog::EventPhase expected_phase) {
  
  size_t j = (i < 0) ?
      static_cast<size_t>(static_cast<int>(entries.size()) + i) :
      static_cast<size_t>(i);
  if (j >= entries.size())
    return ::testing::AssertionFailure() << j << " is out of bounds.";
  const CapturingNetLog::CapturedEntry& entry = entries[j];
  if (expected_event != entry.type) {
    return ::testing::AssertionFailure()
        << "Actual event: " << NetLog::EventTypeToString(entry.type)
        << ". Expected event: " << NetLog::EventTypeToString(expected_event)
        << ".";
  }
  if (expected_phase != entry.phase) {
    return ::testing::AssertionFailure()
        << "Actual phase: " << entry.phase
        << ". Expected phase: " << expected_phase << ".";
  }
  if (check_time) {
    if (expected_time != entry.time) {
      return ::testing::AssertionFailure()
          << "Actual time: " << entry.time.ToInternalValue()
          << ". Expected time: " << expected_time.ToInternalValue()
          << ".";
    }
  }
  return ::testing::AssertionSuccess();
}

inline ::testing::AssertionResult LogContainsEventAtTime(
    const CapturingNetLog::CapturedEntryList& log,
    int i,  
    const base::TimeTicks& expected_time,
    NetLog::EventType expected_event,
    NetLog::EventPhase expected_phase) {
  return LogContainsEventHelper(log, i, expected_time, true,
                                expected_event, expected_phase);
}

inline ::testing::AssertionResult LogContainsEvent(
    const CapturingNetLog::CapturedEntryList& log,
    int i,  
    NetLog::EventType expected_event,
    NetLog::EventPhase expected_phase) {
  return LogContainsEventHelper(log, i, base::TimeTicks(), false,
                                expected_event, expected_phase);
}

inline ::testing::AssertionResult LogContainsBeginEvent(
    const CapturingNetLog::CapturedEntryList& log,
    int i,  
    NetLog::EventType expected_event) {
  return LogContainsEvent(log, i, expected_event, NetLog::PHASE_BEGIN);
}

inline ::testing::AssertionResult LogContainsEndEvent(
    const CapturingNetLog::CapturedEntryList& log,
    int i,  
    NetLog::EventType expected_event) {
  return LogContainsEvent(log, i, expected_event, NetLog::PHASE_END);
}

inline ::testing::AssertionResult LogContainsEntryWithType(
    const CapturingNetLog::CapturedEntryList& entries,
    int i, 
    NetLog::EventType type) {
  
  size_t j = (i < 0) ?
      static_cast<size_t>(static_cast<int>(entries.size()) + i) :
      static_cast<size_t>(i);
  if (j >= entries.size())
    return ::testing::AssertionFailure() << j << " is out of bounds.";
  const CapturingNetLog::CapturedEntry& entry = entries[j];
  if (entry.type != type)
    return ::testing::AssertionFailure() << "Type does not match.";
  return ::testing::AssertionSuccess();
}

inline ::testing::AssertionResult LogContainsEntryWithTypeAfter(
    const CapturingNetLog::CapturedEntryList& entries,
    int min_index, 
    NetLog::EventType type) {
  
  size_t real_index = (min_index < 0) ?
      static_cast<size_t>(static_cast<int>(entries.size()) + min_index) :
      static_cast<size_t>(min_index);
  for (size_t i = real_index; i < entries.size(); ++i) {
    const CapturingNetLog::CapturedEntry& entry = entries[i];
    if (entry.type == type)
      return ::testing::AssertionSuccess();
  }
  return ::testing::AssertionFailure();
}

inline size_t ExpectLogContainsSomewhere(
    const CapturingNetLog::CapturedEntryList& entries,
    size_t min_index,
    NetLog::EventType expected_event,
    NetLog::EventPhase expected_phase) {
  size_t i = 0;
  for (; i < entries.size(); ++i) {
    const CapturingNetLog::CapturedEntry& entry = entries[i];
    if (entry.type == expected_event &&
        entry.phase == expected_phase)
      break;
  }
  EXPECT_LT(i, entries.size());
  EXPECT_GE(i, min_index);
  return i;
}

inline size_t ExpectLogContainsSomewhereAfter(
    const CapturingNetLog::CapturedEntryList& entries,
    size_t min_index,
    NetLog::EventType expected_event,
    NetLog::EventPhase expected_phase) {
  size_t i = min_index;
  for (; i < entries.size(); ++i) {
    const CapturingNetLog::CapturedEntry& entry = entries[i];
    if (entry.type == expected_event &&
        entry.phase == expected_phase)
      break;
  }
  EXPECT_LT(i, entries.size());
  return i;
}

}  

#endif  
