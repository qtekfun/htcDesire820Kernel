// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_BLOCKING_PAGE_H_
#define CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_BLOCKING_PAGE_H_

#include <map>
#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/time/time.h"
#include "chrome/browser/history/history_service.h"
#include "chrome/browser/safe_browsing/ui_manager.h"
#include "content/public/browser/interstitial_page_delegate.h"
#include "url/gurl.h"

class MalwareDetails;
class SafeBrowsingBlockingPageFactory;

namespace base {
class DictionaryValue;
class MessageLoop;
}

namespace content {
class InterstitialPage;
class WebContents;
}

class SafeBrowsingBlockingPage : public content::InterstitialPageDelegate {
 public:
  typedef SafeBrowsingUIManager::UnsafeResource UnsafeResource;
  typedef std::vector<UnsafeResource> UnsafeResourceList;
  typedef std::map<content::WebContents*, UnsafeResourceList> UnsafeResourceMap;

  virtual ~SafeBrowsingBlockingPage();

  
  
  
  
  
  static void ShowBlockingPage(
      SafeBrowsingUIManager* ui_manager, const UnsafeResource& resource);

  
  
  static void RegisterFactory(SafeBrowsingBlockingPageFactory* factory) {
    factory_ = factory;
  }

  
  virtual void CommandReceived(const std::string& command) OVERRIDE;
  virtual void OverrideRendererPrefs(
      content::RendererPreferences* prefs) OVERRIDE;
  virtual void OnProceed() OVERRIDE;
  virtual void OnDontProceed() OVERRIDE;

 protected:
  friend class SafeBrowsingBlockingPageTest;
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingBlockingPageTest,
                           ProceedThenDontProceed);
  friend class SafeBrowsingBlockingPageV2Test;
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingBlockingPageV2Test,
                           ProceedThenDontProceed);

  void SetReportingPreference(bool report);

  
  SafeBrowsingBlockingPage(SafeBrowsingUIManager* ui_manager,
                           content::WebContents* web_contents,
                           const UnsafeResourceList& unsafe_resources);

  
  
  
  
  int64 malware_details_proceed_delay_ms_;
  content::InterstitialPage* interstitial_page() const {
    return interstitial_page_;
  }

  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingBlockingPageTest, MalwareReports);
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingBlockingPageV2Test, MalwareReports);

  enum BlockingPageEvent {
    SHOW,
    PROCEED,
    DONT_PROCEED,
    SHOW_ADVANCED,
  };

  
  
  void RecordUserAction(BlockingPageEvent event);

  
  void OnGotHistoryCount(HistoryService::Handle handle,
                         bool success,
                         int num_visits,
                         base::Time first_visit);

  
  
  
  void RecordUserReactionTime(const std::string& command);

  
  
  bool CanShowMalwareDetailsOption();

  
  
  
  
  void FinishMalwareDetails(int64 delay_ms);

  
  
  bool IsPrefEnabled(const char* pref);

  
  
  
  static UnsafeResourceMap* GetUnsafeResourcesMap();

  
  
  static void NotifySafeBrowsingUIManager(
      SafeBrowsingUIManager* ui_manager,
      const UnsafeResourceList& resources, bool proceed);

  
  
  static bool IsMainPageLoadBlocked(
      const UnsafeResourceList& unsafe_resources);

  friend class SafeBrowsingBlockingPageFactoryImpl;

  
  SafeBrowsingUIManager* ui_manager_;
  base::MessageLoop* report_loop_;

  
  
  
  
  bool is_main_frame_load_blocked_;

  
  
  int navigation_entry_index_to_remove_;

  
  UnsafeResourceList unsafe_resources_;

  
  
  
  scoped_refptr<MalwareDetails> malware_details_;

  bool proceeded_;

  content::WebContents* web_contents_;
  GURL url_;
  content::InterstitialPage* interstitial_page_;  

  
  
  
  
  base::TimeTicks interstitial_show_time_;

  
  
  bool has_expanded_see_more_section_;

  
  enum {
    TYPE_MALWARE,
    TYPE_PHISHING,
    TYPE_MALWARE_AND_PHISHING,
  } interstitial_type_;

  
  
  
  static SafeBrowsingBlockingPageFactory* factory_;

  
  int num_visits_;
  CancelableRequestConsumer request_consumer_;

  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingBlockingPage);
};

class SafeBrowsingBlockingPageV1 : public SafeBrowsingBlockingPage {
 public:
  
  SafeBrowsingBlockingPageV1(SafeBrowsingUIManager* ui_manager,
                             content::WebContents* web_contents,
                             const UnsafeResourceList& unsafe_resources);

  
  virtual std::string GetHTMLContents() OVERRIDE;

 private:
  
  
  void PopulateMultipleThreatStringDictionary(base::DictionaryValue* strings);
  void PopulateMalwareStringDictionary(base::DictionaryValue* strings);
  void PopulatePhishingStringDictionary(base::DictionaryValue* strings);

  
  
  void PopulateStringDictionary(base::DictionaryValue* strings,
                                const base::string16& title,
                                const base::string16& headline,
                                const base::string16& description1,
                                const base::string16& description2,
                                const base::string16& description3);

  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingBlockingPageV1);
};

class SafeBrowsingBlockingPageV2 : public SafeBrowsingBlockingPage {
 public:
  
  SafeBrowsingBlockingPageV2(SafeBrowsingUIManager* ui_manager,
                             content::WebContents* web_contents,
                             const UnsafeResourceList& unsafe_resources);

  
  virtual std::string GetHTMLContents() OVERRIDE;

 private:
  
  
  void PopulateMultipleThreatStringDictionary(base::DictionaryValue* strings);
  void PopulateMalwareStringDictionary(base::DictionaryValue* strings);
  void PopulatePhishingStringDictionary(base::DictionaryValue* strings);

  
  
  void PopulateStringDictionary(base::DictionaryValue* strings,
                                const base::string16& title,
                                const base::string16& headline,
                                const base::string16& description1,
                                const base::string16& description2,
                                const base::string16& description3);

  
  std::string trialCondition_;

  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingBlockingPageV2);
};

class SafeBrowsingBlockingPageFactory {
 public:
  virtual ~SafeBrowsingBlockingPageFactory() { }

  virtual SafeBrowsingBlockingPage* CreateSafeBrowsingPage(
      SafeBrowsingUIManager* ui_manager,
      content::WebContents* web_contents,
      const SafeBrowsingBlockingPage::UnsafeResourceList& unsafe_resources) = 0;
};

#endif  
