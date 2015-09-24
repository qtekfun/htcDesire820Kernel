// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INSTANT_INSTANT_LOADER_MANAGER_H_
#define CHROME_BROWSER_INSTANT_INSTANT_LOADER_MANAGER_H_
#pragma once

#include <map>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/search_engines/template_url_id.h"

class InstantLoader;
class InstantLoaderDelegate;

class InstantLoaderManager {
 public:
  explicit InstantLoaderManager(InstantLoaderDelegate* loader_delegate);
  ~InstantLoaderManager();

  
  
  
  
  InstantLoader* UpdateLoader(TemplateURLID instant_id,
                              scoped_ptr<InstantLoader>* old_loader);

  
  
  bool WillUpateChangeActiveLoader(TemplateURLID instant_id);

  
  
  void MakePendingCurrent(scoped_ptr<InstantLoader>* old_loader);

  
  
  
  InstantLoader* ReleaseCurrentLoader();

  
  void DestroyLoader(InstantLoader* loader);

  
  InstantLoader* ReleaseLoader(InstantLoader* loader);

  
  void RemoveLoaderFromInstant(InstantLoader* loader);

  
  InstantLoader* current_loader() const { return current_loader_; }

  
  InstantLoader* pending_loader() const { return pending_loader_; }

  
  
  InstantLoader* active_loader() const {
    return pending_loader_ ? pending_loader_ : current_loader_;
  }

  
  
  size_t num_instant_loaders() const { return instant_loaders_.size(); }

 private:
  typedef std::map<TemplateURLID, InstantLoader*> Loaders;

  
  InstantLoader* CreateLoader(TemplateURLID id);

  
  
  InstantLoader* GetInstantLoader(TemplateURLID id);

  InstantLoaderDelegate* loader_delegate_;

  
  InstantLoader* current_loader_;

  
  
  
  InstantLoader* pending_loader_;

  
  Loaders instant_loaders_;

  DISALLOW_COPY_AND_ASSIGN(InstantLoaderManager);
};

#endif  
