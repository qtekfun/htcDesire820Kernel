// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_SPDYPROXY_DATA_REDUCTION_PROXY_SETTINGS_H_
#define CHROME_BROWSER_NET_SPDYPROXY_DATA_REDUCTION_PROXY_SETTINGS_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_member.h"
#include "net/base/network_change_notifier.h"
#include "net/url_request/url_fetcher_delegate.h"

class PrefService;

namespace net {
class AuthChallengeInfo;
class HostPortPair;
class HttpAuthCache;
class HttpNetworkSession;
class HttpResponseHeaders;
class URLFetcher;
}

namespace spdyproxy {

const unsigned int kNumDaysInHistory = 60;

const unsigned int kNumDaysInHistorySummary = 30;

COMPILE_ASSERT(kNumDaysInHistorySummary <= kNumDaysInHistory,
               DataReductionProxySettings_summary_too_long);

}  

class DataReductionProxySettings
    : public net::URLFetcherDelegate,
      public net::NetworkChangeNotifier::IPAddressObserver {
 public:
  typedef std::vector<long long> ContentLengthList;
  
  typedef std::vector<GURL> DataReductionProxyList;

  DataReductionProxySettings();
  virtual ~DataReductionProxySettings();

  void InitDataReductionProxySettings();

  
  
  static void InitDataReductionProxySession(net::HttpNetworkSession* session);

  
  
  
  
  
  void AddHostPatternToBypass(const std::string& pattern);

  
  
  
  virtual void AddURLPatternToBypass(const std::string& pattern);

  
  
  
  
  static bool IsDataReductionProxyAllowed();

  
  
  
  
  static bool IsDataReductionProxyPromoAllowed();

  
  
  static bool IsPreconnectHintingAllowed();

  
  
  static bool WasFetchedViaProxy(const net::HttpResponseHeaders* headers);

  
  static std::string GetDataReductionProxyOrigin();

  
  static std::string GetDataReductionProxyFallback();

  
  static DataReductionProxyList GetDataReductionProxies();

  
  
  bool IsAcceptableAuthChallenge(net::AuthChallengeInfo* auth_info);

  
  
  
  base::string16 GetTokenForAuthChallenge(net::AuthChallengeInfo* auth_info);

  
  bool IsDataReductionProxyEnabled();

  
  bool IsDataReductionProxyManaged();

  
  
  
  void SetDataReductionProxyEnabled(bool enabled);

  
  
  int64 GetDataReductionLastUpdateTime();

  
  
  
  ContentLengthList GetDailyOriginalContentLengths();

  
  
  ContentLengthList GetDailyReceivedContentLengths();

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

 protected:
  void InitPrefMembers();

  virtual net::URLFetcher* GetURLFetcher();

  
  virtual PrefService* GetOriginalProfilePrefs();
  virtual PrefService* GetLocalStatePrefs();

  void GetContentLengths(unsigned int days,
                         int64* original_content_length,
                         int64* received_content_length,
                         int64* last_update_time);
  ContentLengthList GetDailyContentLengths(const char* pref_name);

  
  
  
  
  virtual void SetProxyConfigs(bool enabled, bool restricted, bool at_startup);

  
  
  virtual void RecordDataReductionInit();

  virtual void AddDefaultProxyBypassRules();

  
  
  virtual void LogProxyState(bool enabled, bool restricted, bool at_startup);

  
  std::vector<std::string> BypassRules() { return bypass_rules_;}

 private:
  friend class DataReductionProxySettingsTestBase;
  friend class DataReductionProxySettingsTest;
  FRIEND_TEST_ALL_PREFIXES(DataReductionProxySettingsTest,
                           TestAuthenticationInit);
  FRIEND_TEST_ALL_PREFIXES(DataReductionProxySettingsTest,
                           TestAuthHashGeneration);
  FRIEND_TEST_ALL_PREFIXES(DataReductionProxySettingsTest,
                           TestAuthHashGenerationWithOriginSetViaSwitch);
  FRIEND_TEST_ALL_PREFIXES(DataReductionProxySettingsTest,
                           TestResetDataReductionStatistics);
  FRIEND_TEST_ALL_PREFIXES(DataReductionProxySettingsTest,
                           TestIsProxyEnabledOrManaged);
  FRIEND_TEST_ALL_PREFIXES(DataReductionProxySettingsTest,
                           TestContentLengths);
  FRIEND_TEST_ALL_PREFIXES(DataReductionProxySettingsTest,
                           TestGetDailyContentLengths);
  FRIEND_TEST_ALL_PREFIXES(DataReductionProxySettingsTest,
                           TestMaybeActivateDataReductionProxy);
  FRIEND_TEST_ALL_PREFIXES(DataReductionProxySettingsTest,
                           TestOnIPAddressChanged);
  FRIEND_TEST_ALL_PREFIXES(DataReductionProxySettingsTest,
                           TestOnProxyEnabledPrefChange);
  FRIEND_TEST_ALL_PREFIXES(DataReductionProxySettingsTest,
                           TestInitDataReductionProxyOn);
  FRIEND_TEST_ALL_PREFIXES(DataReductionProxySettingsTest,
                           TestInitDataReductionProxyOff);
  FRIEND_TEST_ALL_PREFIXES(DataReductionProxySettingsTest,
                           TestBypassList);

  
  virtual void OnIPAddressChanged() OVERRIDE;

  
  
  static void InitDataReductionAuthentication(net::HttpAuthCache* auth_cache);

  void OnProxyEnabledPrefChange();

  void ResetDataReductionStatistics();

  void MaybeActivateDataReductionProxy(bool at_startup);

  
  
  
  void ProbeWhetherDataReductionProxyIsAvailable();
  std::string GetProxyCheckURL();

  
  
  
  static base::string16 AuthHashForSalt(int64 salt);

  std::vector<std::string> bypass_rules_;

  bool restricted_by_carrier_;
  bool enabled_by_user_;

  scoped_ptr<net::URLFetcher> fetcher_;
  BooleanPrefMember spdy_proxy_auth_enabled_;

  DISALLOW_COPY_AND_ASSIGN(DataReductionProxySettings);
};

#endif  
