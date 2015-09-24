// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_IAPPS_DATA_PROVIDER_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_IAPPS_DATA_PROVIDER_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/files/file_path_watcher.h"
#include "base/memory/weak_ptr.h"

namespace iapps {

class IAppsDataProvider {
 public:
  typedef base::Callback<void(bool)> ReadyCallback;

  explicit IAppsDataProvider(const base::FilePath& library_path);
  virtual ~IAppsDataProvider();

  
  
  virtual void RefreshData(const ReadyCallback& ready_callback);

  
  const base::FilePath& library_path() const;

 protected:
  bool valid() const;

  
  void set_valid(bool valid);

  
  
  virtual void DoParseLibrary(const base::FilePath& library_path,
                              const ReadyCallback& ready_callback) = 0;

  
  virtual void OnLibraryChanged(const base::FilePath& path, bool error);

 private:
  
  
  void OnLibraryWatchStarted(scoped_ptr<base::FilePathWatcher> library_watcher);

  
  const base::FilePath library_path_;

  
  bool needs_refresh_;

  
  
  bool is_valid_;

  
  scoped_ptr<base::FilePathWatcher> library_watcher_;

  base::WeakPtrFactory<IAppsDataProvider> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(IAppsDataProvider);
};

}  

#endif  
