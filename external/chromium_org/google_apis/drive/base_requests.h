// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_DRIVE_BASE_REQUESTS_H_
#define GOOGLE_APIS_DRIVE_BASE_REQUESTS_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/thread_checker.h"
#include "google_apis/drive/gdata_errorcode.h"
#include "net/url_request/url_fetcher.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "net/url_request/url_fetcher_response_writer.h"
#include "url/gurl.h"

namespace base {
class Value;
}  

namespace google_apis {

class RequestSender;

typedef base::Callback<void(scoped_ptr<base::Value> value)> ParseJsonCallback;

typedef base::Callback<void(int64 progress, int64 total)> ProgressCallback;

typedef base::Callback<void(
    GDataErrorCode error,
    scoped_ptr<std::string> content)> GetContentCallback;

void ParseJson(base::TaskRunner* blocking_task_runner,
               const std::string& json,
               const ParseJsonCallback& callback);


class AuthenticatedRequestInterface {
 public:
  
  typedef base::Callback<void(AuthenticatedRequestInterface* request)>
      ReAuthenticateCallback;

  virtual ~AuthenticatedRequestInterface() {}

  
  
  
  
  
  
  
  
  virtual void Start(const std::string& access_token,
                     const std::string& custom_user_agent,
                     const ReAuthenticateCallback& callback) = 0;

  
  virtual void OnAuthFailed(GDataErrorCode code) = 0;

  
  
  
  
  
  virtual base::WeakPtr<AuthenticatedRequestInterface> GetWeakPtr() = 0;

  
  
  virtual void Cancel() = 0;
};


class ResponseWriter : public net::URLFetcherResponseWriter {
 public:
  
  
  ResponseWriter(base::SequencedTaskRunner* file_task_runner,
                 const base::FilePath& file_path,
                 const GetContentCallback& get_content_callback);
  virtual ~ResponseWriter();

  const std::string& data() const { return data_; }

  
  void DisownFile();

  
  virtual int Initialize(const net::CompletionCallback& callback) OVERRIDE;
  virtual int Write(net::IOBuffer* buffer,
                    int num_bytes,
                    const net::CompletionCallback& callback) OVERRIDE;
  virtual int Finish(const net::CompletionCallback& callback) OVERRIDE;

 private:
  void DidWrite(scoped_refptr<net::IOBuffer> buffer,
                const net::CompletionCallback& callback,
                int result);

  const GetContentCallback get_content_callback_;
  std::string data_;
  scoped_ptr<net::URLFetcherFileWriter> file_writer_;
  base::WeakPtrFactory<ResponseWriter> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ResponseWriter);
};


class UrlFetchRequestBase : public AuthenticatedRequestInterface,
                            public net::URLFetcherDelegate {
 public:
  
  virtual void Start(const std::string& access_token,
                     const std::string& custom_user_agent,
                     const ReAuthenticateCallback& callback) OVERRIDE;
  virtual base::WeakPtr<AuthenticatedRequestInterface> GetWeakPtr() OVERRIDE;
  virtual void Cancel() OVERRIDE;

 protected:
  explicit UrlFetchRequestBase(RequestSender* sender);
  virtual ~UrlFetchRequestBase();

  
  virtual GURL GetURL() const = 0;

  
  
  virtual net::URLFetcher::RequestType GetRequestType() const;

  
  
  virtual std::vector<std::string> GetExtraRequestHeaders() const;

  
  
  
  
  virtual bool GetContentData(std::string* upload_content_type,
                              std::string* upload_content);

  
  
  
  
  
  virtual bool GetContentFile(base::FilePath* local_file_path,
                              int64* range_offset,
                              int64* range_length,
                              std::string* upload_content_type);

  
  
  
  
  virtual void GetOutputFilePath(base::FilePath* local_file_path,
                                 GetContentCallback* get_content_callback);

  
  
  virtual void ProcessURLFetchResults(const net::URLFetcher* source) = 0;

  
  
  virtual void RunCallbackOnPrematureFailure(GDataErrorCode code) = 0;

  
  void OnProcessURLFetchResultsComplete();

  
  
  GDataErrorCode GetErrorCode();

  
  bool CalledOnValidThread();

  
  ResponseWriter* response_writer() const { return response_writer_; }

  
  base::SequencedTaskRunner* blocking_task_runner() const;

 private:
  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  virtual void OnAuthFailed(GDataErrorCode code) OVERRIDE;

  ReAuthenticateCallback re_authenticate_callback_;
  int re_authenticate_count_;
  scoped_ptr<net::URLFetcher> url_fetcher_;
  ResponseWriter* response_writer_;  
  RequestSender* sender_;
  GDataErrorCode error_code_;

  base::ThreadChecker thread_checker_;

  
  
  base::WeakPtrFactory<UrlFetchRequestBase> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(UrlFetchRequestBase);
};


typedef base::Callback<void(GDataErrorCode error)> EntryActionCallback;

class EntryActionRequest : public UrlFetchRequestBase {
 public:
  
  
  EntryActionRequest(RequestSender* sender,
                     const EntryActionCallback& callback);
  virtual ~EntryActionRequest();

 protected:
  
  virtual void ProcessURLFetchResults(const net::URLFetcher* source) OVERRIDE;
  virtual void RunCallbackOnPrematureFailure(GDataErrorCode code) OVERRIDE;

 private:
  const EntryActionCallback callback_;

  DISALLOW_COPY_AND_ASSIGN(EntryActionRequest);
};


typedef base::Callback<void(GDataErrorCode error,
                            scoped_ptr<base::Value> json_data)> GetDataCallback;

class GetDataRequest : public UrlFetchRequestBase {
 public:
  
  
  GetDataRequest(RequestSender* sender, const GetDataCallback& callback);
  virtual ~GetDataRequest();

