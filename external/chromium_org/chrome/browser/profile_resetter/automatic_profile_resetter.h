// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILE_RESETTER_AUTOMATIC_PROFILE_RESETTER_H_
#define CHROME_BROWSER_PROFILE_RESETTER_AUTOMATIC_PROFILE_RESETTER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string_piece.h"
#include "base/task_runner.h"
#include "chrome/browser/profile_resetter/automatic_profile_resetter_mementos.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

class AutomaticProfileResetterDelegate;
class Profile;

namespace base {
class DictionaryValue;
class ListValue;
}

// for any given profile, a "memento" that the prompt has appeared is written to
class AutomaticProfileResetter : public BrowserContextKeyedService {
 public:
  
  
  enum PromptResult {
    
    
    PROMPT_NOT_TRIGGERED,
    
    
    
    
    
    PROMPT_SHOWN_BUBBLE,
    
    
    PROMPT_ACTION_RESET,
    PROMPT_ACTION_NO_RESET,
    
    
    
    PROMPT_FOLLOWED_BY_WEBUI_RESET,
    PROMPT_FOLLOWED_BY_WEBUI_NO_RESET,
    
    
    
    
    PROMPT_NOT_SHOWN_BUBBLE_BUT_HAD_WEBUI_RESET,
    PROMPT_NOT_SHOWN_BUBBLE_BUT_HAD_WEBUI_NO_RESET,
    PROMPT_RESULT_MAX
  };

  explicit AutomaticProfileResetter(Profile* profile);
  virtual ~AutomaticProfileResetter();

  
  
  
  void Initialize();

  
  
  
  
  void Activate();

  
  
  
  void TriggerProfileReset(bool send_feedback);

  
  
  
  void SkipProfileReset();

  
  
  
  bool IsResetPromptFlowActive() const;

  
  
  bool ShouldShowResetBanner() const;

  
  void NotifyDidShowResetBubble();

  
  
  
  
  void NotifyDidOpenWebUIResetDialog();

  
  
  
  
  
  void NotifyDidCloseWebUIResetDialog(bool performed_reset);

  
  
  void NotifyDidCloseWebUIResetBanner();

  base::WeakPtr<AutomaticProfileResetter> AsWeakPtr() {
    return weak_ptr_factory_.GetWeakPtr();
  }

  
  void SetProgramForTesting(const std::string& program);

  
  void SetHashSeedForTesting(const std::string& hash_seed);

  
  void SetDelegateForTesting(
      scoped_ptr<AutomaticProfileResetterDelegate> delegate);

  
  
  void SetTaskRunnerForWaitingForTesting(
      const scoped_refptr<base::TaskRunner>& task_runner);

  
  virtual void Shutdown() OVERRIDE;

 private:
  class InputBuilder;
  struct EvaluationResults;

  enum State {
    STATE_UNINITIALIZED,
    STATE_INITIALIZED,
    STATE_DISABLED,
    STATE_WAITING_ON_DEPENDENCIES,
    STATE_READY,
    STATE_EVALUATING_CONDITIONS,
    
    
    STATE_HAS_TRIGGERED_PROMPT,
    
    
    STATE_HAS_SHOWN_BUBBLE,
    STATE_PERFORMING_RESET,
    STATE_DONE
  };

  
  
  void PrepareEvaluationFlow();

  
  void OnTemplateURLServiceIsLoaded();

  
  
  void OnLoadedModulesAreEnumerated();

  
  void OnDependencyIsReady();

  
  
  
  void BeginEvaluationFlow();

  
  
  
  void ContinueWithEvaluationFlow(
      scoped_ptr<base::DictionaryValue> program_input);

  
  
  
  
  
  
  static scoped_ptr<EvaluationResults> EvaluateConditionsOnWorkerPoolThread(
      const std::string& hash_seed,
      const std::string& program,
      scoped_ptr<base::DictionaryValue> program_input);

  
  
  virtual void ReportStatistics(uint32 satisfied_criteria_mask,
                                uint32 combined_status_mask);

  
  
  
  void FinishEvaluationFlow(scoped_ptr<EvaluationResults> results);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void BeginResetPromptFlow();

  
  
  
  void OnProfileSettingsResetCompleted();

  
  
  virtual void ReportPromptResult(PromptResult result);

  
  
  void PersistMementos();

  
  void FinishResetPromptFlow();

  Profile* profile_;

  State state_;
  bool enumeration_of_loaded_modules_ready_;
  bool template_url_service_ready_;
  bool has_already_dismissed_prompt_;

  scoped_ptr<InputBuilder> input_builder_;
  std::string hash_seed_;
  std::string program_;

  scoped_ptr<EvaluationResults> evaluation_results_;

  bool should_show_reset_banner_;

  scoped_ptr<AutomaticProfileResetterDelegate> delegate_;
  scoped_refptr<base::TaskRunner> task_runner_for_waiting_;

  base::WeakPtrFactory<AutomaticProfileResetter> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(AutomaticProfileResetter);
};

#endif  
