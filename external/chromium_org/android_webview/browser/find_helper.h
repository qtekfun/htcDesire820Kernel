// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_BROWSER_FIND_HELPER_H_
#define ANDROID_WEBVIEW_BROWSER_FIND_HELPER_H_

#include "base/memory/weak_ptr.h"
#include "content/public/browser/web_contents_observer.h"

namespace android_webview {

class FindHelper : public content::WebContentsObserver {
 public:
  class Listener {
   public:
    
    
    
    virtual void OnFindResultReceived(int active_ordinal,
                                      int match_count,
                                      bool finished) = 0;
    virtual ~Listener() {}
  };

  explicit FindHelper(content::WebContents* web_contents);
  virtual ~FindHelper();

  
  
  void SetListener(Listener* listener);

  
  void FindAllAsync(const string16& search_string);
  void HandleFindReply(int request_id,
                       int match_count,
                       int active_ordinal,
                       bool finished);

  
  void FindNext(bool forward);
  void ClearMatches();

 private:
  void StartNewRequest(const string16& search_string);
  void NotifyResults(int active_ordinal, int match_count, bool finished);

  
  Listener* listener_;

  
  bool async_find_started_;
  bool sync_find_started_;

  
  
  int find_request_id_counter_;
  int current_request_id_;

  
  string16 last_search_string_;
  int last_match_count_;
  int last_active_ordinal_;

  
  base::WeakPtrFactory<FindHelper> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(FindHelper);
};

}  

#endif  
