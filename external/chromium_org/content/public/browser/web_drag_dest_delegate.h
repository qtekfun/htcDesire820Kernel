// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_WEB_DRAG_DEST_DELEGATE_H_
#define CONTENT_PUBLIC_BROWSER_WEB_DRAG_DEST_DELEGATE_H_

#if defined(TOOLKIT_GTK)
#include <gtk/gtk.h>
#endif  

#include "base/strings/string16.h"

#if defined(OS_WIN)
#include "ui/base/dragdrop/drop_target_win.h"
#endif

class GURL;

namespace ui {
class OSExchangeData;
}

namespace content {
class WebContents;
struct DropData;

class WebDragDestDelegate {
 public:
  
  
  
  virtual void DragInitialize(WebContents* contents) = 0;

  
#if defined(OS_WIN) && !defined(USE_AURA)
  virtual void OnDragOver(IDataObject* data_object) = 0;
  virtual void OnDragEnter(IDataObject* data_object) = 0;
  virtual void OnDrop(IDataObject* data_object) = 0;
  virtual void OnDragLeave(IDataObject* data_object) = 0;
#else
  virtual void OnDragOver() = 0;
  virtual void OnDragEnter() = 0;
  virtual void OnDrop() = 0;
  
  virtual void OnDragLeave() = 0;
#endif

#if defined(TOOLKIT_GTK)
  
  virtual GdkAtom GetBookmarkTargetAtom() const = 0;

  
  
  virtual void OnReceiveDataFromGtk(GtkSelectionData* data) = 0;
  virtual void OnReceiveProcessedData(const GURL& url,
                                      const base::string16& title) = 0;
#elif defined(USE_AURA)
  
  
  virtual void OnReceiveDragData(const ui::OSExchangeData& data) = 0;
#elif defined(OS_WIN)
  
  
  virtual bool AddDragData(const DropData& drop_data,
                           ui::OSExchangeData* data) = 0;
#endif  

  virtual ~WebDragDestDelegate() {}
};

}  

#endif  
