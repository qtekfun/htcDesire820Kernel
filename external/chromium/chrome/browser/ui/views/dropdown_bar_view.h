// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_DROPDOWN_BAR_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_DROPDOWN_BAR_VIEW_H_
#pragma once

#include "views/view.h"

class DropdownBarHost;

class DropdownBarView : public views::View {
 public:
  explicit DropdownBarView(DropdownBarHost* host)
      : host_(host),
        animation_offset_(0) {
  }
  virtual ~DropdownBarView() {}

  
  virtual void SetFocusAndSelection(bool select_all) = 0;

  
  
  void set_animation_offset(int offset) { animation_offset_ = offset; }

  
  int animation_offset() const { return animation_offset_; }

 protected:
  
  DropdownBarHost* host() const { return host_; }

 private:
  
  DropdownBarHost* host_;

  
  
  
  
  int animation_offset_;

  DISALLOW_COPY_AND_ASSIGN(DropdownBarView);
};

#endif  
