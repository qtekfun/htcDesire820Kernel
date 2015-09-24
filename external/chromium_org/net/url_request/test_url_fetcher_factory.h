// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_TEST_URL_FETCHER_FACTORY_H_
#define NET_URL_REQUEST_TEST_URL_FETCHER_FACTORY_H_

#include <list>
#include <map>
#include <string>
#include <utility>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "net/http/http_request_headers.h"
#include "net/http/http_status_code.h"
#include "net/url_request/url_fetcher_factory.h"
#include "net/url_request/url_request_status.h"
#include "url/gurl.h"

namespace net {

class ScopedURLFetcherFactory : public base::NonThreadSafe {
 public:
  explicit ScopedURLFetcherFactory(URLFetcherFactory* factory);
  virtual ~ScopedURLFetcherFactory();

 private:
  DISALLOW_COPY_AND_ASSIGN(ScopedURLFetcherFactory);
};


class TestURLFetcherFactory;
class TestURLFetcher : public URLFetcher {
 public:
  
  
  
  class DelegateForTests {
   public:
    
    virtual void OnRequestStart(int fetcher_id) = 0;

    
    
    virtual void OnChunkUpload(int fetcher_id) = 0;

    
    virtual void OnRequestEnd(int fetcher_id) = 0;
  };

  TestURLFetcher(int id,
                 const GURL& url,
                 URLFetcherDelegate* d);
  virtual ~TestURLFetcher();

  
  virtual void SetUploadData(const std::string& upload_content_type,
                             const std::string& upload_content) OVERRIDE;
  virtual void SetUploadFilePath(
      const std::string& upload_content_type,
      const base::FilePath& file_path,
      uint64 range_offset,
      uint64 range_length,
      scoped_refptr<base::TaskRunner> file_task_runner) OVERRIDE;
  virtual void SetChunkedUpload(
      const std::string& upload_content_type) OVERRIDE;
  
  
  virtual void AppendChunkToUpload(const std::string& data,
                                   bool is_last_chunk) OVERRIDE;
  virtual void SetLoadFlags(int load_flags) OVERRIDE;
  virtual int GetLoadFlags() const OVERRIDE;
  virtual void SetReferrer(const std::string& referrer) OVERRIDE;
  virtual void SetExtraRequestHeaders(
      const std::string& extra_request_headers) OVERRIDE;
  virtual void AddExtraRequestHeader(const std::string& header_line) OVERRIDE;
  virtual void GetExtraRequestHeaders(
      HttpRequestHeaders* headers) const OVERRIDE;
  virtual void SetRequestContext(
      URLRequestContextGetter* request_context_getter) OVERRIDE;
  virtual void SetFirstPartyForCookies(
      const GURL& first_party_for_cookies) OVERRIDE;
  virtual void SetURLRequestUserData(
      const void* key,
      const CreateDataCallback& create_data_callback) OVERRIDE;
  virtual void SetStopOnRedirect(bool stop_on_redirect) OVERRIDE;
  virtual void SetAutomaticallyRetryOn5xx(bool retry) OVERRIDE;
  virtual void SetMaxRetriesOn5xx(int max_retries) OVERRIDE;
  virtual int GetMaxRetriesOn5xx() const OVERRIDE;
  virtual base::TimeDelta GetBackoffDelay() const OVERRIDE;
  virtual void SetAutomaticallyRetryOnNetworkChanges(int max_retries) OVERRIDE;
  virtual void SaveResponseToFileAtPath(
      const base::FilePath& file_path,
      scoped_refptr<base::SequencedTaskRunner> file_task_runner) OVERRIDE;
  virtual void SaveResponseToTemporaryFile(
      scoped_refptr<base::SequencedTaskRunner> file_task_runner) OVERRIDE;
  virtual void SaveResponseWithWriter(
      scoped_ptr<URLFetcherResponseWriter> response_writer) OVERRIDE;
  virtual HttpResponseHeaders* GetResponseHeaders() const OVERRIDE;
  virtual HostPortPair GetSocketAddress() const OVERRIDE;
  virtual bool WasFetchedViaProxy() const OVERRIDE;
  virtual void Start() OVERRIDE;

  
  
  
  virtual const GURL& GetOriginalURL() const OVERRIDE;
  virtual const GURL& GetURL() const OVERRIDE;
  virtual const URLRequestStatus& GetStatus() const OVERRIDE;
  virtual int GetResponseCode() const OVERRIDE;
  virtual const ResponseCookies& GetCookies() const OVERRIDE;
  virtual void ReceivedContentWasMalformed() OVERRIDE;
  
  virtual bool GetResponseAsString(
      std::string* out_response_string) const OVERRIDE;
  virtual bool GetResponseAsFilePath(
      bool take_ownership, base::FilePath* out_response_path) const OVERRIDE;

  
  
  
  void set_owner(TestURLFetcherFactory* owner) { owner_ = owner; }

  
  int id() const { return id_; }

  
  const std::string& upload_data() const { return upload_data_; }
  const base::FilePath& upload_file_path() const { return upload_file_path_; }

  
  const std::list<std::string>& upload_chunks() const { return chunks_; }

  
  bool did_receive_last_chunk() const { return did_receive_last_chunk_; }

  
  URLFetcherDelegate* delegate() const { return delegate_; }

