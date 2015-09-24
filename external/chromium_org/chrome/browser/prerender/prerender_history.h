// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRERENDER_PRERENDER_HISTORY_H_
#define CHROME_BROWSER_PRERENDER_PRERENDER_HISTORY_H_

#include <list>

#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "chrome/browser/prerender/prerender_final_status.h"
#include "chrome/browser/prerender/prerender_origin.h"
#include "url/gurl.h"

namespace base {
class Value;
}

namespace prerender {

class PrerenderHistory : public base::NonThreadSafe {
 public:
  
  
  struct Entry {
    Entry() : final_status(FINAL_STATUS_MAX), origin(ORIGIN_MAX) {}

    Entry(const GURL& url_arg,
          FinalStatus final_status_arg,
          Origin origin_arg,
          base::Time end_time_arg)
        : url(url_arg), final_status(final_status_arg),
          origin(origin_arg),
          end_time(end_time_arg) {
    }

    
    
    
    GURL url;

    
    
    FinalStatus final_status;

    
    Origin origin;

    
    base::Time end_time;
  };

  
  explicit PrerenderHistory(size_t max_items);
  ~PrerenderHistory();

  
  void AddEntry(const Entry& entry);

  
  void Clear();

  
  base::Value* GetEntriesAsValue() const;

 private:
  std::list<Entry> entries_;
  size_t max_items_;

  DISALLOW_COPY_AND_ASSIGN(PrerenderHistory);
};

}
#endif  
