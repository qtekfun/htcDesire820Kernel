// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WEB_CONTENTS_WEB_CONTENTS_DRAG_WIN_H_
#define CONTENT_BROWSER_WEB_CONTENTS_WEB_CONTENTS_DRAG_WIN_H_

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/platform_thread.h"
#include "content/common/content_export.h"
#include "third_party/WebKit/public/web/WebDragOperation.h"
#include "ui/base/dragdrop/os_exchange_data_provider_win.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"

class SkBitmap;

namespace gfx {
class ImageSkia;
}

namespace content {
class DragDropThread;
class WebContents;
class WebDragDest;
class WebDragSource;
struct DropData;

class CONTENT_EXPORT WebContentsDragWin
    : NON_EXPORTED_BASE(public ui::DataObjectImpl::Observer),
      public base::RefCountedThreadSafe<WebContentsDragWin> {
 public:
  WebContentsDragWin(gfx::NativeWindow source_window,
                     WebContents* web_contents,
                     WebDragDest* drag_dest,
                     const base::Callback<void()>& drag_end_callback);
  virtual ~WebContentsDragWin();

  
  void StartDragging(const DropData& drop_data,
                     blink::WebDragOperationsMask ops,
                     const gfx::ImageSkia& image,
                     const gfx::Vector2d& image_offset);
  void CancelDrag();

  
  
  virtual void OnWaitForData();
  virtual void OnDataObjectDisposed();

  
  static void DisableDragDropForTesting();

 private:
  
  void PrepareDragForDownload(const DropData& drop_data,
                              ui::OSExchangeData* data,
                              const GURL& page_url,
                              const std::string& page_encoding);
  void PrepareDragForFileContents(const DropData& drop_data,
                                  ui::OSExchangeData* data);
  void PrepareDragForUrl(const DropData& drop_data,
                         ui::OSExchangeData* data);
  
  
  
  
  bool DoDragging(const DropData& drop_data,
                  blink::WebDragOperationsMask ops,
                  const GURL& page_url,
                  const std::string& page_encoding,
                  const gfx::ImageSkia& image,
                  const gfx::Vector2d& image_offset);

  
  void StartBackgroundDragging(const DropData& drop_data,
                               blink::WebDragOperationsMask ops,
                               const GURL& page_url,
                               const std::string& page_encoding,
                               const gfx::ImageSkia& image,
                               const gfx::Vector2d& image_offset);
  
  void EndDragging();
  void CloseThread();

  
  base::PlatformThreadId drag_drop_thread_id_;

  

  gfx::NativeWindow source_window_;
  WebContents* web_contents_;
  WebDragDest* drag_dest_;

  
  
  
  scoped_refptr<WebDragSource> drag_source_;

  
  
  scoped_ptr<DragDropThread> drag_drop_thread_;

  
  bool drag_ended_;

  base::Callback<void()> drag_end_callback_;

  DISALLOW_COPY_AND_ASSIGN(WebContentsDragWin);
};

}  

#endif  
