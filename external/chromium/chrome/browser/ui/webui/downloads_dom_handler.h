// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_DOWNLOADS_DOM_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_DOWNLOADS_DOM_HANDLER_H_
#pragma once

#include <vector>

#include "base/memory/scoped_callback_factory.h"
#include "chrome/browser/download/download_item.h"
#include "chrome/browser/download/download_manager.h"
#include "content/browser/webui/web_ui.h"

class ListValue;

class DownloadsDOMHandler : public WebUIMessageHandler,
                            public DownloadManager::Observer,
                            public DownloadItem::Observer {
 public:
  explicit DownloadsDOMHandler(DownloadManager* dlm);
  virtual ~DownloadsDOMHandler();

  void Init();

  
  virtual void RegisterMessages();

  
  virtual void OnDownloadUpdated(DownloadItem* download);
  virtual void OnDownloadOpened(DownloadItem* download) { }

  
  virtual void ModelChanged();

  
  void HandleGetDownloads(const ListValue* args);

  
  void HandleOpenFile(const ListValue* args);

  
  void HandleDrag(const ListValue* args);

  
  
  void HandleSaveDangerous(const ListValue* args);

  
  
  void HandleDiscardDangerous(const ListValue* args);

  
  void HandleShow(const ListValue* args);

  
  void HandlePause(const ListValue* args);

  
  
  void HandleRemove(const ListValue* args);

  
  void HandleCancel(const ListValue* args);

  
  void HandleClearAll(const ListValue* args);

 private:
  
  void SendCurrentDownloads();

  
  void ClearDownloadItems();

  
  DownloadItem* GetDownloadById(int id);

  
  DownloadItem* GetDownloadByValue(const ListValue* args);

  
  std::wstring search_text_;

  
  DownloadManager* download_manager_;

  
  
  
  typedef std::vector<DownloadItem*> OrderedDownloads;
  OrderedDownloads download_items_;

  base::ScopedCallbackFactory<DownloadsDOMHandler> callback_factory_;

  DISALLOW_COPY_AND_ASSIGN(DownloadsDOMHandler);
};

#endif  
