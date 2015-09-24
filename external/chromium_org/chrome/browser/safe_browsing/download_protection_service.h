// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_DOWNLOAD_PROTECTION_SERVICE_H_
#define CHROME_BROWSER_SAFE_BROWSING_DOWNLOAD_PROTECTION_SERVICE_H_

#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/safe_browsing/database_manager.h"
#include "chrome/browser/safe_browsing/ui_manager.h"
#include "url/gurl.h"


namespace content {
class DownloadItem;
class PageNavigator;
}

namespace net {
class URLRequestContextGetter;
class X509Certificate;
}  

namespace safe_browsing {
class DownloadFeedbackService;
class SignatureUtil;

class DownloadProtectionService {
 public:
  enum DownloadCheckResult {
    SAFE,
    DANGEROUS,
    UNCOMMON,
    DANGEROUS_HOST,
    POTENTIALLY_UNWANTED
  };

  
  typedef base::Callback<void(DownloadCheckResult)> CheckDownloadCallback;

  
  
  
  DownloadProtectionService(
      SafeBrowsingService* sb_service,
      net::URLRequestContextGetter* request_context_getter);

  virtual ~DownloadProtectionService();

  
  
  
  
  // is finished and written to disk.
  virtual void CheckClientDownload(content::DownloadItem* item,
                                   const CheckDownloadCallback& callback);

  
  
  
  
  
  virtual void CheckDownloadUrl(const content::DownloadItem& item,
                                const CheckDownloadCallback& callback);

  
  
  virtual bool IsSupportedDownload(const content::DownloadItem& item,
                                   const base::FilePath& target_path) const;

  
  
  
  void ShowDetailsForDownload(const content::DownloadItem& item,
                              content::PageNavigator* navigator);

  
  
  
  
  void SetEnabled(bool enabled);

  bool enabled() const {
    return enabled_;
  }

  
  int64 download_request_timeout_ms() const {
    return download_request_timeout_ms_;
  }

  DownloadFeedbackService* feedback_service() {
    return feedback_service_.get();
  }

 protected:
  
  
  enum DownloadCheckResultReason {
    REASON_INVALID_URL,
    REASON_SB_DISABLED,
    REASON_WHITELISTED_URL,
    REASON_WHITELISTED_REFERRER,
    REASON_INVALID_REQUEST_PROTO,
    REASON_SERVER_PING_FAILED,
    REASON_INVALID_RESPONSE_PROTO,
    REASON_NOT_BINARY_FILE,
    REASON_REQUEST_CANCELED,
    REASON_DOWNLOAD_DANGEROUS,
    REASON_DOWNLOAD_SAFE,
    REASON_EMPTY_URL_CHAIN,
    DEPRECATED_REASON_HTTPS_URL,
    REASON_PING_DISABLED,
    REASON_TRUSTED_EXECUTABLE,
    REASON_OS_NOT_SUPPORTED,
    REASON_DOWNLOAD_UNCOMMON,
    REASON_DOWNLOAD_NOT_SUPPORTED,
    REASON_INVALID_RESPONSE_VERDICT,
    REASON_ARCHIVE_WITHOUT_BINARIES,
    REASON_DOWNLOAD_DANGEROUS_HOST,
    REASON_DOWNLOAD_POTENTIALLY_UNWANTED,
    REASON_MAX  
  };

 private:
  class CheckClientDownloadRequest;  
  friend class DownloadProtectionServiceTest;
  FRIEND_TEST_ALL_PREFIXES(DownloadProtectionServiceTest,
                           CheckClientDownloadValidateRequest);
  FRIEND_TEST_ALL_PREFIXES(DownloadProtectionServiceTest,
                           CheckClientDownloadSuccess);
  FRIEND_TEST_ALL_PREFIXES(DownloadProtectionServiceTest,
                           CheckClientDownloadHTTPS);
  FRIEND_TEST_ALL_PREFIXES(DownloadProtectionServiceTest,
                           CheckClientDownloadZip);
  FRIEND_TEST_ALL_PREFIXES(DownloadProtectionServiceTest,
                           CheckClientDownloadFetchFailed);
  FRIEND_TEST_ALL_PREFIXES(DownloadProtectionServiceTest,
                           TestDownloadRequestTimeout);
  FRIEND_TEST_ALL_PREFIXES(DownloadProtectionServiceTest,
                           CheckClientCrxDownloadSuccess);
  static const char kDownloadRequestUrl[];

  
  
  void CancelPendingRequests();

  
  
  void RequestFinished(CheckClientDownloadRequest* request);

  
  
  
  static void GetCertificateWhitelistStrings(
      const net::X509Certificate& certificate,
      const net::X509Certificate& issuer,
      std::vector<std::string>* whitelist_strings);

  
  static GURL GetDownloadRequestUrl();

  
  scoped_refptr<SafeBrowsingUIManager> ui_manager_;
  scoped_refptr<SafeBrowsingDatabaseManager> database_manager_;

  
  scoped_refptr<net::URLRequestContextGetter> request_context_getter_;

  
  
  
  std::set<scoped_refptr<CheckClientDownloadRequest> > download_requests_;

  
  bool enabled_;

  
  scoped_refptr<SignatureUtil> signature_util_;

  int64 download_request_timeout_ms_;

  scoped_ptr<DownloadFeedbackService> feedback_service_;

  DISALLOW_COPY_AND_ASSIGN(DownloadProtectionService);
};
}  

#endif  
