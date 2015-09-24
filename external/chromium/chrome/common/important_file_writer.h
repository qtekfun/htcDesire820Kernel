// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_IMPORTANT_FILE_WRITER_H_
#define CHROME_COMMON_IMPORTANT_FILE_WRITER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/threading/non_thread_safe.h"
#include "base/time.h"
#include "base/timer.h"

namespace base {
class MessageLoopProxy;
class Thread;
}

class ImportantFileWriter : public base::NonThreadSafe {
 public:
  
  
  class DataSerializer {
   public:
    virtual ~DataSerializer() {}

    
    
    
    virtual bool SerializeData(std::string* data) = 0;
  };

  
  
  
  
  
  ImportantFileWriter(const FilePath& path,
                      base::MessageLoopProxy* file_message_loop_proxy);

  
  
  ~ImportantFileWriter();

  const FilePath& path() const { return path_; }

  
  
  bool HasPendingWrite() const;

  
  
  void WriteNow(const std::string& data);

  
  
  
  
  
  
  void ScheduleWrite(DataSerializer* serializer);

  
  void DoScheduledWrite();

  base::TimeDelta commit_interval() const {
    return commit_interval_;
  }

  void set_commit_interval(const base::TimeDelta& interval) {
    commit_interval_ = interval;
  }

 private:
  // Path being written to.
  const FilePath path_;

  
  scoped_refptr<base::MessageLoopProxy> file_message_loop_proxy_;

  
  base::OneShotTimer<ImportantFileWriter> timer_;

  
  DataSerializer* serializer_;

  // Time delta after which scheduled data will be written to disk.
  base::TimeDelta commit_interval_;

  DISALLOW_COPY_AND_ASSIGN(ImportantFileWriter);
};

#endif  