  void set_url(const GURL& url) { fake_url_ = url; }
  void set_status(const URLRequestStatus& status);
  void set_response_code(int response_code) {
    fake_response_code_ = response_code;
  }
  void set_cookies(const ResponseCookies& c) { fake_cookies_ = c; }
  void set_was_fetched_via_proxy(bool flag);
  void set_response_headers(scoped_refptr<HttpResponseHeaders> headers);
  void set_backoff_delay(base::TimeDelta backoff_delay);
  void SetDelegateForTests(DelegateForTests* delegate_for_tests);

  
  void SetResponseString(const std::string& response);

  
  void SetResponseFilePath(const base::FilePath& path);

 private:
  enum ResponseDestinationType {
    STRING,  
    TEMP_FILE  
  };

  TestURLFetcherFactory* owner_;
  const int id_;
  const GURL original_url_;
  URLFetcherDelegate* delegate_;
  DelegateForTests* delegate_for_tests_;
  std::string upload_data_;
  base::FilePath upload_file_path_;
  std::list<std::string> chunks_;
  bool did_receive_last_chunk_;

  
  
  
  
  int fake_load_flags_;
  GURL fake_url_;
  URLRequestStatus fake_status_;
  int fake_response_code_;
  ResponseCookies fake_cookies_;
  ResponseDestinationType fake_response_destination_;
  std::string fake_response_string_;
  base::FilePath fake_response_file_path_;
  bool fake_was_fetched_via_proxy_;
  scoped_refptr<HttpResponseHeaders> fake_response_headers_;
  HttpRequestHeaders fake_extra_request_headers_;
  int fake_max_retries_;
  base::TimeDelta fake_backoff_delay_;

  DISALLOW_COPY_AND_ASSIGN(TestURLFetcher);
};

typedef TestURLFetcher::DelegateForTests TestURLFetcherDelegateForTests;

class TestURLFetcherFactory : public URLFetcherFactory,
                              public ScopedURLFetcherFactory {
 public:
  TestURLFetcherFactory();
  virtual ~TestURLFetcherFactory();

  virtual URLFetcher* CreateURLFetcher(
      int id,
      const GURL& url,
      URLFetcher::RequestType request_type,
      URLFetcherDelegate* d) OVERRIDE;
  TestURLFetcher* GetFetcherByID(int id) const;
  void RemoveFetcherFromMap(int id);
  void SetDelegateForTests(TestURLFetcherDelegateForTests* delegate_for_tests);
  void set_remove_fetcher_on_delete(bool remove_fetcher_on_delete) {
    remove_fetcher_on_delete_ = remove_fetcher_on_delete;
  }

 private:
  
  typedef std::map<int, TestURLFetcher*> Fetchers;
  Fetchers fetchers_;
  TestURLFetcherDelegateForTests* delegate_for_tests_;
  
  
  bool remove_fetcher_on_delete_;

  DISALLOW_COPY_AND_ASSIGN(TestURLFetcherFactory);
};


class FakeURLFetcher : public TestURLFetcher {
 public:
  
  FakeURLFetcher(const GURL& url,
                 URLFetcherDelegate* d,
                 const std::string& response_data,
                 HttpStatusCode response_code,
                 URLRequestStatus::Status status);

  
  
  virtual void Start() OVERRIDE;

  virtual const GURL& GetURL() const OVERRIDE;

  virtual ~FakeURLFetcher();

 private:
  
  
  void RunDelegate();

  base::WeakPtrFactory<FakeURLFetcher> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(FakeURLFetcher);
};


class FakeURLFetcherFactory : public URLFetcherFactory,
                              public ScopedURLFetcherFactory {
 public:
  
  
  
  
  
  
  
  
  
  
  typedef base::Callback<scoped_ptr<FakeURLFetcher>(
      const GURL&,
      URLFetcherDelegate*,
      const std::string&,
      HttpStatusCode,
      URLRequestStatus::Status)> FakeURLFetcherCreator;

  
  
  
  
  explicit FakeURLFetcherFactory(URLFetcherFactory* default_factory);

  
  
  
  
  
  
  
  FakeURLFetcherFactory(URLFetcherFactory* default_factory,
                        const FakeURLFetcherCreator& creator);

  virtual ~FakeURLFetcherFactory();

  
  
  
  
  
  virtual URLFetcher* CreateURLFetcher(
      int id,
      const GURL& url,
      URLFetcher::RequestType request_type,
      URLFetcherDelegate* d) OVERRIDE;

  
  
  
  
  
  
  
  void SetFakeResponse(const GURL& url,
                       const std::string& response_data,
                       HttpStatusCode response_code,
                       URLRequestStatus::Status status);

  
  
  void ClearFakeResponses();

 private:
  struct FakeURLResponse {
    std::string response_data;
    HttpStatusCode response_code;
    URLRequestStatus::Status status;
  };
  typedef std::map<GURL, FakeURLResponse> FakeResponseMap;

  const FakeURLFetcherCreator creator_;
  FakeResponseMap fake_responses_;
  URLFetcherFactory* const default_factory_;

  static scoped_ptr<FakeURLFetcher> DefaultFakeURLFetcherCreator(
      const GURL& url,
      URLFetcherDelegate* delegate,
      const std::string& response_data,
      HttpStatusCode response_code,
      URLRequestStatus::Status status);
  DISALLOW_COPY_AND_ASSIGN(FakeURLFetcherFactory);
};

class URLFetcherImplFactory : public URLFetcherFactory {
 public:
  URLFetcherImplFactory();
  virtual ~URLFetcherImplFactory();

  
  virtual URLFetcher* CreateURLFetcher(
      int id,
      const GURL& url,
      URLFetcher::RequestType request_type,
      URLFetcherDelegate* d) OVERRIDE;

};

}  

#endif  
