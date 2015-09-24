// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_SHELF_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_SHELF_H_

#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "build/build_config.h"

class Browser;

namespace gfx {
class Canvas;
class ImageSkia;
class Rect;
}

namespace content {
class DownloadItem;
class DownloadManager;
}

#if defined(TOOLKIT_VIEWS)
namespace views {
class View;
}
#endif

class DownloadShelf {
 public:
  
  enum CloseReason {
    
    
    
    AUTOMATIC,

    
    
    
    USER_ACTION
  };

  enum PaintDownloadProgressSize {
    SMALL = 0,
    BIG
  };

  

  enum {
    
    kUnknownAngleDegrees = 50,

    
    kUnknownIncrementDegrees = 12,

    
    kStartAngleDegrees = -90,

    
    kMaxDegrees = 360,

    
    kProgressRateMs = 150,

    
    kSmallIconSize = 16,
    kBigIconSize = 32,

    kSmallProgressIconSize = 39,
    kBigProgressIconSize = 52,

    kSmallProgressIconOffset = (kSmallProgressIconSize - kSmallIconSize) / 2
  };

  DownloadShelf();
  virtual ~DownloadShelf();

  
  
  
  static int GetBigProgressIconSize();

  
  static int GetBigProgressIconOffset();

  
  
  
  
  
  
  
  
  
  static void PaintCustomDownloadProgress(
      gfx::Canvas* canvas,
      const gfx::ImageSkia& background_image,
      const gfx::ImageSkia& foreground_image,
      int image_size,
      const gfx::Rect& bounds,
      int start_angle,
      int percent_done);

  static void PaintDownloadProgress(gfx::Canvas* canvas,
#if defined(TOOLKIT_VIEWS)
                                    views::View* containing_view,
#endif
                                    int origin_x,
                                    int origin_y,
                                    int start_angle,
                                    int percent,
                                    PaintDownloadProgressSize size);

  static void PaintDownloadComplete(gfx::Canvas* canvas,
#if defined(TOOLKIT_VIEWS)
                                    views::View* containing_view,
#endif
                                    int origin_x,
                                    int origin_y,
                                    double animation_progress,
                                    PaintDownloadProgressSize size);

  static void PaintDownloadInterrupted(gfx::Canvas* canvas,
#if defined(TOOLKIT_VIEWS)
                                       views::View* containing_view,
#endif
                                       int origin_x,
                                       int origin_y,
                                       double animation_progress,
                                       PaintDownloadProgressSize size);

  
  
  
  
  
  
  
  void AddDownload(content::DownloadItem* download);

  
  
  
  
  virtual bool IsShowing() const = 0;

  
  virtual bool IsClosing() const = 0;

  
  void Show();

  
  void Close(CloseReason reason);

  
  void Hide();

  
  
  void Unhide();

  virtual Browser* browser() const = 0;

  
  bool is_hidden() { return is_hidden_; }

 protected:
  virtual void DoAddDownload(content::DownloadItem* download) = 0;
  virtual void DoShow() = 0;
  virtual void DoClose(CloseReason reason) = 0;

  
  
  virtual base::TimeDelta GetTransientDownloadShowDelay();

  
  
  
  virtual content::DownloadManager* GetDownloadManager();

 private:
  
  
  void ShowDownload(content::DownloadItem* download);

  
  
  
  void ShowDownloadById(int32 download_id);

  bool should_show_on_unhide_;
  bool is_hidden_;
  base::WeakPtrFactory<DownloadShelf> weak_ptr_factory_;
};

#endif  
