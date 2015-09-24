// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_SEARCH_TERMS_TRACKER_H_
#define CHROME_BROWSER_SEARCH_SEARCH_TERMS_TRACKER_H_

#include <map>

#include "base/strings/string16.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

namespace content {
class NavigationController;
class WebContents;
}

namespace chrome {

class SearchTermsTracker : public content::NotificationObserver {
 public:
  SearchTermsTracker();
  virtual ~SearchTermsTracker();

  
  
  
  
  bool GetSearchTerms(const content::WebContents* contents,
                      base::string16* search_terms,
                      int* navigation_index) const;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  struct TabData {
    TabData() : srp_navigation_index(-1) {}
    base::string16 search_terms;
    int srp_navigation_index;
  };

  
  typedef std::map<const content::WebContents*, TabData> TabState;

  
  
  bool FindMostRecentSearch(const content::NavigationController* controller,
                            TabData* tab_data);

  
  void RemoveTabData(const content::WebContents* contents);

  content::NotificationRegistrar registrar_;
  TabState tabs_;

  DISALLOW_COPY_AND_ASSIGN(SearchTermsTracker);
};

}  

#endif  
