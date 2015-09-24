// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ICON_LOADER_H_
#define CHROME_BROWSER_ICON_LOADER_H_

#include "build/build_config.h"

#include <string>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop_proxy.h"
#include "ui/gfx/image/image.h"

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
    LARGE,      
    ALL,        
  };

  class Delegate {
   public:
    
    
    
    virtual bool OnGroupLoaded(IconLoader* source,
                               const IconGroupID& group) = 0;
    
    
    
    virtual bool OnImageLoaded(IconLoader* source,
                               gfx::Image* result,
                               const IconGroupID& group) = 0;

   protected:
    virtual ~Delegate() {}
  };

  IconLoader(const base::FilePath& file_path,
             IconSize size,
             Delegate* delegate);

  
  void Start();

 private:
  friend class base::RefCountedThreadSafe<IconLoader>;

  virtual ~IconLoader();

  
  
  static IconGroupID ReadGroupIDFromFilepath(const base::FilePath& path);

  
  static bool IsIconMutableFromFilepath(const base::FilePath& path);

  void ReadGroup();
  void OnReadGroup();
  void ReadIcon();

  void NotifyDelegate();

  
  scoped_refptr<base::MessageLoopProxy> target_message_loop_;

  base::FilePath file_path_;

  IconGroupID group_;

  IconSize icon_size_;

  scoped_ptr<gfx::Image> image_;

  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(IconLoader);
};

#endif  
