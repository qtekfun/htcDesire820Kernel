// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_HISTORY_UI_H_
#define CHROME_BROWSER_UI_WEBUI_HISTORY_UI_H_
#pragma once

#include <string>

#include "base/string16.h"
#include "chrome/browser/history/history.h"
#include "chrome/browser/ui/webui/chrome_url_data_manager.h"
#include "content/browser/cancelable_request.h"
#include "content/browser/webui/web_ui.h"

class GURL;

class HistoryUIHTMLSource : public ChromeURLDataManager::DataSource {
 public:
  HistoryUIHTMLSource();

  
  
  virtual void StartDataRequest(const std::string& path,
                                bool is_incognito,
                                int request_id);
  virtual std::string GetMimeType(const std::string&) const;

 private:
  ~HistoryUIHTMLSource() {}

  DISALLOW_COPY_AND_ASSIGN(HistoryUIHTMLSource);
};

class BrowsingHistoryHandler : public WebUIMessageHandler {
 public:
  BrowsingHistoryHandler();
  virtual ~BrowsingHistoryHandler();

  
  virtual WebUIMessageHandler* Attach(WebUI* web_ui);
  virtual void RegisterMessages();

  
  void HandleGetHistory(const ListValue* args);

  
  void HandleSearchHistory(const ListValue* args);

  
  void HandleRemoveURLsOnOneDay(const ListValue* args);

  
  void HandleClearBrowsingData(const ListValue* args);

 private:
  
  void QueryComplete(HistoryService::Handle request_handle,
                     history::QueryResults* results);

  
  void RemoveComplete();

  
  void ExtractSearchHistoryArguments(const ListValue* args,
                                     int* month,
                                     string16* query);

  
  history::QueryOptions CreateMonthQueryOptions(int month);

  
  string16 search_text_;

  
  CancelableRequestConsumerT<int, 0> cancelable_search_consumer_;

  
  CancelableRequestConsumerT<int, 0> cancelable_delete_consumer_;

  DISALLOW_COPY_AND_ASSIGN(BrowsingHistoryHandler);
};

class HistoryUI : public WebUI {
 public:
  explicit HistoryUI(TabContents* contents);

  
  static const GURL GetHistoryURLWithSearchText(const string16& text);

  static RefCountedMemory* GetFaviconResourceBytes();

 private:
  DISALLOW_COPY_AND_ASSIGN(HistoryUI);
};

#endif  
