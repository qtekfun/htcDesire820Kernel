// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_UTIL_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_UTIL_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/string16.h"
#include "ui/gfx/native_widget_types.h"

#if defined(TOOLKIT_VIEWS)
#include "views/view.h"
#endif

namespace gfx {
class Canvas;
class Image;
}

class BaseDownloadItemModel;
class DictionaryValue;
class DownloadItem;
class DownloadManager;
class GURL;
class Profile;
class ResourceDispatcherHost;
class SkBitmap;

struct DownloadCreateInfo;
struct DownloadSaveInfo;

namespace net {
class URLRequestContextGetter;
}

namespace download_util {


const FilePath& GetDefaultDownloadDirectory();

bool CreateTemporaryFileForDownload(FilePath* path);

bool DownloadPathIsDangerous(const FilePath& download_path);

void GenerateExtension(const FilePath& file_name,
                       const std::string& mime_type,
                       FilePath::StringType* generated_extension);

void GenerateFileNameFromInfo(DownloadCreateInfo* info,
                              FilePath* generated_name);

void GenerateFileName(const GURL& url,
                      const std::string& content_disposition,
                      const std::string& referrer_charset,
                      const std::string& mime_type,
                      FilePath* generated_name);

void GenerateSafeFileName(const std::string& mime_type, FilePath* file_name);

void OpenChromeExtension(Profile* profile,
                         DownloadManager* download_manager,
                         const DownloadItem& download_item);


const int kUnknownAngleDegrees = 50;

const int kUnknownIncrementDegrees = 12;

const int kStartAngleDegrees = -90;

const int kMaxDegrees = 360;

const int kProgressRateMs = 150;

const int kSmallIconSize = 16;
const int kBigIconSize = 32;

int GetBigProgressIconSize();

const int kSmallProgressIconSize = 39;
const int kBigProgressIconSize = 52;

int GetBigProgressIconOffset();

const int kSmallProgressIconOffset =
    (kSmallProgressIconSize - kSmallIconSize) / 2;

enum PaintDownloadProgressSize {
  SMALL = 0,
  BIG
};

enum DownloadCountTypes {
  
  
  INITIATED_BY_NAVIGATION_COUNT = 0,

  
  INITIATED_BY_CONTEXT_MENU_COUNT,

  
  
  
  INITIATED_BY_SAVE_PACKAGE_FAILURE_COUNT,

  
  
  INITIATED_BY_DRAG_N_DROP_COUNT,

  
  
  INITIATED_BY_RENDERER_COUNT,

  
  
  UNTHROTTLED_COUNT,

  
  COMPLETED_COUNT,

  
  CANCELLED_COUNT,

  DOWNLOAD_COUNT_TYPES_LAST_ENTRY
};

void RecordDownloadCount(DownloadCountTypes type);

void PaintDownloadProgress(gfx::Canvas* canvas,
#if defined(TOOLKIT_VIEWS)
                           views::View* containing_view,
#endif
                           int origin_x,
                           int origin_y,
                           int start_angle,
                           int percent,
                           PaintDownloadProgressSize size);

void PaintDownloadComplete(gfx::Canvas* canvas,
#if defined(TOOLKIT_VIEWS)
                           views::View* containing_view,
#endif
                           int origin_x,
                           int origin_y,
                           double animation_progress,
                           PaintDownloadProgressSize size);

void PaintDownloadInterrupted(gfx::Canvas* canvas,
#if defined(TOOLKIT_VIEWS)
                              views::View* containing_view,
#endif
                              int origin_x,
                              int origin_y,
                              double animation_progress,
                              PaintDownloadProgressSize size);


void DragDownload(const DownloadItem* download,
                  gfx::Image* icon,
                  gfx::NativeView view);


DictionaryValue* CreateDownloadItemValue(DownloadItem* download, int id);

string16 GetProgressStatusText(DownloadItem* download);

void UpdateAppIconDownloadProgress(int download_count,
                                   bool progress_known,
                                   float progress);

void AppendNumberToPath(FilePath* path, int number);

int GetUniquePathNumber(const FilePath& path);

void DownloadUrl(const GURL& url,
                 const GURL& referrer,
                 const std::string& referrer_charset,
                 const DownloadSaveInfo& save_info,
                 ResourceDispatcherHost* rdh,
                 int render_process_host_id,
                 int render_view_id,
                 net::URLRequestContextGetter* request_context_getter);

void CancelDownloadRequest(ResourceDispatcherHost* rdh,
                           int render_process_id,
                           int request_id);

void NotifyDownloadInitiated(int render_process_id, int render_view_id);

int GetUniquePathNumberWithCrDownload(const FilePath& path);

void EraseUniqueDownloadFiles(const FilePath& path_prefix);

FilePath GetCrDownloadPath(const FilePath& suggested_path);

bool IsDangerous(DownloadCreateInfo* info, Profile* profile, bool auto_open);

}  

#endif  
