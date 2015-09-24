// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_PUBLIC_TEST_TEST_BROWSER_THREAD_TEST_BUNDLE_H_
#define CONTENT_PUBLIC_TEST_TEST_BROWSER_THREAD_TEST_BUNDLE_H_

#include "base/memory/scoped_ptr.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace base {
class MessageLoop;
}  

namespace content {

class TestBrowserThread;

class TestBrowserThreadBundle {
 public:
  
  
  
  enum Options {
    DEFAULT = 0x00,
    IO_MAINLOOP = 0x01,
    REAL_DB_THREAD = 0x02,
    REAL_FILE_THREAD = 0x08,
    REAL_FILE_USER_BLOCKING_THREAD = 0x10,
    REAL_PROCESS_LAUNCHER_THREAD = 0x20,
    REAL_CACHE_THREAD = 0x40,
    REAL_IO_THREAD = 0x80,
  };

  TestBrowserThreadBundle();
  explicit TestBrowserThreadBundle(int options);

  ~TestBrowserThreadBundle();

 private:
  void Init(int options);

  scoped_ptr<base::MessageLoop> message_loop_;
  scoped_ptr<TestBrowserThread> ui_thread_;
  scoped_ptr<TestBrowserThread> db_thread_;
  scoped_ptr<TestBrowserThread> file_thread_;
  scoped_ptr<TestBrowserThread> file_user_blocking_thread_;
  scoped_ptr<TestBrowserThread> process_launcher_thread_;
  scoped_ptr<TestBrowserThread> cache_thread_;
  scoped_ptr<TestBrowserThread> io_thread_;

  DISALLOW_COPY_AND_ASSIGN(TestBrowserThreadBundle);
};

}  

#endif 
