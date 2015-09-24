// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ICON_MANAGER_H_
#define CHROME_BROWSER_ICON_MANAGER_H_

#include <map>

#include "base/files/file_path.h"
#include "chrome/browser/icon_loader.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "ui/gfx/image/image.h"

class IconManager : public IconLoader::Delegate {
 public:
  IconManager();
  virtual ~IconManager();

  
  
  
  
  
  gfx::Image* LookupIconFromFilepath(const base::FilePath& file_name,
                                     IconLoader::IconSize size);

  typedef base::Callback<void(gfx::Image*)> IconRequestCallback;

  
  
  
  
  
  
  
  
  
  
  CancelableTaskTracker::TaskId LoadIcon(const base::FilePath& file_name,
                                         IconLoader::IconSize size,
                                         const IconRequestCallback& callback,
                                         CancelableTaskTracker* tracker);

  
  virtual bool OnGroupLoaded(IconLoader* loader,
                             const IconGroupID& group) OVERRIDE;
  virtual bool OnImageLoaded(IconLoader* loader,
                             gfx::Image* result,
                             const IconGroupID& group) OVERRIDE;

 private:
  struct CacheKey {
    CacheKey(const IconGroupID& group, IconLoader::IconSize size);

    
    bool operator<(const CacheKey &other) const;

    IconGroupID group;
    IconLoader::IconSize size;
  };

  gfx::Image* LookupIconFromGroup(const IconGroupID& group,
                                  IconLoader::IconSize size);

  typedef std::map<CacheKey, gfx::Image*> IconMap;
  IconMap icon_cache_;

  typedef std::map<base::FilePath, IconGroupID> GroupMap;
  GroupMap group_cache_;

  
  struct ClientRequest;
  typedef std::map<IconLoader*, ClientRequest> ClientRequests;
  ClientRequests requests_;

  DISALLOW_COPY_AND_ASSIGN(IconManager);
};

#endif  
