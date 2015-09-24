// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TAB_CONTENTS_TAB_CONTENTS_DRAG_WIN_H_
#define CHROME_BROWSER_UI_VIEWS_TAB_CONTENTS_TAB_CONTENTS_DRAG_WIN_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/platform_thread.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebDragOperation.h"
#include "ui/base/dragdrop/os_exchange_data_provider_win.h"
#include "ui/gfx/point.h"

class DragDropThread;
class NativeTabContentsViewWin;
class WebDragSource;
struct WebDropData;

class TabContentsDragWin
    : public ui::DataObjectImpl::Observer,
      public base::RefCountedThreadSafe<TabContentsDragWin> {
 public:
  explicit TabContentsDragWin(NativeTabContentsViewWin* view);
  virtual ~TabContentsDragWin();

  
  void StartDragging(const WebDropData& drop_data,
                     WebKit::WebDragOperationsMask ops,
                     const SkBitmap& image,
                     const gfx::Point& image_offset);
  void CancelDrag();

  
  
  virtual void OnWaitForData();
  virtual void OnDataObjectDisposed();

 private:
  
  void PrepareDragForDownload(const WebDropData& drop_data,
                              ui::OSExchangeData* data,
                              const GURL& page_url,
                              const std::string& page_encoding);
  void PrepareDragForFileContents(const WebDropData& drop_data,
                                  ui::OSExchangeData* data);
  void PrepareDragForUrl(const WebDropData& drop_data,
                         ui::OSExchangeData* data);
  void DoDragging(const WebDropData& drop_data,
                  WebKit::WebDragOperationsMask ops,
                  const GURL& page_url,
                  const std::string& page_encoding,
                  const SkBitmap& image,
                  const gfx::Point& image_offset);

  
  void StartBackgroundDragging(const WebDropData& drop_data,
                               WebKit::WebDragOperationsMask ops,
                               const GURL& page_url,
                               const std::string& page_encoding,
                               const SkBitmap& image,
                               const gfx::Point& image_offset);
  
  void EndDragging(bool restore_suspended_state);
  void CloseThread();

  
  base::PlatformThreadId drag_drop_thread_id_;

  

  
  NativeTabContentsViewWin* view_;

  
  
  
  scoped_refptr<WebDragSource> drag_source_;

  
  
  scoped_ptr<DragDropThread> drag_drop_thread_;

  
  bool drag_ended_;

  
  bool old_drop_target_suspended_state_;

  DISALLOW_COPY_AND_ASSIGN(TabContentsDragWin);
};


#endif  
