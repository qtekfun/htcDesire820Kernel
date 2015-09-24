// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_WEBUI_CHROMEOS_IMAGEBURNER_UI_H_
#define CHROME_BROWSER_UI_WEBUI_CHROMEOS_IMAGEBURNER_UI_H_

#include <map>
#include <string>
#include <vector>

#include "base/file_path.h"
#include "base/file_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "base/values.h"
#include "chrome/browser/chromeos/cros/burn_library.h"
#include "chrome/browser/chromeos/cros/cros_library.h"
#include "chrome/browser/chromeos/cros/mount_library.h"
#include "chrome/browser/download/download_item.h"
#include "chrome/browser/download/download_manager.h"
#include "chrome/browser/download/download_util.h"
#include "chrome/browser/ui/webui/chrome_url_data_manager.h"
#include "content/browser/webui/web_ui.h"
#include "googleurl/src/gurl.h"
#include "net/base/file_stream.h"
#include "ui/base/dragdrop/download_file_interface.h"

template <typename T> struct DefaultSingletonTraits;

class TabContents;
class ImageBurnTaskProxy;
class ImageBurnDownloaderTaskProxy;

class ImageBurnDownloader {
 public:

  class Listener {
   public:
    
    
    virtual void OnDownloadStarted(bool success) = 0;
  };

 private:
  typedef std::multimap<GURL, Listener*> ListenerMap;

 public:
  
  static ImageBurnDownloader* GetInstance();

  
  
  void DownloadFile(const GURL& url, const FilePath& target_file,
      TabContents* tab_contents);

  
  
  void CreateFileStreamOnFileThread(const GURL& url, const FilePath& file_path,
      TabContents* tab_contents, ImageBurnDownloaderTaskProxy* task);

  
  void OnFileStreamCreatedOnUIThread(const GURL& url,
      const FilePath& file_path, TabContents* tab_contents,
      net::FileStream* created_file_stream);

  
  
  void AddListener(Listener* listener, const GURL& url);

 private:
  ImageBurnDownloader() {}
  ~ImageBurnDownloader() {}

  
  void DownloadStarted(bool success, const GURL& url);

  friend struct DefaultSingletonTraits<ImageBurnDownloader>;
  std::multimap<GURL, Listener*> listeners_;

  DISALLOW_COPY_AND_ASSIGN(ImageBurnDownloader);
};

class ImageBurnResourceManager
    : public DownloadManager::Observer,
      public DownloadItem::Observer,
      public ImageBurnDownloader::Listener {
 public:

  class Delegate {
   public:
    virtual void OnImageDirCreated(bool success, ImageBurnTaskProxy* task) = 0;
    virtual void OnImageUrlCreated(GURL* image_url, bool success) = 0;
  };

  
  static ImageBurnResourceManager* GetInstance();

  
  virtual void OnDownloadUpdated(DownloadItem* download) OVERRIDE;
  virtual void OnDownloadOpened(DownloadItem* download) OVERRIDE {}

  
  virtual void ModelChanged() OVERRIDE;

  
  virtual void OnDownloadStarted(bool success) OVERRIDE;

  
  
  void CreateImageUrl(TabContents* tab_content,
                      Delegate* delegate);

  
  
  void CreateImageDir(Delegate* delegate,
                      ImageBurnTaskProxy* task);

  
  
  const FilePath& GetImageDir();

  void ConfigFileFetched(bool fetched);

  bool image_download_requested() const { return image_download_requested_; }
  void set_image_download_requested(bool r) { image_download_requested_ = r; }

  bool download_started() const { return download_started_; }
  void set_download_started(bool s) { download_started_ = s; }

  bool download_finished() const { return download_finished_; }
  void SetDownloadFinished(bool finished);

  bool burn_in_progress() const { return burn_in_progress_; }
  void set_burn_in_progress(bool b) { burn_in_progress_ = b; }

 private:
  friend struct DefaultSingletonTraits<ImageBurnResourceManager>;

  ImageBurnResourceManager();
  ~ImageBurnResourceManager();

  FilePath image_dir_;
  FilePath config_file_path_;

  bool image_download_requested_;
  bool download_started_;
  bool download_finished_;
  bool burn_in_progress_;

  DownloadManager* download_manager_;
  bool download_item_observer_added_;
  DownloadItem*  active_download_item_;

  scoped_ptr<GURL> image_url_;
  GURL config_file_url_;
  bool config_file_requested_;
  bool config_file_fetched_;
  std::vector<Delegate*> downloaders_;

  DISALLOW_COPY_AND_ASSIGN(ImageBurnResourceManager);
};

