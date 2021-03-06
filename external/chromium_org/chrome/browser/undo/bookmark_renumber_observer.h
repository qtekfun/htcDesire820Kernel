// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UNDO_BOOKMARK_RENUMBER_OBSERVER_H_
#define CHROME_BROWSER_UNDO_BOOKMARK_RENUMBER_OBSERVER_H_

class BookmarkRenumberObserver {
 public:
  
  
  virtual void OnBookmarkRenumbered(int64 old_id, int64 new_id) = 0;

 protected:
  virtual ~BookmarkRenumberObserver() {}
};

#endif  
