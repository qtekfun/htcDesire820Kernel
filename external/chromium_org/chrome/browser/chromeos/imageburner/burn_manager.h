// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_IMAGEBURNER_BURN_MANAGER_H_
#define CHROME_BROWSER_CHROMEOS_IMAGEBURNER_BURN_MANAGER_H_

#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/memory/ref_counted_memory.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "chrome/browser/chromeos/imageburner/burn_device_handler.h"
#include "chromeos/disks/disk_mount_manager.h"
#include "chromeos/network/network_state_handler_observer.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "url/gurl.h"

namespace net {
class URLFetcher;
class URLRequestContextGetter;
}  

namespace chromeos {

enum BurnEvent {
  UNZIP_STARTED,
  UNZIP_COMPLETE,
  UNZIP_FAIL,
  BURN_UPDATE,
  BURN_SUCCESS,
  BURN_FAIL,
  UNKNOWN
};

struct ImageBurnStatus {
  ImageBurnStatus() : amount_burnt(0), total_size(0) {
  }

  ImageBurnStatus(int64 burnt, int64 total)
      : amount_burnt(burnt), total_size(total) {
  }

  int64 amount_burnt;
  int64 total_size;
};

namespace imageburner {

enum ProgressType {
  DOWNLOADING,
  UNZIPPING,
  BURNING
};

extern const char kName[];
extern const char kHwid[];
extern const char kFileName[];
extern const char kUrl[];

class ConfigFile {
 public:
  ConfigFile();

  explicit ConfigFile(const std::string& file_content);

  ~ConfigFile();

  
  void reset(const std::string& file_content);

  void clear();

  bool empty() const { return config_struct_.empty(); }

  size_t size() const { return config_struct_.size(); }

  
  const std::string& GetProperty(const std::string& property_name,
                                 const std::string& hwid) const;

 private:
  void DeleteLastBlockIfHasNoHwid();
  void ProcessLine(const std::vector<std::string>& line);

  typedef std::map<std::string, std::string> PropertyMap;
  typedef std::set<std::string> HwidsSet;

  
  
  
  
  struct ConfigFileBlock {
    ConfigFileBlock();
    ~ConfigFileBlock();

    PropertyMap properties;
    HwidsSet hwids;
  };

  
  
  
  
  
  typedef std::list<ConfigFileBlock> BlockList;
  BlockList config_struct_;
};

class StateMachine {
 public:
  enum State {
    INITIAL,
    DOWNLOADING,
    BURNING,
  };

  State state() { return state_; }

  class Observer {
   public:
    virtual void OnBurnStateChanged(State new_state) = 0;
    virtual void OnError(int error_message_id) = 0;
  };

  StateMachine();
  ~StateMachine();

  bool download_started() const { return download_started_; }
  void OnDownloadStarted() {
    download_started_ = true;
    state_ = DOWNLOADING;
    OnStateChanged();
  }

  bool download_finished() const { return download_finished_; }
  void OnDownloadFinished() { download_finished_ = true; }

  void OnBurnStarted() {
    state_ = BURNING;
    OnStateChanged();
  }

  bool new_burn_posible() const { return state_ == INITIAL; }

  void OnSuccess();
  void OnError(int error_message_id);

  void OnStateChanged() {
    FOR_EACH_OBSERVER(Observer, observers_, OnBurnStateChanged(state_));
  }

  void AddObserver(Observer* observer) {
    observers_.AddObserver(observer);
  }

  void RemoveObserver(Observer* observer) {
    observers_.RemoveObserver(observer);
  }

 private:
  bool download_started_;
  bool download_finished_;

  State state_;

  ObserverList<Observer> observers_;

