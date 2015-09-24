// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_BLOCKING_PAGE_H_
#define CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_BLOCKING_PAGE_H_
#pragma once

#include <map>
#include <vector>

#include "chrome/browser/safe_browsing/safe_browsing_service.h"
#include "content/browser/tab_contents/interstitial_page.h"
#include "googleurl/src/gurl.h"

class DictionaryValue;
class MessageLoop;
class SafeBrowsingBlockingPageFactory;
class MalwareDetails;
class TabContents;

class SafeBrowsingBlockingPage : public InterstitialPage {
 public:
  typedef std::vector<SafeBrowsingService::UnsafeResource> UnsafeResourceList;
  typedef std::map<TabContents*, UnsafeResourceList> UnsafeResourceMap;

  virtual ~SafeBrowsingBlockingPage();

  
  
  
  
  
  static void ShowBlockingPage(
      SafeBrowsingService* service,
      const SafeBrowsingService::UnsafeResource& resource);

  
  
  static void RegisterFactory(SafeBrowsingBlockingPageFactory* factory) {
    factory_ = factory;
  }

  
  virtual std::string GetHTMLContents();
  virtual void SetReportingPreference(bool report);
  virtual void Proceed();
  virtual void DontProceed();

 protected:
  friend class SafeBrowsingBlockingPageTest;

  
  virtual void CommandReceived(const std::string& command);

  
  SafeBrowsingBlockingPage(SafeBrowsingService* service,
                           TabContents* tab_contents,
                           const UnsafeResourceList& unsafe_resources);

 private:
  enum BlockingPageEvent {
    SHOW,
    PROCEED,
    DONT_PROCEED,
  };

  
  
  void PopulateMultipleThreatStringDictionary(DictionaryValue* strings);
  void PopulateMalwareStringDictionary(DictionaryValue* strings);
  void PopulatePhishingStringDictionary(DictionaryValue* strings);

  
  
  void PopulateStringDictionary(DictionaryValue* strings,
                                const string16& title,
                                const string16& headline,
                                const string16& description1,
                                const string16& description2,
                                const string16& description3);

  
  
  void RecordUserAction(BlockingPageEvent event);

  
  
  bool CanShowMalwareDetailsOption();

  
  
  
  
  void FinishMalwareDetails();

  
  
  
  static UnsafeResourceMap* GetUnsafeResourcesMap();

  
  
  static void NotifySafeBrowsingService(SafeBrowsingService* sb_service,
                                        const UnsafeResourceList& resources,
                                        bool proceed);

  
  static bool IsMainPage(const UnsafeResourceList& unsafe_resources);

 private:
  friend class SafeBrowsingBlockingPageFactoryImpl;

  
  SafeBrowsingService* sb_service_;
  MessageLoop* report_loop_;

  
  bool is_main_frame_;

  
  
  int navigation_entry_index_to_remove_;

  
  UnsafeResourceList unsafe_resources_;

  
  
  
  scoped_refptr<MalwareDetails> malware_details_;

  
  
  
  static SafeBrowsingBlockingPageFactory* factory_;

  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingBlockingPage);
};

class SafeBrowsingBlockingPageFactory {
 public:
  virtual ~SafeBrowsingBlockingPageFactory() { }

  virtual SafeBrowsingBlockingPage* CreateSafeBrowsingPage(
      SafeBrowsingService* service,
      TabContents* tab_contents,
      const SafeBrowsingBlockingPage::UnsafeResourceList& unsafe_resources) = 0;
};

#endif  
