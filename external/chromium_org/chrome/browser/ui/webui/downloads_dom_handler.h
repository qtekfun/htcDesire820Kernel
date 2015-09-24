// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_DOWNLOADS_DOM_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_DOWNLOADS_DOM_HANDLER_H_

#include <set>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/download/all_download_item_notifier.h"
#include "chrome/browser/download/download_danger_prompt.h"
#include "content/public/browser/download_item.h"
#include "content/public/browser/download_manager.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace base {
class ListValue;
}

namespace content {
class WebContents;
}

class DownloadsDOMHandler : public content::WebUIMessageHandler,
                            public AllDownloadItemNotifier::Observer {
 public:
  explicit DownloadsDOMHandler(content::DownloadManager* dlm);
  virtual ~DownloadsDOMHandler();

  void Init();

  
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void OnDownloadCreated(
      content::DownloadManager* manager,
      content::DownloadItem* download_item) OVERRIDE;
  virtual void OnDownloadUpdated(
      content::DownloadManager* manager,
      content::DownloadItem* download_item) OVERRIDE;
  virtual void OnDownloadRemoved(
      content::DownloadManager* manager,
      content::DownloadItem* download_item) OVERRIDE;

  
  void OnPageLoaded(const base::ListValue* args);

  
  void HandleGetDownloads(const base::ListValue* args);

  
  void HandleOpenFile(const base::ListValue* args);

  
  void HandleDrag(const base::ListValue* args);

  
  
  void HandleSaveDangerous(const base::ListValue* args);

  
  
  void HandleDiscardDangerous(const base::ListValue* args);

  
  void HandleShow(const base::ListValue* args);

  
  void HandlePause(const base::ListValue* args);

  
  void HandleResume(const base::ListValue* args);

  
  
  void HandleRemove(const base::ListValue* args);

  
  void HandleCancel(const base::ListValue* args);

  
  void HandleClearAll(const base::ListValue* args);

  
  
  void HandleOpenDownloadsFolder(const base::ListValue* args);

 protected:
  
  
  
  virtual content::WebContents* GetWebUIWebContents();
  virtual void CallDownloadsList(const base::ListValue& downloads);
  virtual void CallDownloadUpdated(const base::ListValue& download);

  
  
  void ScheduleSendCurrentDownloads();

 private:
  
  
  typedef std::set<content::DownloadItem*> DownloadSet;

  
  void SendCurrentDownloads();

  
  
  
  
  
  
  void ShowDangerPrompt(content::DownloadItem* dangerous);

  
  
  void DangerPromptDone(int download_id, DownloadDangerPrompt::Action action);

  
  
  bool IsDeletingHistoryAllowed();

  
  content::DownloadItem* GetDownloadByValue(const base::ListValue* args);

  
  scoped_ptr<base::ListValue> search_terms_;

  
  AllDownloadItemNotifier main_notifier_;

  
  
  scoped_ptr<AllDownloadItemNotifier> original_notifier_;

  
  bool update_scheduled_;

  base::WeakPtrFactory<DownloadsDOMHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(DownloadsDOMHandler);
};

#endif  
