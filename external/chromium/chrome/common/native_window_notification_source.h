// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_NATIVE_WINDOW_NOTIFICATION_SOURCE_H_
#define CHROME_COMMON_NATIVE_WINDOW_NOTIFICATION_SOURCE_H_
#pragma once

#include "content/common/notification_source.h"
#include "ui/gfx/native_widget_types.h"

template<>
class Source<gfx::NativeWindow> : public NotificationSource {
 public:
  explicit Source(gfx::NativeWindow wnd) : NotificationSource(wnd) {}

  explicit Source(const NotificationSource& other)
      : NotificationSource(other) {}

  gfx::NativeWindow operator->() const { return ptr(); }
  gfx::NativeWindow ptr() const {
    return static_cast<gfx::NativeWindow>(const_cast<void*>(ptr_));
  }
};

#endif  
