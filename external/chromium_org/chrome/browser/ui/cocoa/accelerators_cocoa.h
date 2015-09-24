// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_ACCELERATORS_COCOA_H_
#define CHROME_BROWSER_UI_COCOA_ACCELERATORS_COCOA_H_

#include <map>

#include "ui/base/accelerators/accelerator.h"

template <typename T> struct DefaultSingletonTraits;

class AcceleratorsCocoa {
 public:
  typedef std::map<int, ui::Accelerator> AcceleratorMap;
  typedef AcceleratorMap::const_iterator const_iterator;

  const_iterator const begin() { return accelerators_.begin(); }
  const_iterator const end() { return accelerators_.end(); }

  
  const ui::Accelerator* GetAcceleratorForCommand(int command_id);

  
  static AcceleratorsCocoa* GetInstance();

 private:
  friend struct DefaultSingletonTraits<AcceleratorsCocoa>;

  AcceleratorsCocoa();
  ~AcceleratorsCocoa();

  AcceleratorMap accelerators_;

  DISALLOW_COPY_AND_ASSIGN(AcceleratorsCocoa);
};

#endif  
