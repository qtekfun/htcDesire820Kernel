// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef CHROME_BROWSER_CHROMEOS_FIRST_RUN_DRIVE_FIRST_RUN_CONTROLLER_H_
#define CHROME_BROWSER_CHROMEOS_FIRST_RUN_DRIVE_FIRST_RUN_CONTROLLER_H_

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "base/timer/timer.h"
#include "chrome/browser/profiles/profile.h"

namespace chromeos {

class DriveWebContentsManager;

class DriveFirstRunController {
 public:
  class Observer {
   public:
    
    
    virtual void OnTimedOut() = 0;

    
    
    virtual void OnCompletion(bool success) = 0;

   protected:
    virtual ~Observer() {}
  };

  explicit DriveFirstRunController(Profile* profile);
  ~DriveFirstRunController();

  
  void EnableOfflineMode();

  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  void SetDelaysForTest(int initial_delay_secs, int timeout_secs);

  
  void SetAppInfoForTest(const std::string& app_id,
                         const std::string& endpoint_url);

 private:
  
  
  void OnOfflineInit(bool success);

  
  void OnWebContentsTimedOut();

  
  void ShowNotification();

  
  void CleanUp();

  Profile* profile_;
  scoped_ptr<DriveWebContentsManager> web_contents_manager_;
  base::OneShotTimer<DriveFirstRunController> web_contents_timer_;
  base::OneShotTimer<DriveFirstRunController> initial_delay_timer_;
  bool started_;
  ObserverList<Observer> observer_list_;

  int initial_delay_secs_;
  int web_contents_timeout_secs_;
  std::string drive_offline_endpoint_url_;
  std::string drive_hosted_app_id_;

  DISALLOW_COPY_AND_ASSIGN(DriveFirstRunController);
};

}  

#endif  
