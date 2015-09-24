// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.







#ifndef BASE_METRICS_FIELD_TRIAL_H_
#define BASE_METRICS_FIELD_TRIAL_H_

#include <map>
#include <string>
#include <vector>

#include "base/base_export.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list_threadsafe.h"
#include "base/synchronization/lock.h"
#include "base/time/time.h"

namespace base {

class FieldTrialList;

class BASE_EXPORT FieldTrial : public RefCounted<FieldTrial> {
 public:
  typedef int Probability;  

  
  enum RandomizationType {
    
    
    
    ONE_TIME_RANDOMIZED,
    
    
    SESSION_RANDOMIZED,
  };

  
  
  class BASE_EXPORT EntropyProvider {
   public:
    virtual ~EntropyProvider();

    
    
    
    
    
    virtual double GetEntropyForTrial(const std::string& trial_name,
                                      uint32 randomization_seed) const = 0;
  };

  
  struct ActiveGroup {
    std::string trial_name;
    std::string group_name;
  };

  typedef std::vector<ActiveGroup> ActiveGroups;

  
  
  static const int kNotFinalized;

  
  
  
  
  
  
  
  void Disable();

  
  
  
  
  int AppendGroup(const std::string& name, Probability group_probability);

  
  const std::string& trial_name() const { return trial_name_; }

  
  
  
  
  
  int group();

  
  
  const std::string& group_name();

  
  
  
  
  
  
  
  
  void SetForced();

  
  static void EnableBenchmarking();

  
  
  
  
  
  
  
  
  
  
  static FieldTrial* CreateSimulatedFieldTrial(
      const std::string& trial_name,
      Probability total_probability,
      const std::string& default_group_name,
      double entropy_value);

 private:
  
  FRIEND_TEST_ALL_PREFIXES(FieldTrialTest, Registration);
  FRIEND_TEST_ALL_PREFIXES(FieldTrialTest, AbsoluteProbabilities);
  FRIEND_TEST_ALL_PREFIXES(FieldTrialTest, RemainingProbability);
  FRIEND_TEST_ALL_PREFIXES(FieldTrialTest, FiftyFiftyProbability);
  FRIEND_TEST_ALL_PREFIXES(FieldTrialTest, MiddleProbabilities);
  FRIEND_TEST_ALL_PREFIXES(FieldTrialTest, OneWinner);
  FRIEND_TEST_ALL_PREFIXES(FieldTrialTest, DisableProbability);
  FRIEND_TEST_ALL_PREFIXES(FieldTrialTest, ActiveGroups);
  FRIEND_TEST_ALL_PREFIXES(FieldTrialTest, ActiveGroupsNotFinalized);
  FRIEND_TEST_ALL_PREFIXES(FieldTrialTest, Save);
  FRIEND_TEST_ALL_PREFIXES(FieldTrialTest, DuplicateRestore);
  FRIEND_TEST_ALL_PREFIXES(FieldTrialTest, SetForcedTurnFeatureOff);
  FRIEND_TEST_ALL_PREFIXES(FieldTrialTest, SetForcedTurnFeatureOn);
  FRIEND_TEST_ALL_PREFIXES(FieldTrialTest, SetForcedChangeDefault_Default);
  FRIEND_TEST_ALL_PREFIXES(FieldTrialTest, SetForcedChangeDefault_NonDefault);
  FRIEND_TEST_ALL_PREFIXES(FieldTrialTest, FloatBoundariesGiveEqualGroupSizes);
  FRIEND_TEST_ALL_PREFIXES(FieldTrialTest, DoesNotSurpassTotalProbability);

  friend class base::FieldTrialList;

  friend class RefCounted<FieldTrial>;

  
  
  
  static const int kDefaultGroupNumber;

  
  
  FieldTrial(const std::string& trial_name,
             Probability total_probability,
             const std::string& default_group_name,
             double entropy_value);
  virtual ~FieldTrial();

  
  std::string default_group_name() const { return default_group_name_; }

  
  
