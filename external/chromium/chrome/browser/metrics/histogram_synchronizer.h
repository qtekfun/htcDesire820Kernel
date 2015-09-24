// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_METRICS_HISTOGRAM_SYNCHRONIZER_H_
#define CHROME_BROWSER_METRICS_HISTOGRAM_SYNCHRONIZER_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/condition_variable.h"
#include "base/synchronization/lock.h"
#include "base/time.h"

class MessageLoop;
class Task;


class HistogramSynchronizer : public
    base::RefCountedThreadSafe<HistogramSynchronizer> {
 public:

  enum RendererHistogramRequester {
    ASYNC_HISTOGRAMS,
    SYNCHRONOUS_HISTOGRAMS
  };

  
  
  
  
  HistogramSynchronizer();

  ~HistogramSynchronizer();

  
  
  static HistogramSynchronizer* CurrentSynchronizer();

  
  
  
  
  void FetchRendererHistogramsSynchronously(base::TimeDelta wait_time);

  
  
  
  
  static void FetchRendererHistogramsAsynchronously(
      MessageLoop* callback_thread, Task* callback_task, int wait_time);

  
  
  static void DeserializeHistogramList(
      int sequence_number, const std::vector<std::string>& histograms);

 private:
  
  
  
  
  
  int NotifyAllRenderers(RendererHistogramRequester requester);

  
  
  
  void DecrementPendingRenderers(int sequence_number);

  
  
  
  
  
  void SetCallbackTaskAndThread(MessageLoop* callback_thread,
                                Task* callback_task);

  void ForceHistogramSynchronizationDoneCallback(int sequence_number);

  
  
  int GetNextAvailableSequenceNumber(RendererHistogramRequester requster,
                                     int renderer_count);

  
  void InternalPostTask(MessageLoop* thread, Task* task,
      int unresponsive_renderers, const base::TimeTicks& started);

  
  base::Lock lock_;

  
  
  base::ConditionVariable received_all_renderer_histograms_;

  
  
  
  Task* callback_task_;
  MessageLoop* callback_thread_;

  
  
  
  
  
  
  
  
  int last_used_sequence_number_;

  
  
  int async_sequence_number_;

  
  
  int async_renderers_pending_;

  
  
  base::TimeTicks async_callback_start_time_;

  
  
  int synchronous_sequence_number_;

  
  
  int synchronous_renderers_pending_;

  
  
  
  
  
  static HistogramSynchronizer* histogram_synchronizer_;

  DISALLOW_COPY_AND_ASSIGN(HistogramSynchronizer);
};

#endif  