class ImageBurnHandler : public WebUIMessageHandler,
                         public chromeos::MountLibrary::Observer,
                         public chromeos::BurnLibrary::Observer,
                         public DownloadManager::Observer,
                         public DownloadItem::Observer,
                         public ImageBurnResourceManager::Delegate,
                         public ImageBurnDownloader::Listener,
                         public base::SupportsWeakPtr<ImageBurnHandler> {
 public:
  explicit ImageBurnHandler(TabContents* contents);
  virtual ~ImageBurnHandler();

  
  virtual WebUIMessageHandler* Attach(WebUI* web_ui) OVERRIDE;
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void DiskChanged(chromeos::MountLibraryEventType event,
                           const chromeos::MountLibrary::Disk* disk) OVERRIDE;
  virtual void DeviceChanged(chromeos::MountLibraryEventType event,
                             const std::string& device_path) OVERRIDE;

  
  virtual void ProgressUpdated(chromeos::BurnLibrary* object,
                               chromeos::BurnEventType evt,
                               const ImageBurnStatus& status) OVERRIDE;

  
  virtual void OnDownloadUpdated(DownloadItem* download) OVERRIDE;
  virtual void OnDownloadOpened(DownloadItem* download) OVERRIDE;

  
  virtual void ModelChanged() OVERRIDE;

  
  virtual void OnImageDirCreated(bool success, ImageBurnTaskProxy* task)
      OVERRIDE;
  virtual void OnImageUrlCreated(GURL* image_url, bool success) OVERRIDE;

  
  virtual void OnDownloadStarted(bool success) OVERRIDE;

  
  void CreateImageDirOnFileThread(ImageBurnTaskProxy* task);
  void OnImageDirCreatedOnUIThread(bool success);
  void BurnImageOnFileThread();
  void UnzipImageOnFileThread(ImageBurnTaskProxy* task);
  void UnzipComplete(bool success);

 private:
  
  void HandleGetRoots(const ListValue* args);

  
  void HandleDownloadImage(const ListValue* args);

  
  void HandleBurnImage(const ListValue* args);

  
  void HandleCancelBurnImage(const ListValue* args);

  void DownloadCompleted(bool success);

  void FinalizeBurn(bool successful);

  void UpdateBurnProgress(int64 total_burnt, int64 image_size,
                          const std::string& path, chromeos::BurnEventType evt);
  string16 GetBurnProgressText(int64 total_burnt, int64 image_size);

  void UnzipImage();
  bool UnzipImageImpl();

  
  void ExtractTargetedDeviceSystemPath(const ListValue* list_value);

 private:
  FilePath zip_image_file_path_;
  FilePath image_file_path_;
  FilePath image_target_;
  GURL* image_download_url_;
  TabContents* tab_contents_;
  DownloadManager* download_manager_;
  bool download_item_observer_added_;
  DownloadItem*  active_download_item_;
  ImageBurnResourceManager* resource_manager_;

  DISALLOW_COPY_AND_ASSIGN(ImageBurnHandler);
};

class ImageBurnUI : public WebUI {
 public:
  explicit ImageBurnUI(TabContents* contents);

 private:
  DISALLOW_COPY_AND_ASSIGN(ImageBurnUI);
};

#endif  
