// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INSTANT_INSTANT_DELEGATE_H_
#define CHROME_BROWSER_INSTANT_INSTANT_DELEGATE_H_
#pragma once

#include "base/string16.h"
#include "chrome/common/instant_types.h"

class TabContentsWrapper;

namespace gfx {
class Rect;
}

class InstantDelegate {
 public:
  
  
  
  virtual void PrepareForInstant() = 0;

  
  virtual void ShowInstant(TabContentsWrapper* preview_contents) = 0;

  
  
  
  virtual void HideInstant() = 0;

  
  
  
  virtual void CommitInstant(TabContentsWrapper* preview_contents) = 0;

  
  virtual void SetSuggestedText(const string16& text,
                                InstantCompleteBehavior behavior) = 0;

  
  virtual gfx::Rect GetInstantBounds() = 0;

 protected:
  virtual ~InstantDelegate() {}
};

#endif  
