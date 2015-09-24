// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSING_DATA_BROWSING_DATA_QUOTA_HELPER_H_
#define CHROME_BROWSER_BROWSING_DATA_BROWSING_DATA_QUOTA_HELPER_H_

#include <list>
#include <string>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/message_loop/message_loop_proxy.h"
#include "base/sequenced_task_runner_helpers.h"
#include "webkit/common/quota/quota_types.h"

class BrowsingDataQuotaHelper;
class Profile;

struct BrowsingDataQuotaHelperDeleter {
  static void Destruct(const BrowsingDataQuotaHelper* helper);
};

class BrowsingDataQuotaHelper
    : public base::RefCountedThreadSafe<BrowsingDataQuotaHelper,
                                        BrowsingDataQuotaHelperDeleter> {
 public:
  
  
  struct QuotaInfo {
    QuotaInfo();
    explicit QuotaInfo(const std::string& host);
    QuotaInfo(const std::string& host,
              int64 temporary_usage,
              int64 persistent_usage,
              int64 syncable_usage);
    ~QuotaInfo();

    
    
    
    bool operator <(const QuotaInfo& rhs) const;
    bool operator ==(const QuotaInfo& rhs) const;

    std::string host;
    int64 temporary_usage;
    int64 persistent_usage;
    int64 syncable_usage;
  };

  typedef std::list<QuotaInfo> QuotaInfoArray;
  typedef base::Callback<void(const QuotaInfoArray&)> FetchResultCallback;

  static BrowsingDataQuotaHelper* Create(Profile* profile);

  virtual void StartFetching(const FetchResultCallback& callback) = 0;

  virtual void RevokeHostQuota(const std::string& host) = 0;

 protected:
  explicit BrowsingDataQuotaHelper(base::MessageLoopProxy* io_thread_);
  virtual ~BrowsingDataQuotaHelper();

 private:
  friend class base::DeleteHelper<BrowsingDataQuotaHelper>;
  friend struct BrowsingDataQuotaHelperDeleter;
  scoped_refptr<base::MessageLoopProxy> io_thread_;

  DISALLOW_COPY_AND_ASSIGN(BrowsingDataQuotaHelper);
};

#endif  
