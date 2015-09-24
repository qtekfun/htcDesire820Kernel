// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_FILES_FILE_PATH_WATCHER_H_
#define BASE_FILES_FILE_PATH_WATCHER_H_
#pragma once

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/message_loop_proxy.h"

namespace base {
namespace files {

class FilePathWatcher {
 public:
  
  
  
  class Delegate : public base::RefCountedThreadSafe<Delegate> {
   public:
    virtual ~Delegate() {}
    virtual void OnFilePathChanged(const FilePath& path) = 0;
    
    
    virtual void OnFilePathError(const FilePath& path) {}
  };

  FilePathWatcher();
  ~FilePathWatcher();

  
  
  
  
  bool Watch(const FilePath& path, Delegate* delegate) WARN_UNUSED_RESULT;

  class PlatformDelegate;

  
  
  
  class CancelTask : public Task {
   public:
    CancelTask(PlatformDelegate* delegate): delegate_(delegate) {}
    virtual ~CancelTask() {
      delegate_->CancelOnMessageLoopThread();
    }

    virtual void Run() {
      delegate_->CancelOnMessageLoopThread();
    }
   private:
    scoped_refptr<PlatformDelegate> delegate_;

    DISALLOW_COPY_AND_ASSIGN(CancelTask);
  };

  
  class PlatformDelegate : public base::RefCountedThreadSafe<PlatformDelegate> {
   public:
    PlatformDelegate();

    
    virtual bool Watch(const FilePath& path,
                       Delegate* delegate) WARN_UNUSED_RESULT = 0;

    
    
    
    virtual void Cancel() = 0;

   protected:
    virtual ~PlatformDelegate();

    
    
    
    virtual void CancelOnMessageLoopThread() = 0;

    scoped_refptr<base::MessageLoopProxy> message_loop() const {
      return message_loop_;
    }

    void set_message_loop(base::MessageLoopProxy* loop) {
      message_loop_ = loop;
    }

    
    void set_cancelled() {
      cancelled_ = true;
    }

    bool is_cancelled() const {
      return cancelled_;
    }

   private:
    friend class base::RefCountedThreadSafe<PlatformDelegate>;
    friend class CancelTask;

    scoped_refptr<base::MessageLoopProxy> message_loop_;
    bool cancelled_;
  };

 private:
  scoped_refptr<PlatformDelegate> impl_;

  DISALLOW_COPY_AND_ASSIGN(FilePathWatcher);
};

}  
}  

#endif  
