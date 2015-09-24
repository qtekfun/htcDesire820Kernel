// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_FETCHER_H_
#define NET_URL_REQUEST_URL_FETCHER_H_

#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/supports_user_data.h"
#include "net/base/net_export.h"

class GURL;

namespace base {
class FilePath;
class MessageLoopProxy;
class SequencedTaskRunner;
class TaskRunner;
class TimeDelta;
}

namespace net {
class HostPortPair;
class HttpRequestHeaders;
class HttpResponseHeaders;
class URLFetcherDelegate;
class URLFetcherResponseWriter;
class URLRequestContextGetter;
class URLRequestStatus;
typedef std::vector<std::string> ResponseCookies;

class NET_EXPORT URLFetcher {
 public:
  
  
  enum ResponseCode {
    RESPONSE_CODE_INVALID = -1
  };

  enum RequestType {
    GET,
    POST,
    HEAD,
    DELETE_REQUEST,   
                      
    PUT,
    PATCH,
  };

  
  
  typedef base::Callback<base::SupportsUserData::Data*()> CreateDataCallback;

  virtual ~URLFetcher();

  
  
  
  static URLFetcher* Create(const GURL& url,
                            URLFetcher::RequestType request_type,
                            URLFetcherDelegate* d);

  
  
  
  static URLFetcher* Create(int id,
                            const GURL& url,
                            URLFetcher::RequestType request_type,
                            URLFetcherDelegate* d);

  
  
  
  
  
  
  static void CancelAll();

  
  
  
  static void SetEnableInterceptionForTests(bool enabled);

  
  
  
  
  
  static void SetIgnoreCertificateRequests(bool ignored);

  
  
  
  
  
  virtual void SetUploadData(const std::string& upload_content_type,
                             const std::string& upload_content) = 0;

  
  
  
  
  
  
  
  
  virtual void SetUploadFilePath(
      const std::string& upload_content_type,
      const base::FilePath& file_path,
      uint64 range_offset,
      uint64 range_length,
      scoped_refptr<base::TaskRunner> file_task_runner) = 0;

  
  
  
  virtual void SetChunkedUpload(const std::string& upload_content_type) = 0;

  
  
  
  virtual void AppendChunkToUpload(const std::string& data,
                                   bool is_last_chunk) = 0;

  
  
  virtual void SetLoadFlags(int load_flags) = 0;

  
  virtual int GetLoadFlags() const = 0;

  
  
  virtual void SetReferrer(const std::string& referrer) = 0;

  
  
  
  virtual void SetExtraRequestHeaders(
      const std::string& extra_request_headers) = 0;

  
  
  
  virtual void AddExtraRequestHeader(const std::string& header_line) = 0;

  virtual void GetExtraRequestHeaders(
      HttpRequestHeaders* headers) const = 0;

  
  
  virtual void SetRequestContext(
      URLRequestContextGetter* request_context_getter) = 0;

  
  
  virtual void SetFirstPartyForCookies(
      const GURL& first_party_for_cookies) = 0;

  
  
  virtual void SetURLRequestUserData(
      const void* key,
      const CreateDataCallback& create_data_callback) = 0;

  
  
  
  
  
  virtual void SetStopOnRedirect(bool stop_on_redirect) = 0;

  
  
  
  virtual void SetAutomaticallyRetryOn5xx(bool retry) = 0;

  virtual void SetMaxRetriesOn5xx(int max_retries) = 0;
  virtual int GetMaxRetriesOn5xx() const = 0;

  
  
  virtual base::TimeDelta GetBackoffDelay() const = 0;

  
  
  
  virtual void SetAutomaticallyRetryOnNetworkChanges(int max_retries) = 0;

  
  
  
  
  
  
  virtual void SaveResponseToFileAtPath(
      const base::FilePath& file_path,
      scoped_refptr<base::SequencedTaskRunner> file_task_runner) = 0;

  
  
  
  
  
  virtual void SaveResponseToTemporaryFile(
      scoped_refptr<base::SequencedTaskRunner> file_task_runner) = 0;

  
  
  virtual void SaveResponseWithWriter(
      scoped_ptr<URLFetcherResponseWriter> response_writer) = 0;

  
  
  virtual HttpResponseHeaders* GetResponseHeaders() const = 0;

  
  
  
  virtual HostPortPair GetSocketAddress() const = 0;

  
  
  
  virtual bool WasFetchedViaProxy() const = 0;

  
  
  virtual void Start() = 0;

  
  virtual const GURL& GetOriginalURL() const = 0;

  
  virtual const GURL& GetURL() const = 0;

  
  virtual const URLRequestStatus& GetStatus() const = 0;

  
  
  virtual int GetResponseCode() const = 0;

  
  virtual const ResponseCookies& GetCookies() const = 0;

  
  virtual void ReceivedContentWasMalformed() = 0;

  
  
  virtual bool GetResponseAsString(std::string* out_response_string) const = 0;

  
  
  
  
  
  virtual bool GetResponseAsFilePath(
      bool take_ownership,
      base::FilePath* out_response_path) const = 0;
};

}  

#endif  
