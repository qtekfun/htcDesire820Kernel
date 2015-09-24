// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_AUTOCOMPLETE_AUTOCOMPLETE_RESULT_VIEW_MODEL_H_
#define CHROME_BROWSER_UI_VIEWS_AUTOCOMPLETE_AUTOCOMPLETE_RESULT_VIEW_MODEL_H_
#pragma once

class SkBitmap;

class AutocompleteResultViewModel {
 public:
  
  virtual bool IsSelectedIndex(size_t index) const = 0;

  
  virtual bool IsHoveredIndex(size_t index) const = 0;

  
  
  virtual const SkBitmap* GetIconIfExtensionMatch(size_t index) const = 0;
};

#endif  
