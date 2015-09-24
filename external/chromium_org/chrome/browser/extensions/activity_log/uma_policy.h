// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_ACTIVITY_LOG_UMA_POLICY_H_
#define CHROME_BROWSER_EXTENSIONS_ACTIVITY_LOG_UMA_POLICY_H_

#include <map>
#include <string>

#include "chrome/browser/extensions/activity_log/activity_log_policy.h"

#include "chrome/browser/ui/browser_list_observer.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "url/gurl.h"

namespace extensions {

class UmaPolicy : public ActivityLogPolicy,
                  public TabStripModelObserver,
                  public chrome::BrowserListObserver {
 public:
  
  
  enum PageStatus {
    NONE = 0,
    CONTENT_SCRIPT = 1,
    READ_DOM,
    MODIFIED_DOM,
    DOM_METHOD,
    DOCUMENT_WRITE,
    INNER_HTML,
    CREATED_SCRIPT,
    CREATED_IFRAME,
    CREATED_DIV,
    CREATED_LINK,
    CREATED_INPUT,
    CREATED_EMBED,
    CREATED_OBJECT,
    MAX_STATUS  
  };

  explicit UmaPolicy(Profile* profile);

  virtual void ProcessAction(scoped_refptr<Action> action) OVERRIDE;
  virtual void Close() OVERRIDE;

  
  static const char* GetHistogramName(PageStatus status);

 protected:
  
  virtual ~UmaPolicy();

 private:
  
  static const char kNumberOfTabs[];

  
  static const size_t kMaxTabsTracked;

  typedef std::map<std::string, int> ExtensionMap;
  typedef std::map<std::string, ExtensionMap> SiteMap;

  
  virtual void OnBrowserAdded(Browser* browser) OVERRIDE;
  virtual void OnBrowserRemoved(Browser* browser) OVERRIDE;

  
  
  
  virtual void TabChangedAt(content::WebContents* contents,
                            int index,
                            TabChangeType change_type) OVERRIDE;
  
  virtual void TabClosingAt(TabStripModel* tab_strip_model,
                            content::WebContents* contents,
                            int index) OVERRIDE;

  
  int MatchActionToStatus(scoped_refptr<Action> action);

  
  void SetupOpenedPage(const std::string& url);

  
  void CleanupClosedPage(const std::string& url);

  
  void HistogramOnClose(const std::string& url);

  
  static std::string CleanURL(const GURL& gurl);

  
  SiteMap url_status() { return url_status_; }

  Profile* profile_;

  
  SiteMap url_status_;

  
  std::map<int32, std::string> tab_list_;

  FRIEND_TEST_ALL_PREFIXES(UmaPolicyTest, CleanURLTest);
  FRIEND_TEST_ALL_PREFIXES(UmaPolicyTest, MatchActionToStatusTest);
  FRIEND_TEST_ALL_PREFIXES(UmaPolicyTest, ProcessActionTest);
  FRIEND_TEST_ALL_PREFIXES(UmaPolicyTest, SiteUrlTest);
};

}  

#endif  
