// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GAIA_OAUTH2_TOKEN_SERVICE_H_
#define GOOGLE_APIS_GAIA_OAUTH2_TOKEN_SERVICE_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "google_apis/gaia/google_service_auth_error.h"
#include "google_apis/gaia/oauth2_access_token_consumer.h"
#include "google_apis/gaia/oauth2_access_token_fetcher.h"

namespace net {
class URLRequestContextGetter;
}

class GoogleServiceAuthError;

class OAuth2TokenService : public base::NonThreadSafe {
 public:
  
  class Request {
   public:
    virtual ~Request();
    virtual std::string GetAccountId() const = 0;
   protected:
    Request();
  };

  
  
  class Consumer {
   public:
    Consumer();
    virtual ~Consumer();
    
    
    virtual void OnGetTokenSuccess(const Request* request,
                                   const std::string& access_token,
                                   const base::Time& expiration_time) = 0;
    virtual void OnGetTokenFailure(const Request* request,
                                   const GoogleServiceAuthError& error) = 0;
  };

  
  
  class Observer {
   public:
    
    
    
    
    virtual void OnRefreshTokenAvailable(const std::string& account_id) {}
    
    
    virtual void OnRefreshTokenRevoked(const std::string& account_id) {}
    
    
    virtual void OnRefreshTokensLoaded() {}
   protected:
    virtual ~Observer() {}
  };

  
  typedef std::set<std::string> ScopeSet;

  OAuth2TokenService();
  virtual ~OAuth2TokenService();

  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  
  
  
  
  
  
  scoped_ptr<Request> StartRequest(const std::string& account_id,
                                   const ScopeSet& scopes,
                                   Consumer* consumer);

  
  
  
  scoped_ptr<Request> StartRequestForClient(
      const std::string& account_id,
      const std::string& client_id,
      const std::string& client_secret,
      const ScopeSet& scopes,
      Consumer* consumer);

  
  
  
  scoped_ptr<Request> StartRequestWithContext(
      const std::string& account_id,
      net::URLRequestContextGetter* getter,
      const ScopeSet& scopes,
      Consumer* consumer);

  
  
  virtual std::vector<std::string> GetAccounts();

  
  
  virtual bool RefreshTokenIsAvailable(const std::string& account_id);

  
  
  
  
  
  void InvalidateToken(const std::string& account_id,
                       const ScopeSet& scopes,
                       const std::string& access_token);

  
  
  void InvalidateTokenForClient(const std::string& account_id,
                                const std::string& client_id,
                                const ScopeSet& scopes,
                                const std::string& access_token);


  
  int cache_size_for_testing() const;
  void set_max_authorization_token_fetch_retries_for_testing(int max_retries);
  
  size_t GetNumPendingRequestsForTesting(
      const std::string& client_id,
      const std::string& account_id,
      const ScopeSet& scopes) const;

 protected:
  
  
  
  class RequestImpl : public base::SupportsWeakPtr<RequestImpl>,
                      public base::NonThreadSafe,
                      public Request {
   public:
    
    explicit RequestImpl(const std::string& account_id, Consumer* consumer);
    virtual ~RequestImpl();

    
    virtual std::string GetAccountId() const OVERRIDE;

    
    void InformConsumer(const GoogleServiceAuthError& error,
                        const std::string& access_token,
                        const base::Time& expiration_date);

   private:
    
    const std::string account_id_;
    Consumer* const consumer_;
  };

  
  
  virtual std::string GetRefreshToken(const std::string& account_id) = 0;

  
  virtual void UpdateAuthError(
      const std::string& account_id,
      const GoogleServiceAuthError& error);

  
  
  
  virtual void RegisterCacheEntry(const std::string& client_id,
                                  const std::string& account_id,
                                  const ScopeSet& scopes,
                                  const std::string& access_token,
                                  const base::Time& expiration_date);

  
  void ClearCache();

  
  
  
  void ClearCacheForAccount(const std::string& account_id);

  
  void CancelAllRequests();

  
  void CancelRequestsForAccount(const std::string& account_id);

  
  virtual void FireRefreshTokenAvailable(const std::string& account_id);
  virtual void FireRefreshTokenRevoked(const std::string& account_id);
  virtual void FireRefreshTokensLoaded();

  
  
  
  virtual scoped_ptr<RequestImpl> CreateRequest(
      const std::string& account_id,
      Consumer* consumer);

  
  
  virtual void FetchOAuth2Token(RequestImpl* request,
                                const std::string& account_id,
                                net::URLRequestContextGetter* getter,
                                const std::string& client_id,
                                const std::string& client_secret,
                                const ScopeSet& scopes);

  
  
  
  virtual void InvalidateOAuth2Token(const std::string& account_id,
                                     const std::string& client_id,
                                     const ScopeSet& scopes,
                                     const std::string& access_token);

 private:
  class Fetcher;
  friend class Fetcher;

  
  struct RequestParameters {
    RequestParameters(const std::string& client_id,
                      const std::string& account_id,
                      const ScopeSet& scopes);
    ~RequestParameters();
    bool operator<(const RequestParameters& params) const;

    
    std::string client_id;
    
    std::string account_id;
    
    ScopeSet scopes;
  };

  typedef std::map<RequestParameters, Fetcher*> PendingFetcherMap;

  
  
  virtual net::URLRequestContextGetter* GetRequestContext() = 0;

  
  struct CacheEntry {
    std::string access_token;
    base::Time expiration_date;
  };

  
  
  
  scoped_ptr<Request> StartRequestForClientWithContext(
      const std::string& account_id,
      net::URLRequestContextGetter* getter,
      const std::string& client_id,
      const std::string& client_secret,
      const ScopeSet& scopes,
      Consumer* consumer);

  
  
  bool HasCacheEntry(const RequestParameters& client_scopes);

  
  
  void StartCacheLookupRequest(RequestImpl* request,
                               const RequestParameters& client_scopes,
                               Consumer* consumer);

  
  
  
  
  const CacheEntry* GetCacheEntry(const RequestParameters& client_scopes);

  
  
  bool RemoveCacheEntry(const RequestParameters& client_scopes,
                        const std::string& token_to_remove);

  
  void OnFetchComplete(Fetcher* fetcher);

  
  void CancelFetchers(std::vector<Fetcher*> fetchers_to_cancel);

  
  typedef std::map<RequestParameters, CacheEntry> TokenCache;
  TokenCache token_cache_;

  
  
  PendingFetcherMap pending_fetchers_;

  
  
  ObserverList<Observer, true> observer_list_;

  
  static int max_fetch_retry_num_;

  FRIEND_TEST_ALL_PREFIXES(OAuth2TokenServiceTest, RequestParametersOrderTest);
  FRIEND_TEST_ALL_PREFIXES(OAuth2TokenServiceTest,
                           SameScopesRequestedForDifferentClients);

  DISALLOW_COPY_AND_ASSIGN(OAuth2TokenService);
};

#endif  
