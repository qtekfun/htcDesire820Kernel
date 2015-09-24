// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DRAG_DOWNLOAD_FILE_H_
#define CHROME_BROWSER_DOWNLOAD_DRAG_DOWNLOAD_FILE_H_
#pragma once

#include "base/file_path.h"
#include "base/memory/linked_ptr.h"
#include "chrome/browser/download/download_file.h"
#include "chrome/browser/download/download_item.h"
#include "chrome/browser/download/download_manager.h"
#include "googleurl/src/gurl.h"
#include "ui/base/dragdrop/download_file_interface.h"

class TabContents;

namespace net {
class FileStream;
}

class DragDownloadFile : public ui::DownloadFileProvider,
                         public DownloadManager::Observer,
                         public DownloadItem::Observer {
 public:
  
  
  
  
  
  
  
  
  
  DragDownloadFile(const FilePath& file_name_or_path,
                   linked_ptr<net::FileStream> file_stream,
                   const GURL& url,
                   const GURL& referrer,
                   const std::string& referrer_encoding,
                   TabContents* tab_contents);

  
  
  
  virtual bool Start(ui::DownloadFileObserver* observer);
  virtual void Stop();
#if defined(OS_WIN)
  virtual IStream* GetStream() { return NULL; }
#endif

  
  
  virtual void ModelChanged();

  
  
  virtual void OnDownloadUpdated(DownloadItem* download);
  virtual void OnDownloadOpened(DownloadItem* download) { }

 private:
  
  
  virtual ~DragDownloadFile();

  
#if defined(OS_WIN)
  void StartNestedMessageLoop();
  void QuitNestedMessageLoop();
#endif

  
  
  void InitiateDownload();
  void DownloadCompleted(bool is_successful);

  
  void AssertCurrentlyOnDragThread();
  void AssertCurrentlyOnUIThread();

  
  
  
  FilePath file_path_;
  FilePath file_name_;
  linked_ptr<net::FileStream> file_stream_;
  GURL url_;
  GURL referrer_;
  std::string referrer_encoding_;
  TabContents* tab_contents_;
  MessageLoop* drag_message_loop_;
  FilePath temp_dir_path_;

  
  
  bool is_started_;
  bool is_successful_;
  scoped_refptr<ui::DownloadFileObserver> observer_;

  
#if defined(OS_WIN)
  bool is_running_nested_message_loop_;
#endif

  
  DownloadManager* download_manager_;
  bool download_item_observer_added_;

  DISALLOW_COPY_AND_ASSIGN(DragDownloadFile);
};

#endif  
