// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ICON_LOADER_H_
#define CHROME_BROWSER_ICON_LOADER_H_
#pragma once

#include "build/build_config.h"

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop_proxy.h"
#include "ui/gfx/image.h"

#if defined(TOOLKIT_USES_GTK)
#include "base/file_path.h"
#endif

#if defined(OS_WIN)
typedef std::wstring IconGroupID;
#elif defined(OS_POSIX)
typedef std::string IconGroupID;
#endif

class IconLoader : public base::RefCountedThreadSafe<IconLoader> {
 public:
  enum IconSize {
    SMALL = 0,  
    NORMAL,     
    LARGE
  };

  class Delegate {
   public:
    
    
    
    virtual bool OnImageLoaded(IconLoader* source, gfx::Image* result) = 0;

   protected:
    virtual ~Delegate() {}
  };

  IconLoader(const IconGroupID& group, IconSize size, Delegate* delegate);

  
  void Start();

 private:
  friend class base::RefCountedThreadSafe<IconLoader>;

  virtual ~IconLoader();

  void ReadIcon();

  void NotifyDelegate();

  
  scoped_refptr<base::MessageLoopProxy> target_message_loop_;

  IconGroupID group_;

  IconSize icon_size_;

  scoped_ptr<gfx::Image> image_;

  Delegate* delegate_;

#if defined(TOOLKIT_USES_GTK)
  
  
  
  void ParseIcon();
  FilePath filename_;
  std::string icon_data_;
#endif

  DISALLOW_COPY_AND_ASSIGN(IconLoader);
};

#endif  
