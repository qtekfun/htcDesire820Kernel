// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_DESKTOP_MEDIA_PICKER_H_
#define CHROME_BROWSER_MEDIA_DESKTOP_MEDIA_PICKER_H_

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "content/public/browser/desktop_media_id.h"
#include "ui/gfx/native_widget_types.h"

class DesktopMediaList;

class DesktopMediaPicker {
 public:
  typedef base::Callback<void(content::DesktopMediaID)> DoneCallback;

  
  
  static scoped_ptr<DesktopMediaPicker> Create();

  DesktopMediaPicker() {}
  virtual ~DesktopMediaPicker() {}

  
  
  
  virtual void Show(gfx::NativeWindow context,
                    gfx::NativeWindow parent,
                    const base::string16& app_name,
                    scoped_ptr<DesktopMediaList> list,
                    const DoneCallback& done_callback) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(DesktopMediaPicker);
};

#endif  
