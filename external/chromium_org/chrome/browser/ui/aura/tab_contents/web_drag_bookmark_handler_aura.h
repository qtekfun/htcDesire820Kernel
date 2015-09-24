// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AURA_TAB_CONTENTS_WEB_DRAG_BOOKMARK_HANDLER_AURA_H_
#define CHROME_BROWSER_UI_AURA_TAB_CONTENTS_WEB_DRAG_BOOKMARK_HANDLER_AURA_H_

#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_drag_dest_delegate.h"
#include "chrome/browser/bookmarks/bookmark_node_data.h"

class BookmarkTabHelper;

namespace content {
class WebContents;
}

class WebDragBookmarkHandlerAura : public content::WebDragDestDelegate {
 public:
  WebDragBookmarkHandlerAura();
  virtual ~WebDragBookmarkHandlerAura();

  
  virtual void DragInitialize(content::WebContents* contents) OVERRIDE;
  virtual void OnDragOver() OVERRIDE;
  virtual void OnDragEnter() OVERRIDE;
  virtual void OnDrop() OVERRIDE;
  virtual void OnDragLeave() OVERRIDE;

  virtual void OnReceiveDragData(const ui::OSExchangeData& data) OVERRIDE;

 private:
  
  
  
  BookmarkTabHelper* bookmark_tab_helper_;

  content::WebContents* web_contents_;

  
  BookmarkNodeData bookmark_drag_data_;

  DISALLOW_COPY_AND_ASSIGN(WebDragBookmarkHandlerAura);
};

#endif  
