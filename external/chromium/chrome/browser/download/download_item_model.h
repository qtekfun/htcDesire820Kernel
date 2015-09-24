// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_ITEM_MODEL_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_ITEM_MODEL_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/string16.h"

class DownloadItem;
class SavePackage;

class BaseDownloadItemModel {
 public:
  explicit BaseDownloadItemModel(DownloadItem* download)
      : download_(download) { }
  virtual ~BaseDownloadItemModel() { }

  
  virtual void CancelTask() = 0;

  
  virtual string16 GetStatusText() = 0;

  DownloadItem* download() { return download_; }

 protected:
  DownloadItem* download_;
};

class DownloadItemModel : public BaseDownloadItemModel {
 public:
  explicit DownloadItemModel(DownloadItem* download);
  virtual ~DownloadItemModel() { }

  
  virtual void CancelTask();

  
  virtual string16 GetStatusText();

 private:
  DISALLOW_COPY_AND_ASSIGN(DownloadItemModel);
};

class SavePageModel : public BaseDownloadItemModel {
 public:
  SavePageModel(SavePackage* save, DownloadItem* download);
  virtual ~SavePageModel() { }

  
  virtual void CancelTask();

  
  virtual string16 GetStatusText();

 private:
  
  SavePackage* save_;

  DISALLOW_COPY_AND_ASSIGN(SavePageModel);
};

#endif  
