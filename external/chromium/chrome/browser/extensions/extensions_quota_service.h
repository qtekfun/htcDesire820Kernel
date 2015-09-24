// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSIONS_QUOTA_SERVICE_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSIONS_QUOTA_SERVICE_H_
#pragma once

#include <list>
#include <map>
#include <string>

#include "base/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "base/time.h"
#include "base/timer.h"
#include "base/values.h"

class ExtensionFunction;
class QuotaLimitHeuristic;
typedef std::list<QuotaLimitHeuristic*> QuotaLimitHeuristics;

class ExtensionsQuotaService {
 public:
  
  
  class TimedLimit;
  class SustainedLimit;

  ExtensionsQuotaService();
  ~ExtensionsQuotaService();

  
  
  
  
  bool Assess(const std::string& extension_id, ExtensionFunction* function,
              const ListValue* args, const base::TimeTicks& event_time);
 private:
  friend class ExtensionTestQuotaResetFunction;
  typedef std::map<std::string, QuotaLimitHeuristics> FunctionHeuristicsMap;

  
  
  
  
  
  
  void Purge();
  void PurgeFunctionHeuristicsMap(FunctionHeuristicsMap* map);
  base::RepeatingTimer<ExtensionsQuotaService> purge_timer_;

  
  
  
  
  
  std::map<std::string, FunctionHeuristicsMap> function_heuristics_;

  
  
  
  base::hash_set<std::string> violators_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionsQuotaService);
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

  
  static const char kGenericOverQuotaError[];

  
  
  
  class BucketMapper {
   public:
    virtual ~BucketMapper() {}
    
    
    
    
    
    virtual void GetBucketsForArgs(const ListValue* args,
                                   BucketList* buckets) = 0;
  };

  
  explicit QuotaLimitHeuristic(const Config& config, BucketMapper* map);
  virtual ~QuotaLimitHeuristic();

  
  
  
  
  bool ApplyToArgs(const ListValue* args, const base::TimeTicks& event_time);

 protected:
  const Config& config() { return config_; }

  
  
  virtual bool Apply(Bucket* bucket, const base::TimeTicks& event_time) = 0;

 private:
  friend class QuotaLimitHeuristicTest;

  const Config config_;

  
  scoped_ptr<BucketMapper> bucket_mapper_;

  DISALLOW_COPY_AND_ASSIGN(QuotaLimitHeuristic);
};

class ExtensionsQuotaService::TimedLimit : public QuotaLimitHeuristic {
 public:
  explicit TimedLimit(const Config& config, BucketMapper* map)
      : QuotaLimitHeuristic(config, map) {}
  virtual bool Apply(Bucket* bucket, const base::TimeTicks& event_time);
};

class ExtensionsQuotaService::SustainedLimit : public QuotaLimitHeuristic {
 public:
  SustainedLimit(const base::TimeDelta& sustain,
                 const Config& config,
                 BucketMapper* map);
  virtual bool Apply(Bucket* bucket, const base::TimeTicks& event_time);
 private:
  
  
  const int64 repeat_exhaustion_allowance_;
  int64 num_available_repeat_exhaustions_;
};

#endif  
