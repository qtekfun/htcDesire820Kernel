// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_FETCHER_CORE_H_
#define NET_URL_REQUEST_URL_FETCHER_CORE_H_

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/debug/stack_trace.h"
#include "base/files/file_path.h"
#include "base/lazy_instance.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer/timer.h"
#include "net/base/host_port_pair.h"
#include "net/http/http_request_headers.h"
#include "net/url_request/url_fetcher.h"
#include "net/url_request/url_request.h"
#include "net/url_request/url_request_status.h"
#include "url/gurl.h"

namespace base {
class SingleThreadTaskRunner;
}  

namespace net {
class DrainableIOBuffer;
class HttpResponseHeaders;
class IOBuffer;
class URLFetcherDelegate;
class URLFetcherResponseWriter;
class URLRequestContextGetter;
class URLRequestThrottlerEntryInterface;

class URLFetcherCore
    : public base::RefCountedThreadSafe<URLFetcherCore>,
      public URLRequest::Delegate {
 public:
  URLFetcherCore(URLFetcher* fetcher,
                 const GURL& original_url,
                 URLFetcher::RequestType request_type,
                 URLFetcherDelegate* d);

  
  
  
  
  
  
  void Start();

  
  
  void Stop();

  

  
  
  void SetUploadData(const std::string& upload_content_type,
                     const std::string& upload_content);
  void SetUploadFilePath(const std::string& upload_content_type,
                         const base::FilePath& file_path,
                         uint64 range_offset,
                         uint64 range_length,
                         scoped_refptr<base::TaskRunner> file_task_runner);
  void SetChunkedUpload(const std::string& upload_content_type);
  
  
  void AppendChunkToUpload(const std::string& data, bool is_last_chunk);
  
  
  void SetLoadFlags(int load_flags);
  int GetLoadFlags() const;
  void SetReferrer(const std::string& referrer);
  void SetExtraRequestHeaders(const std::string& extra_request_headers);
  void AddExtraRequestHeader(const std::string& header_line);
  void GetExtraRequestHeaders(HttpRequestHeaders* headers) const;
  void SetRequestContext(URLRequestContextGetter* request_context_getter);
  
  
  void SetFirstPartyForCookies(const GURL& first_party_for_cookies);
  
  
  void SetURLRequestUserData(
      const void* key,
      const URLFetcher::CreateDataCallback& create_data_callback);
  void SetStopOnRedirect(bool stop_on_redirect);
  void SetAutomaticallyRetryOn5xx(bool retry);
  void SetMaxRetriesOn5xx(int max_retries);
  int GetMaxRetriesOn5xx() const;
  base::TimeDelta GetBackoffDelay() const;
  void SetAutomaticallyRetryOnNetworkChanges(int max_retries);
  void SaveResponseToFileAtPath(
      const base::FilePath& file_path,
      scoped_refptr<base::SequencedTaskRunner> file_task_runner);
  void SaveResponseToTemporaryFile(
      scoped_refptr<base::SequencedTaskRunner> file_task_runner);
  void SaveResponseWithWriter(
      scoped_ptr<URLFetcherResponseWriter> response_writer);
  HttpResponseHeaders* GetResponseHeaders() const;
  HostPortPair GetSocketAddress() const;
  bool WasFetchedViaProxy() const;
  const GURL& GetOriginalURL() const;
  const GURL& GetURL() const;
  const URLRequestStatus& GetStatus() const;
  int GetResponseCode() const;
  const ResponseCookies& GetCookies() const;
  
  
  
  
  
  
  void ReceivedContentWasMalformed();
  bool GetResponseAsString(std::string* out_response_string) const;
  bool GetResponseAsFilePath(bool take_ownership,
                             base::FilePath* out_response_path);

  
  virtual void OnReceivedRedirect(URLRequest* request,
                                  const GURL& new_url,
                                  bool* defer_redirect) OVERRIDE;
  virtual void OnResponseStarted(URLRequest* request) OVERRIDE;
  virtual void OnReadCompleted(URLRequest* request,
                               int bytes_read) OVERRIDE;
  virtual void OnCertificateRequested(
      URLRequest* request,
      SSLCertRequestInfo* cert_request_info) OVERRIDE;

  URLFetcherDelegate* delegate() const { return delegate_; }
  static void CancelAll();
  static int GetNumFetcherCores();
  static void SetEnableInterceptionForTests(bool enabled);
  static void SetIgnoreCertificateRequests(bool ignored);

 private:
  friend class base::RefCountedThreadSafe<URLFetcherCore>;

  class Registry {
   public:
    Registry();
    ~Registry();

    void AddURLFetcherCore(URLFetcherCore* core);
    void RemoveURLFetcherCore(URLFetcherCore* core);

    void CancelAll();

    int size() const {
      return fetchers_.size();
    }

   private:
    std::set<URLFetcherCore*> fetchers_;

    DISALLOW_COPY_AND_ASSIGN(Registry);
  };

  virtual ~URLFetcherCore();

  
  
  void StartOnIOThread();
  void StartURLRequest();
  void DidInitializeWriter(int result);
  void StartURLRequestWhenAppropriate();
  void CancelURLRequest(int error);
  void OnCompletedURLRequest(base::TimeDelta backoff_delay);
  void InformDelegateFetchIsComplete();
  void NotifyMalformedContent();
  void DidFinishWriting(int result);
  void RetryOrCompleteUrlFetch();

  
  
  void ReleaseRequest();

  
  
  base::TimeTicks GetBackoffReleaseTime();

  void CompleteAddingUploadDataChunk(const std::string& data,
                                     bool is_last_chunk);

  
  // Returns OK if all bytes in |data| get written synchronously. Otherwise,
  
  int WriteBuffer(scoped_refptr<DrainableIOBuffer> data);

  
  void DidWriteBuffer(scoped_refptr<DrainableIOBuffer> data, int result);

  
  void ReadResponse();

  
  void InformDelegateUploadProgress();
  void InformDelegateUploadProgressInDelegateThread(int64 current, int64 total);
  void InformDelegateDownloadProgress();
  void InformDelegateDownloadProgressInDelegateThread(int64 current,
                                                      int64 total);

  URLFetcher* fetcher_;              
  GURL original_url_;                
  GURL url_;                         
  URLFetcher::RequestType request_type_;  
  URLRequestStatus status_;          
  URLFetcherDelegate* delegate_;     
  
  scoped_refptr<base::SingleThreadTaskRunner> delegate_task_runner_;
  
  scoped_refptr<base::SingleThreadTaskRunner> network_task_runner_;
  
  scoped_refptr<base::TaskRunner> upload_file_task_runner_;
  scoped_ptr<URLRequest> request_;   
  int load_flags_;                   
  int response_code_;                
  scoped_refptr<IOBuffer> buffer_;
                                     
  scoped_refptr<URLRequestContextGetter> request_context_getter_;
                                     
  GURL first_party_for_cookies_;     
  
  const void* url_request_data_key_;
  URLFetcher::CreateDataCallback url_request_create_data_callback_;
  ResponseCookies cookies_;          
  HttpRequestHeaders extra_request_headers_;
  scoped_refptr<HttpResponseHeaders> response_headers_;
  bool was_fetched_via_proxy_;
  HostPortPair socket_address_;

  bool upload_content_set_;          
  std::string upload_content_;       
  base::FilePath upload_file_path_;  
  uint64 upload_range_offset_;       
                                     
  uint64 upload_range_length_;       
                                     
  std::string upload_content_type_;  
  std::string referrer_;             
  bool is_chunked_upload_;           

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  scoped_refptr<URLRequestThrottlerEntryInterface>
      original_url_throttler_entry_;
  scoped_refptr<URLRequestThrottlerEntryInterface> url_throttler_entry_;

  
  bool was_cancelled_;

  
  scoped_ptr<URLFetcherResponseWriter> response_writer_;

  
  
  
  bool stop_on_redirect_;
  
  
  
  bool stopped_on_redirect_;

  
  
  
  
  bool automatically_retry_on_5xx_;
  
  
  
  
  int num_retries_on_5xx_;
  
  int max_retries_on_5xx_;
  
  base::TimeDelta backoff_delay_;

  
  int num_retries_on_network_changes_;
  
  
  int max_retries_on_network_changes_;

  
  
  scoped_ptr<base::RepeatingTimer<URLFetcherCore> >
      upload_progress_checker_timer_;
  
  int64 current_upload_bytes_;
  
  int64 current_response_bytes_;
  
  int64 total_response_bytes_;

  
  base::debug::StackTrace stack_trace_;

  static base::LazyInstance<Registry> g_registry;

  DISALLOW_COPY_AND_ASSIGN(URLFetcherCore);
};

}  

#endif  
