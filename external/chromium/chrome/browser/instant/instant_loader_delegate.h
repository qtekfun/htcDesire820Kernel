// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INSTANT_INSTANT_LOADER_DELEGATE_H_
#define CHROME_BROWSER_INSTANT_INSTANT_LOADER_DELEGATE_H_
#pragma once

#include "base/string16.h"
#include "chrome/common/instant_types.h"

class GURL;

namespace gfx {
class Rect;
}

class InstantLoader;

class InstantLoaderDelegate {
 public:
  
  virtual void InstantStatusChanged(InstantLoader* loader) = 0;

  
  virtual void SetSuggestedTextFor(
      InstantLoader* loader,
      const string16& text,
      InstantCompleteBehavior behavior) = 0;

  
  virtual gfx::Rect GetInstantBounds() = 0;

  
  virtual bool ShouldCommitInstantOnMouseUp() = 0;

  
  virtual void CommitInstantLoader(InstantLoader* loader) = 0;

  
  
  virtual void InstantLoaderDoesntSupportInstant(InstantLoader* loader) = 0;

  
  virtual void AddToBlacklist(InstantLoader* loader, const GURL& url) = 0;

 protected:
  virtual ~InstantLoaderDelegate() {}
};

#endif  
