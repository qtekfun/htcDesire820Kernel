// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_NOTIFICATION_REGISTRAR_H_
#define CONTENT_PUBLIC_BROWSER_NOTIFICATION_REGISTRAR_H_

#include <vector>

#include "base/basictypes.h"
#include "base/threading/non_thread_safe.h"
#include "content/common/content_export.h"

namespace content {

class NotificationObserver;
class NotificationSource;

class CONTENT_EXPORT NotificationRegistrar :
    NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  
  
  NotificationRegistrar();
  ~NotificationRegistrar();

  
  void Add(NotificationObserver* observer,
           int type,
           const NotificationSource& source);
  void Remove(NotificationObserver* observer,
              int type,
              const NotificationSource& source);

  
  void RemoveAll();

  
  bool IsEmpty() const;

  
  
  bool IsRegistered(NotificationObserver* observer,
                    int type,
                    const NotificationSource& source);

 private:
  struct Record;

  
  
  
  
  typedef std::vector<Record> RecordVector;

  
  RecordVector registered_;

  DISALLOW_COPY_AND_ASSIGN(NotificationRegistrar);
};

}  

#endif  
