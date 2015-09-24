// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_FILE_PATH_WATCHER_UTIL_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_FILE_PATH_WATCHER_UTIL_H_

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/files/file_path_watcher.h"

typedef base::Callback<void(scoped_ptr<base::FilePathWatcher> watcher)>
    FileWatchStartedCallback;

void StartFilePathWatchOnMediaTaskRunner(
    const base::FilePath& path,
    const FileWatchStartedCallback& watch_started_callback,
    const base::FilePathWatcher::Callback& path_changed_callback);

#endif  
