// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FAVICON_FAVICON_HANDLER_DELEGATE_H_
#define CHROME_BROWSER_FAVICON_FAVICON_HANDLER_DELEGATE_H_

class GURL;

namespace content {
class NavigationEntry;
}

class FaviconHandlerDelegate {
 public:
  
  virtual content::NavigationEntry* GetActiveEntry() = 0;

  
  
  
  
  
  
  
  
  virtual int StartDownload(const GURL& url, int max_bitmap_size) = 0;

  
  
  
  virtual void NotifyFaviconUpdated(bool icon_url_changed) = 0;
};

#endif  
