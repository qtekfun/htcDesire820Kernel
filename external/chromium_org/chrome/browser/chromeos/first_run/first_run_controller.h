// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FIRST_RUN_FIRST_RUN_CONTROLLER_H_
#define CHROME_BROWSER_CHROMEOS_FIRST_RUN_FIRST_RUN_CONTROLLER_H_

#include <string>
#include <vector>

#include "ash/first_run/first_run_helper.h"
#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/ui/webui/chromeos/first_run/first_run_actor.h"

class Profile;

namespace content {
class WebContents;
}

namespace chromeos {

class FirstRunUIBrowserTest;

namespace first_run {
class Step;
}

class FirstRunController : public FirstRunActor::Delegate,
                           public ash::FirstRunHelper::Observer {
  typedef std::vector<linked_ptr<first_run::Step> > Steps;

 public:
  virtual ~FirstRunController();

  
  static void Start();

  
  static void Stop();

 private:
  friend class FirstRunUIBrowserTest;

  FirstRunController();
  void Init();
  void Finalize();

  static FirstRunController* GetInstanceForTest();

  
  virtual void OnActorInitialized() OVERRIDE;
  virtual void OnNextButtonClicked(const std::string& step_name) OVERRIDE;
  virtual void OnHelpButtonClicked() OVERRIDE;
  virtual void OnStepShown(const std::string& step_name) OVERRIDE;
  virtual void OnStepHidden(const std::string& step_name) OVERRIDE;
  virtual void OnActorFinalized() OVERRIDE;
  virtual void OnActorDestroyed() OVERRIDE;

  
  virtual void OnCancelled() OVERRIDE;

  void RegisterSteps();
  void ShowNextStep();
  void AdvanceStep();
  first_run::Step* GetCurrentStep() const;

  
  
  FirstRunActor* actor_;

  
  scoped_ptr<ash::FirstRunHelper> shell_helper_;

  
  Steps steps_;

  
  size_t current_step_index_;

  
  Profile* user_profile_;

  
  base::Closure on_actor_finalized_;

  
  content::WebContents* web_contents_for_tests_;

  
  base::Time start_time_;

  DISALLOW_COPY_AND_ASSIGN(FirstRunController);
};

}  

#endif  

