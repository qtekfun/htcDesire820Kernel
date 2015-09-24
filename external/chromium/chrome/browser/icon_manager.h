// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ICON_MANAGER_H_
#define CHROME_BROWSER_ICON_MANAGER_H_
#pragma once

#include <map>

#include "base/callback.h"
#include "base/hash_tables.h"
#include "chrome/browser/icon_loader.h"
#include "content/browser/cancelable_request.h"
#include "ui/gfx/image.h"

class FilePath;

class IconManager : public IconLoader::Delegate,
                    public CancelableRequestProvider {
 public:
  IconManager();
  ~IconManager();

  
  
  
  
  
  gfx::Image* LookupIcon(const FilePath& file_name,
                         IconLoader::IconSize size);

  typedef CancelableRequestProvider::Handle Handle;
  typedef Callback2<Handle, gfx::Image*>::Type IconRequestCallback;

  
  
  
  
  
  
  Handle LoadIcon(const FilePath& file_name,
                  IconLoader::IconSize size,
                  CancelableRequestConsumerBase* consumer,
                  IconRequestCallback* callback);

  
  virtual bool OnImageLoaded(IconLoader* source, gfx::Image* result);

  
  
  static IconGroupID GetGroupIDFromFilepath(const FilePath& path);

 private:
  struct CacheKey {
    CacheKey(const IconGroupID& group, IconLoader::IconSize size);

    
    bool operator<(const CacheKey &other) const;

    IconGroupID group;
    IconLoader::IconSize size;
  };

  typedef std::map<CacheKey, gfx::Image*> IconMap;
  IconMap icon_cache_;

  typedef CancelableRequest<IconRequestCallback> IconRequest;

  
  struct ClientRequest;
  typedef std::map<IconLoader*, ClientRequest> ClientRequests;
  ClientRequests requests_;

  DISALLOW_COPY_AND_ASSIGN(IconManager);
};

#endif  
