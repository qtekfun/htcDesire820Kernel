// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_USER_STYLE_SHEET_WATCHER_H_
#define CHROME_BROWSER_USER_STYLE_SHEET_WATCHER_H_
#pragma once

#include "base/file_path.h"
#include "base/files/file_path_watcher.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "content/browser/browser_thread.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "googleurl/src/gurl.h"

class UserStyleSheetLoader;

class UserStyleSheetWatcher
    : public base::RefCountedThreadSafe<UserStyleSheetWatcher,
                                        BrowserThread::DeleteOnUIThread>,
      public NotificationObserver {
 public:
  explicit UserStyleSheetWatcher(const FilePath& profile_path);

  void Init();

  GURL user_style_sheet() const;

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  friend struct BrowserThread::DeleteOnThread<BrowserThread::UI>;
  friend class DeleteTask<UserStyleSheetWatcher>;

  virtual ~UserStyleSheetWatcher();

  
  FilePath profile_path_;

  
  scoped_refptr<UserStyleSheetLoader> loader_;

  
  scoped_ptr<base::files::FilePathWatcher> file_watcher_;

  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(UserStyleSheetWatcher);
};

#endif  
