// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_DATA_DELETER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_DATA_DELETER_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/string16.h"
#include "content/browser/browser_thread.h"
#include "googleurl/src/gurl.h"

namespace webkit_database {
class DatabaseTracker;
}

namespace fileapi {
class FileSystemContext;
}

class Profile;
class WebKitContext;

namespace net {
class URLRequestContextGetter;
}

class ExtensionDataDeleter
  : public base::RefCountedThreadSafe<ExtensionDataDeleter,
                                      BrowserThread::DeleteOnUIThread> {
 public:
  ExtensionDataDeleter(Profile* profile, const GURL& extension_url);

  
  
  
  
  void StartDeleting();

 private:
  friend struct BrowserThread::DeleteOnThread<BrowserThread::UI>;
  friend class DeleteTask<ExtensionDataDeleter>;

  ~ExtensionDataDeleter();

  
  
  void DeleteCookiesOnIOThread();

  
  
  void DeleteDatabaseOnFileThread();

  
  
  void DeleteLocalStorageOnWebkitThread();

  
  
  void DeleteIndexedDBOnWebkitThread();

  
  
  void DeleteFileSystemOnFileThread();

  
  scoped_refptr<webkit_database::DatabaseTracker> database_tracker_;

  
  scoped_refptr<net::URLRequestContextGetter> extension_request_context_;

  
  GURL extension_url_;

  
  string16 origin_id_;

  
  scoped_refptr<WebKitContext> webkit_context_;

  scoped_refptr<fileapi::FileSystemContext> file_system_context_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionDataDeleter);
};

#endif  