  void SetTrialRegistered();

  
  void SetGroupChoice(const std::string& group_name, int number);

  
  
  
  void FinalizeGroupChoice();

  
  
  
  
  
  
  bool GetActiveGroup(ActiveGroup* active_group) const;

  
  std::string group_name_internal() const { return group_name_; }

  
  const std::string trial_name_;

  
  
  const Probability divisor_;

  
  const std::string default_group_name_;

  
  
  
  Probability random_;

  
  Probability accumulated_group_probability_;

  
  int next_group_number_;

  
  
  int group_;

  
  
  std::string group_name_;

  
  
  bool enable_field_trial_;

  
  
  bool forced_;

  
  bool group_reported_;

  
  
  bool trial_registered_;

  
  
  static bool enable_benchmarking_;

  DISALLOW_COPY_AND_ASSIGN(FieldTrial);
};

class BASE_EXPORT FieldTrialList {
 public:
  
  
  enum FieldTrialActivationMode {
    DONT_ACTIVATE_TRIALS,
    ACTIVATE_TRIALS,
  };

  
  
  
  static const char kPersistentStringSeparator;  

  
  
  static int kNoExpirationYear;

  
  class BASE_EXPORT Observer {
   public:
    
    virtual void OnFieldTrialGroupFinalized(const std::string& trial_name,
                                            const std::string& group_name) = 0;

   protected:
    virtual ~Observer();
  };

  
  
  
  
  
  
  explicit FieldTrialList(const FieldTrial::EntropyProvider* entropy_provider);

  
  ~FieldTrialList();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static FieldTrial* FactoryGetFieldTrial(
      const std::string& trial_name,
      FieldTrial::Probability total_probability,
      const std::string& default_group_name,
      const int year,
      const int month,
      const int day_of_month,
      FieldTrial::RandomizationType randomization_type,
      int* default_group_number);

  
  
  
  
  
  
  
  static FieldTrial* FactoryGetFieldTrialWithRandomizationSeed(
      const std::string& trial_name,
      FieldTrial::Probability total_probability,
      const std::string& default_group_name,
      const int year,
      const int month,
      const int day_of_month,
      FieldTrial::RandomizationType randomization_type,
      uint32 randomization_seed,
      int* default_group_number);

  
  
  static FieldTrial* Find(const std::string& name);

  
  
  static int FindValue(const std::string& name);

  
  
  static std::string FindFullName(const std::string& name);

  
  static bool TrialExists(const std::string& name);

  
  
  
  
  
  
  
  
  static void StatesToString(std::string* output);

  
  
  
  
  static void GetActiveFieldTrialGroups(
      FieldTrial::ActiveGroups* active_groups);

  
  
  
  
  
  
  
  
  static bool CreateTrialsFromString(const std::string& prior_trials,
                                     FieldTrialActivationMode mode);

  
  
  
  
  
  
  static FieldTrial* CreateFieldTrial(const std::string& name,
                                      const std::string& group_name);

  
  
  
  static void AddObserver(Observer* observer);

  
  static void RemoveObserver(Observer* observer);

  
  static void NotifyFieldTrialGroupSelection(FieldTrial* field_trial);

  
  static size_t GetFieldTrialCount();

 private:
  
  typedef std::map<std::string, FieldTrial*> RegistrationList;

  
  
  static const FieldTrial::EntropyProvider*
      GetEntropyProviderForOneTimeRandomization();

  
  FieldTrial* PreLockedFind(const std::string& name);

  
  
  
  static void Register(FieldTrial* trial);

  static FieldTrialList* global_;  

  
  
  
  
  static bool used_without_global_;

  
  base::Lock lock_;
  RegistrationList registered_;

  
  
  scoped_ptr<const FieldTrial::EntropyProvider> entropy_provider_;

  
  scoped_refptr<ObserverListThreadSafe<Observer> > observer_list_;

  DISALLOW_COPY_AND_ASSIGN(FieldTrialList);
};

}  

#endif  
