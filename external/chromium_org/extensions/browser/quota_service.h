// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef EXTENSIONS_BROWSER_QUOTA_SERVICE_H_
#define EXTENSIONS_BROWSER_QUOTA_SERVICE_H_

#include <list>
#include <map>
#include <string>

#include "base/compiler_specific.h"
#include "base/containers/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "base/values.h"

class ExtensionFunction;

namespace extensions {
class QuotaLimitHeuristic;
class TestResetQuotaFunction;

typedef std::list<QuotaLimitHeuristic*> QuotaLimitHeuristics;

class QuotaService : public base::NonThreadSafe {
 public:
  
  
  class TimedLimit;
  class SustainedLimit;

  QuotaService();
  virtual ~QuotaService();

  
  
  
  
  std::string Assess(const std::string& extension_id,
                     ExtensionFunction* function,
                     const base::ListValue* args,
                     const base::TimeTicks& event_time);

 private:
  friend class extensions::TestResetQuotaFunction;
  typedef std::string ExtensionId;
  typedef std::string FunctionName;
  
  typedef std::map<FunctionName, QuotaLimitHeuristics> FunctionHeuristicsMap;

  
  
  
  
  
  
  void Purge();
  void PurgeFunctionHeuristicsMap(FunctionHeuristicsMap* map);
  base::RepeatingTimer<QuotaService> purge_timer_;

  
  
  
  
  
  std::map<ExtensionId, FunctionHeuristicsMap> function_heuristics_;

  
  
  
  typedef std::map<std::string, std::string> ViolationErrorMap;
  ViolationErrorMap violation_errors_;

  DISALLOW_COPY_AND_ASSIGN(QuotaService);
};

class QuotaLimitHeuristic {
 public:
  
  
  struct Config {
    
    
    int64 refill_token_count;

    
    base::TimeDelta refill_interval;
  };

  
  
  
  
  
  
  
  class Bucket {
   public:
    Bucket() : num_tokens_(0) {}
    
    
    bool DeductToken() { return num_tokens_-- > 0; }

    
    bool has_tokens() const { return num_tokens_ > 0; }

    
    
    
    void Reset(const Config& config, const base::TimeTicks& start);

    
    
    const base::TimeTicks& expiration() { return expiration_; }

   private:
    base::TimeTicks expiration_;
    int64 num_tokens_;
    DISALLOW_COPY_AND_ASSIGN(Bucket);
  };
  typedef std::list<Bucket*> BucketList;

  
  
  
  class BucketMapper {
   public:
    virtual ~BucketMapper() {}
    
    
    
    
    
    virtual void GetBucketsForArgs(const base::ListValue* args,
                                   BucketList* buckets) = 0;
  };

  
  
  class SingletonBucketMapper : public BucketMapper {
   public:
    SingletonBucketMapper() {}
    virtual ~SingletonBucketMapper() {}
    virtual void GetBucketsForArgs(const base::ListValue* args,
                                   BucketList* buckets) OVERRIDE;

   private:
    Bucket bucket_;
    DISALLOW_COPY_AND_ASSIGN(SingletonBucketMapper);
  };

  
  QuotaLimitHeuristic(const Config& config,
                      BucketMapper* map,
                      const std::string& name);
  virtual ~QuotaLimitHeuristic();

  
  
  
  
  bool ApplyToArgs(const base::ListValue* args,
                   const base::TimeTicks& event_time);

  
  std::string GetError() const;

 protected:
  const Config& config() { return config_; }

  
  
  virtual bool Apply(Bucket* bucket, const base::TimeTicks& event_time) = 0;

 private:
  friend class QuotaLimitHeuristicTest;

  const Config config_;

  
  scoped_ptr<BucketMapper> bucket_mapper_;

  
  std::string name_;

  DISALLOW_COPY_AND_ASSIGN(QuotaLimitHeuristic);
};

class QuotaService::TimedLimit : public QuotaLimitHeuristic {
 public:
  TimedLimit(const Config& config, BucketMapper* map, const std::string& name)
      : QuotaLimitHeuristic(config, map, name) {}
  virtual bool Apply(Bucket* bucket,
                     const base::TimeTicks& event_time) OVERRIDE;
};

class QuotaService::SustainedLimit : public QuotaLimitHeuristic {
 public:
  SustainedLimit(const base::TimeDelta& sustain,
                 const Config& config,
                 BucketMapper* map,
                 const std::string& name);
  virtual bool Apply(Bucket* bucket,
                     const base::TimeTicks& event_time) OVERRIDE;

 private:
  
  
  const int64 repeat_exhaustion_allowance_;
  int64 num_available_repeat_exhaustions_;
};

}  

#endif  