  DISALLOW_COPY_AND_ASSIGN(StateMachine);
};

class BurnManager : public net::URLFetcherDelegate,
                    public NetworkStateHandlerObserver {
 public:
  
  
  class Observer {
   public:
    
    virtual void OnDeviceAdded(const disks::DiskMountManager::Disk& disk) = 0;

    
    virtual void OnDeviceRemoved(const disks::DiskMountManager::Disk& disk) = 0;

    
    virtual void OnNetworkDetected() = 0;

    
    virtual void OnSuccess() = 0;

    
    
    
    virtual void OnProgressWithRemainingTime(
        ProgressType progress_type,
        int64 received_bytes,
        int64 total_bytes,
        const base::TimeDelta& estimated_remaining_time) = 0;

    
    
    
    virtual void OnProgress(ProgressType progress_type,
                            int64 received_bytes,
                            int64 total_bytes) = 0;
  };

  
  static void Initialize(
      const base::FilePath& downloads_directory,
      scoped_refptr<net::URLRequestContextGetter> context_getter);

  
  static void Shutdown();

  
  
  static BurnManager* GetInstance();

  
  void AddObserver(Observer* observer);

  
  void RemoveObserver(Observer* observer);

  
  std::vector<disks::DiskMountManager::Disk> GetBurnableDevices();

  
  
  
  
  
  
  void Cancel();

  
  
  
  
  
  
  
  
  
  void OnError(int message_id);

  
  
  void FetchConfigFile();

  
  void FetchImage();

  
  
  
  
  
  
  
  
  void DoBurn();

  
  
  void CancelBurnImage();

  
  void CancelImageFetch();

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;
  virtual void OnURLFetchDownloadProgress(const net::URLFetcher* source,
                                          int64 current,
                                          int64 total) OVERRIDE;

  
  virtual void DefaultNetworkChanged(const NetworkState* network) OVERRIDE;

  
  
  void CreateImageDir();

  
  
  
  base::FilePath GetImageDir();

  const base::FilePath& target_device_path() { return target_device_path_; }
  void set_target_device_path(const base::FilePath& path) {
    target_device_path_ = path;
  }

  const base::FilePath& target_file_path() { return target_file_path_; }
  void set_target_file_path(const base::FilePath& path) {
    target_file_path_ = path;
  }

  void ResetTargetPaths() {
    target_device_path_.clear();
    target_file_path_.clear();
  }

  StateMachine* state_machine() const { return state_machine_.get(); }

 private:
  BurnManager(const base::FilePath& downloads_directory,
              scoped_refptr<net::URLRequestContextGetter> context_getter);
  virtual ~BurnManager();

  void UpdateBurnStatus(BurnEvent evt, const ImageBurnStatus& status);

  void OnImageDirCreated(bool success);
  void ConfigFileFetched(bool fetched, const std::string& content);

  void OnImageUnzipped(scoped_refptr<base::RefCountedString> source_image_file);
  void OnDevicesUnmounted(bool success);
  void OnBurnImageFail();
  void OnBurnFinished(const std::string& target_path,
                      bool success,
                      const std::string& error);
  void OnBurnProgressUpdate(const std::string& target_path,
                            int64 num_bytes_burnt,
                            int64 total_size);

  void NotifyDeviceAdded(const disks::DiskMountManager::Disk& disk);
  void NotifyDeviceRemoved(const disks::DiskMountManager::Disk& disk);

  BurnDeviceHandler device_handler_;

  bool image_dir_created_;
  bool unzipping_;
  bool cancelled_;
  bool burning_;
  bool block_burn_signals_;

  base::FilePath image_dir_;
  base::FilePath zip_image_file_path_;
  base::FilePath source_image_path_;
  base::FilePath target_device_path_;
  base::FilePath target_file_path_;

  GURL config_file_url_;
  bool config_file_fetched_;
  std::string image_file_name_;
  GURL image_download_url_;

  scoped_ptr<StateMachine> state_machine_;

  scoped_ptr<net::URLFetcher> config_fetcher_;
  scoped_ptr<net::URLFetcher> image_fetcher_;

  scoped_refptr<net::URLRequestContextGetter> url_request_context_getter_;

  base::TimeTicks tick_image_download_start_;
  int64 bytes_image_download_progress_last_reported_;

  ObserverList<Observer> observers_;

  
  
  base::WeakPtrFactory<BurnManager> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(BurnManager);
};

}  

}  

#endif  
