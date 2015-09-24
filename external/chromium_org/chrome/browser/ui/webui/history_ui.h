// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_HISTORY_UI_H_
#define CHROME_BROWSER_UI_WEBUI_HISTORY_UI_H_

#include <string>

#include "base/strings/string16.h"
#include "base/timer/timer.h"
#include "base/values.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/history/history_service.h"
#include "chrome/browser/history/web_history_service.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_ui_controller.h"
#include "content/public/browser/web_ui_message_handler.h"

class BookmarkModel;
class ManagedUserService;
class ProfileSyncService;

class BrowsingHistoryHandler : public content::WebUIMessageHandler,
                               public content::NotificationObserver {
 public:
  
  
  
  struct HistoryEntry {
    
    
    enum EntryType {
      EMPTY_ENTRY = 0,
      LOCAL_ENTRY,
      REMOTE_ENTRY,
      COMBINED_ENTRY
    };

    HistoryEntry(EntryType type, const GURL& url, const base::string16& title,
                 base::Time time, const std::string& client_id,
                 bool is_search_result, const base::string16& snippet,
                 bool blocked_visit, const std::string& accept_languages);
    HistoryEntry();
    virtual ~HistoryEntry();

    
    void SetUrlAndTitle(DictionaryValue* result) const;

    
    scoped_ptr<DictionaryValue> ToValue(
        BookmarkModel* bookmark_model,
        ManagedUserService* managed_user_service,
        const ProfileSyncService* sync_service) const;

    
    static bool SortByTimeDescending(
        const HistoryEntry& entry1, const HistoryEntry& entry2);

    
    EntryType entry_type;

    GURL url;
    base::string16 title;  

    
    
    base::Time time;

    
    std::string client_id;

    
    std::set<int64> all_timestamps;

    
    bool is_search_result;

    
    base::string16 snippet;

    
    bool blocked_visit;

    
    std::string accept_languages;
  };

  BrowsingHistoryHandler();
  virtual ~BrowsingHistoryHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  
  void HandleQueryHistory(const base::ListValue* args);

  
  void HandleRemoveVisits(const base::ListValue* args);

  
  void HandleClearBrowsingData(const base::ListValue* args);

  
  void HandleRemoveBookmark(const base::ListValue* args);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  static void MergeDuplicateResults(
      std::vector<BrowsingHistoryHandler::HistoryEntry>* results);

 private:
  
  
  
  
  enum Range {
    ALL_TIME = 0,
    WEEK = 1,
    MONTH = 2
  };

  
  void QueryHistory(base::string16 search_text,
                    const history::QueryOptions& options);

  
  
  void ReturnResultsToFrontEnd();

  
  
  void WebHistoryTimeout();

  
  void QueryComplete(const base::string16& search_text,
                     const history::QueryOptions& options,
                     HistoryService::Handle request_handle,
                     history::QueryResults* results);

  
  void WebHistoryQueryComplete(const base::string16& search_text,
                               const history::QueryOptions& options,
                               base::TimeTicks start_time,
                               history::WebHistoryService::Request* request,
                               const base::DictionaryValue* results_value);

  
  void RemoveComplete();

  
  void RemoveWebHistoryComplete(history::WebHistoryService::Request* request,
                                bool success);

  bool ExtractIntegerValueAtIndex(
      const base::ListValue* value, int index, int* out_int);

  
  void SetQueryTimeInWeeks(int offset, history::QueryOptions* options);

  
  void SetQueryTimeInMonths(int offset, history::QueryOptions* options);

  
  std::string GetAcceptLanguages() const;

  content::NotificationRegistrar registrar_;

  
  CancelableRequestConsumerT<int, 0> history_request_consumer_;

  
  
  scoped_ptr<history::WebHistoryService::Request> web_history_request_;

  
  
  scoped_ptr<history::WebHistoryService::Request> web_history_delete_request_;

  
  CancelableTaskTracker delete_task_tracker_;

  
  std::set<GURL> urls_to_be_deleted_;

  
  base::DictionaryValue results_info_value_;

  
  std::vector<HistoryEntry> query_results_;

  
  std::vector<HistoryEntry> web_history_query_results_;

  
  base::OneShotTimer<BrowsingHistoryHandler> web_history_timer_;

  DISALLOW_COPY_AND_ASSIGN(BrowsingHistoryHandler);
};

class HistoryUI : public content::WebUIController {
 public:
  explicit HistoryUI(content::WebUI* web_ui);

  
  static const GURL GetHistoryURLWithSearchText(const base::string16& text);

  static base::RefCountedMemory* GetFaviconResourceBytes(
      ui::ScaleFactor scale_factor);

 private:
  DISALLOW_COPY_AND_ASSIGN(HistoryUI);
};

#endif  
