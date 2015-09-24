// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NATIVE_WINDOW_NOTIFICATION_SOURCE_H_
#define CHROME_BROWSER_NATIVE_WINDOW_NOTIFICATION_SOURCE_H_

#include "content/public/browser/notification_source.h"
#include "ui/gfx/native_widget_types.h"

namespace content {

template<>
class Source<gfx::NativeWindow> : public content::NotificationSource {
 public:
  explicit Source(gfx::NativeWindow wnd) : content::NotificationSource(wnd) {}

  explicit Source(const content::NotificationSource& other)
      : content::NotificationSource(other) {}

  gfx::NativeWindow operator->() const { return ptr(); }
  gfx::NativeWindow ptr() const {
    return static_cast<gfx::NativeWindow>(const_cast<void*>(ptr_));
  }
};

}

#endif  