 protected:
  
  virtual void ProcessURLFetchResults(const net::URLFetcher* source) OVERRIDE;
  virtual void RunCallbackOnPrematureFailure(
      GDataErrorCode fetch_error_code) OVERRIDE;

 private:
  
  void ParseResponse(GDataErrorCode fetch_error_code, const std::string& data);

  
  void OnDataParsed(GDataErrorCode fetch_error_code,
                    scoped_ptr<base::Value> value);

  const GetDataCallback callback_;

  
  
  base::WeakPtrFactory<GetDataRequest> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(GetDataRequest);
};



typedef base::Callback<void(GDataErrorCode error,
                            const GURL& upload_url)> InitiateUploadCallback;

class InitiateUploadRequestBase : public UrlFetchRequestBase {
 protected:
  
  
  
  
  InitiateUploadRequestBase(RequestSender* sender,
                            const InitiateUploadCallback& callback,
                            const std::string& content_type,
                            int64 content_length);
  virtual ~InitiateUploadRequestBase();

  
  virtual void ProcessURLFetchResults(const net::URLFetcher* source) OVERRIDE;
  virtual void RunCallbackOnPrematureFailure(GDataErrorCode code) OVERRIDE;
  virtual std::vector<std::string> GetExtraRequestHeaders() const OVERRIDE;

 private:
  const InitiateUploadCallback callback_;
  const std::string content_type_;
  const int64 content_length_;

  DISALLOW_COPY_AND_ASSIGN(InitiateUploadRequestBase);
};


struct UploadRangeResponse {
  UploadRangeResponse();
  UploadRangeResponse(GDataErrorCode code,
                      int64 start_position_received,
                      int64 end_position_received);
  ~UploadRangeResponse();

  GDataErrorCode code;
  
  
  
  
  
  
  int64 start_position_received;
  int64 end_position_received;
};

class UploadRangeRequestBase : public UrlFetchRequestBase {
 protected:
  
  UploadRangeRequestBase(RequestSender* sender, const GURL& upload_url);
  virtual ~UploadRangeRequestBase();

  
  virtual GURL GetURL() const OVERRIDE;
  virtual net::URLFetcher::RequestType GetRequestType() const OVERRIDE;
  virtual void ProcessURLFetchResults(const net::URLFetcher* source) OVERRIDE;
  virtual void RunCallbackOnPrematureFailure(GDataErrorCode code) OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void OnRangeRequestComplete(
      const UploadRangeResponse& response, scoped_ptr<base::Value> value) = 0;

 private:
  
  void OnDataParsed(GDataErrorCode code, scoped_ptr<base::Value> value);

  const GURL upload_url_;

  
  
  base::WeakPtrFactory<UploadRangeRequestBase> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(UploadRangeRequestBase);
};


class ResumeUploadRequestBase : public UploadRangeRequestBase {
 protected:
  
  
  
  
  
  
  
  
  
  ResumeUploadRequestBase(RequestSender* sender,
                          const GURL& upload_location,
                          int64 start_position,
                          int64 end_position,
                          int64 content_length,
                          const std::string& content_type,
                          const base::FilePath& local_file_path);
  virtual ~ResumeUploadRequestBase();

  
  virtual std::vector<std::string> GetExtraRequestHeaders() const OVERRIDE;
  virtual bool GetContentFile(base::FilePath* local_file_path,
                              int64* range_offset,
                              int64* range_length,
                              std::string* upload_content_type) OVERRIDE;

 private:
  
  const int64 start_position_;
  const int64 end_position_;
  const int64 content_length_;
  const std::string content_type_;
  const base::FilePath local_file_path_;

  DISALLOW_COPY_AND_ASSIGN(ResumeUploadRequestBase);
};


class GetUploadStatusRequestBase : public UploadRangeRequestBase {
 public:
  
  
  
  GetUploadStatusRequestBase(RequestSender* sender,
                             const GURL& upload_url,
                             int64 content_length);
  virtual ~GetUploadStatusRequestBase();

 protected:
  
  virtual std::vector<std::string> GetExtraRequestHeaders() const OVERRIDE;

 private:
  const int64 content_length_;

  DISALLOW_COPY_AND_ASSIGN(GetUploadStatusRequestBase);
};


typedef base::Callback<void(GDataErrorCode error,
                            const base::FilePath& temp_file)>
    DownloadActionCallback;

class DownloadFileRequestBase : public UrlFetchRequestBase {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  DownloadFileRequestBase(
      RequestSender* sender,
      const DownloadActionCallback& download_action_callback,
      const GetContentCallback& get_content_callback,
      const ProgressCallback& progress_callback,
      const GURL& download_url,
      const base::FilePath& output_file_path);
  virtual ~DownloadFileRequestBase();

 protected:
  
  virtual GURL GetURL() const OVERRIDE;
  virtual void GetOutputFilePath(
      base::FilePath* local_file_path,
      GetContentCallback* get_content_callback) OVERRIDE;
  virtual void ProcessURLFetchResults(const net::URLFetcher* source) OVERRIDE;
  virtual void RunCallbackOnPrematureFailure(GDataErrorCode code) OVERRIDE;

  
  virtual void OnURLFetchDownloadProgress(const net::URLFetcher* source,
                                          int64 current, int64 total) OVERRIDE;

 private:
  const DownloadActionCallback download_action_callback_;
  const GetContentCallback get_content_callback_;
  const ProgressCallback progress_callback_;
  const GURL download_url_;
  const base::FilePath output_file_path_;

  DISALLOW_COPY_AND_ASSIGN(DownloadFileRequestBase);
};

}  

#endif  
