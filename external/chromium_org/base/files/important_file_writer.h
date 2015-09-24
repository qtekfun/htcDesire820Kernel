// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_FILES_IMPORTANT_FILE_WRITER_H_
#define BASE_FILES_IMPORTANT_FILE_WRITER_H_

#include <string>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "base/timer/timer.h"

namespace base {

class SequencedTaskRunner;
class Thread;

class BASE_EXPORT ImportantFileWriter : public NonThreadSafe {
 public:
  
  
  class BASE_EXPORT DataSerializer {
   public:
    
    
    
    virtual bool SerializeData(std::string* data) = 0;

   protected:
    virtual ~DataSerializer() {}
  };

  
  
  static bool WriteFileAtomically(const FilePath& path,
                                  const std::string& data);

  
  
  
  
  
  ImportantFileWriter(const FilePath& path,
                      base::SequencedTaskRunner* task_runner);

  
  
  ~ImportantFileWriter();

  const FilePath& path() const { return path_; }

  
  
  bool HasPendingWrite() const;

  
  
  void WriteNow(const std::string& data);

  
  
  
  
  
  
  void ScheduleWrite(DataSerializer* serializer);

  
  void DoScheduledWrite();

  TimeDelta commit_interval() const {
    return commit_interval_;
  }

  void set_commit_interval(const TimeDelta& interval) {
    commit_interval_ = interval;
  }

 private:
  // Path being written to.
  const FilePath path_;

  
  const scoped_refptr<base::SequencedTaskRunner> task_runner_;

  
  OneShotTimer<ImportantFileWriter> timer_;

  
  DataSerializer* serializer_;

  // Time delta after which scheduled data will be written to disk.
  TimeDelta commit_interval_;

  DISALLOW_COPY_AND_ASSIGN(ImportantFileWriter);
};

}  

#endif  
