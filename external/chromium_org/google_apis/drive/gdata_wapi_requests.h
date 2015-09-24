// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_DRIVE_GDATA_WAPI_REQUESTS_H_
#define GOOGLE_APIS_DRIVE_GDATA_WAPI_REQUESTS_H_

#include <string>
#include <vector>

#include "google_apis/drive/base_requests.h"
#include "google_apis/drive/drive_common_callbacks.h"
#include "google_apis/drive/gdata_wapi_url_generator.h"

namespace google_apis {

class AccountMetadata;
class GDataWapiUrlGenerator;
class ResourceEntry;


class GetResourceListRequest : public GetDataRequest {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  GetResourceListRequest(RequestSender* sender,
                         const GDataWapiUrlGenerator& url_generator,
                         const GURL& override_url,
                         int64 start_changestamp,
                         const std::string& search_string,
                         const std::string& directory_resource_id,
                         const GetResourceListCallback& callback);
  virtual ~GetResourceListRequest();

 protected:
  
  virtual GURL GetURL() const OVERRIDE;

 private:
  const GDataWapiUrlGenerator url_generator_;
  const GURL override_url_;
  const int64 start_changestamp_;
  const std::string search_string_;
  const std::string directory_resource_id_;

  DISALLOW_COPY_AND_ASSIGN(GetResourceListRequest);
};


class SearchByTitleRequest : public GetDataRequest {
 public:
  
  
  
  
  
  
  
  
  SearchByTitleRequest(RequestSender* sender,
                       const GDataWapiUrlGenerator& url_generator,
                       const std::string& title,
                       const std::string& directory_resource_id,
                       const GetResourceListCallback& callback);
  virtual ~SearchByTitleRequest();

 protected:
  
  virtual GURL GetURL() const OVERRIDE;

 private:
  const GDataWapiUrlGenerator url_generator_;
  const std::string title_;
  const std::string directory_resource_id_;

  DISALLOW_COPY_AND_ASSIGN(SearchByTitleRequest);
};


class GetResourceEntryRequest : public GetDataRequest {
 public:
  
  GetResourceEntryRequest(RequestSender* sender,
                          const GDataWapiUrlGenerator& url_generator,
                          const std::string& resource_id,
                          const GURL& embed_origin,
                          const GetDataCallback& callback);
  virtual ~GetResourceEntryRequest();

 protected:
  
  virtual GURL GetURL() const OVERRIDE;

 private:
  const GDataWapiUrlGenerator url_generator_;
  
  const std::string resource_id_;
  
  const GURL& embed_origin_;

  DISALLOW_COPY_AND_ASSIGN(GetResourceEntryRequest);
};


typedef base::Callback<void(GDataErrorCode error,
                            scoped_ptr<AccountMetadata> account_metadata)>
    GetAccountMetadataCallback;

class GetAccountMetadataRequest : public GetDataRequest {
 public:
  
  
  
  GetAccountMetadataRequest(RequestSender* sender,
                            const GDataWapiUrlGenerator& url_generator,
                            const GetAccountMetadataCallback& callback,
                            bool include_installed_apps);
  virtual ~GetAccountMetadataRequest();

 protected:
  
  virtual GURL GetURL() const OVERRIDE;

 private:
  const GDataWapiUrlGenerator url_generator_;
  const bool include_installed_apps_;

  DISALLOW_COPY_AND_ASSIGN(GetAccountMetadataRequest);
};


class DeleteResourceRequest : public EntryActionRequest {
 public:
  
  DeleteResourceRequest(RequestSender* sender,
                        const GDataWapiUrlGenerator& url_generator,
                        const EntryActionCallback& callback,
                        const std::string& resource_id,
                        const std::string& etag);
  virtual ~DeleteResourceRequest();

 protected:
  
  virtual GURL GetURL() const OVERRIDE;
  virtual net::URLFetcher::RequestType GetRequestType() const OVERRIDE;
  virtual std::vector<std::string> GetExtraRequestHeaders() const OVERRIDE;

 private:
  const GDataWapiUrlGenerator url_generator_;
  const std::string resource_id_;
  const std::string etag_;

  DISALLOW_COPY_AND_ASSIGN(DeleteResourceRequest);
};


class CreateDirectoryRequest : public GetDataRequest {
 public:
  
  
  
  
  CreateDirectoryRequest(RequestSender* sender,
                         const GDataWapiUrlGenerator& url_generator,
                         const GetDataCallback& callback,
                         const std::string& parent_resource_id,
                         const std::string& directory_title);
  virtual ~CreateDirectoryRequest();

 protected:
  
  virtual GURL GetURL() const OVERRIDE;
  virtual net::URLFetcher::RequestType GetRequestType() const OVERRIDE;
  virtual bool GetContentData(std::string* upload_content_type,
                              std::string* upload_content) OVERRIDE;

 private:
  const GDataWapiUrlGenerator url_generator_;
  const std::string parent_resource_id_;
  const std::string directory_title_;

  DISALLOW_COPY_AND_ASSIGN(CreateDirectoryRequest);
};


class RenameResourceRequest : public EntryActionRequest {
 public:
  
  RenameResourceRequest(RequestSender* sender,
                        const GDataWapiUrlGenerator& url_generator,
                        const EntryActionCallback& callback,
                        const std::string& resource_id,
                        const std::string& new_title);
  virtual ~RenameResourceRequest();

 protected:
  
  virtual net::URLFetcher::RequestType GetRequestType() const OVERRIDE;
  virtual std::vector<std::string> GetExtraRequestHeaders() const OVERRIDE;
  virtual GURL GetURL() const OVERRIDE;
  virtual bool GetContentData(std::string* upload_content_type,
                              std::string* upload_content) OVERRIDE;

 private:
  const GDataWapiUrlGenerator url_generator_;
  const std::string resource_id_;
  const std::string new_title_;

  DISALLOW_COPY_AND_ASSIGN(RenameResourceRequest);
};


class AuthorizeAppRequest : public GetDataRequest {
 public:
  
  AuthorizeAppRequest(RequestSender* sender,
                      const GDataWapiUrlGenerator& url_generator,
                      const AuthorizeAppCallback& callback,
                      const std::string& resource_id,
                      const std::string& app_id);
  virtual ~AuthorizeAppRequest();

 protected:
  
  virtual net::URLFetcher::RequestType GetRequestType() const OVERRIDE;
  virtual bool GetContentData(std::string* upload_content_type,
                              std::string* upload_content) OVERRIDE;
  virtual std::vector<std::string> GetExtraRequestHeaders() const OVERRIDE;
  virtual GURL GetURL() const OVERRIDE;

 private:
  const GDataWapiUrlGenerator url_generator_;
  const std::string resource_id_;
  const std::string app_id_;

  DISALLOW_COPY_AND_ASSIGN(AuthorizeAppRequest);
};


class AddResourceToDirectoryRequest : public EntryActionRequest {
 public:
  
  AddResourceToDirectoryRequest(RequestSender* sender,
                                const GDataWapiUrlGenerator& url_generator,
                                const EntryActionCallback& callback,
                                const std::string& parent_resource_id,
                                const std::string& resource_id);
  virtual ~AddResourceToDirectoryRequest();

 protected:
  
  virtual GURL GetURL() const OVERRIDE;
  virtual net::URLFetcher::RequestType GetRequestType() const OVERRIDE;
  virtual bool GetContentData(std::string* upload_content_type,
                              std::string* upload_content) OVERRIDE;

 private:
  const GDataWapiUrlGenerator url_generator_;
  const std::string parent_resource_id_;
  const std::string resource_id_;

  DISALLOW_COPY_AND_ASSIGN(AddResourceToDirectoryRequest);
};


class RemoveResourceFromDirectoryRequest : public EntryActionRequest {
 public:
  
  RemoveResourceFromDirectoryRequest(RequestSender* sender,
                                     const GDataWapiUrlGenerator& url_generator,
                                     const EntryActionCallback& callback,
                                     const std::string& parent_resource_id,
                                     const std::string& resource_id);
  virtual ~RemoveResourceFromDirectoryRequest();

 protected:
  
  virtual GURL GetURL() const OVERRIDE;
  virtual net::URLFetcher::RequestType GetRequestType() const OVERRIDE;
  virtual std::vector<std::string> GetExtraRequestHeaders() const OVERRIDE;

 private:
  const GDataWapiUrlGenerator url_generator_;
  const std::string resource_id_;
  const std::string parent_resource_id_;

  DISALLOW_COPY_AND_ASSIGN(RemoveResourceFromDirectoryRequest);
};


class InitiateUploadNewFileRequest : public InitiateUploadRequestBase {
 public:
  
  
  
  
  
  InitiateUploadNewFileRequest(RequestSender* sender,
                               const GDataWapiUrlGenerator& url_generator,
                               const InitiateUploadCallback& callback,
                               const std::string& content_type,
                               int64 content_length,
                               const std::string& parent_resource_id,
                               const std::string& title);
  virtual ~InitiateUploadNewFileRequest();

 protected:
  
  virtual GURL GetURL() const OVERRIDE;
  virtual net::URLFetcher::RequestType GetRequestType() const OVERRIDE;
  virtual bool GetContentData(std::string* upload_content_type,
                              std::string* upload_content) OVERRIDE;

 private:
  const GDataWapiUrlGenerator url_generator_;
  const std::string parent_resource_id_;
  const std::string title_;

  DISALLOW_COPY_AND_ASSIGN(InitiateUploadNewFileRequest);
};


class InitiateUploadExistingFileRequest
    : public InitiateUploadRequestBase {
 public:
  
  
  
  
  
  InitiateUploadExistingFileRequest(RequestSender* sender,
                                    const GDataWapiUrlGenerator& url_generator,
                                    const InitiateUploadCallback& callback,
                                    const std::string& content_type,
                                    int64 content_length,
                                    const std::string& resource_id,
                                    const std::string& etag);
  virtual ~InitiateUploadExistingFileRequest();

 protected:
  
  virtual GURL GetURL() const OVERRIDE;
  virtual net::URLFetcher::RequestType GetRequestType() const OVERRIDE;
  virtual std::vector<std::string> GetExtraRequestHeaders() const OVERRIDE;
  virtual bool GetContentData(std::string* upload_content_type,
                              std::string* upload_content) OVERRIDE;

 private:
  const GDataWapiUrlGenerator url_generator_;
  const std::string resource_id_;
  const std::string etag_;

  DISALLOW_COPY_AND_ASSIGN(InitiateUploadExistingFileRequest);
};


class ResumeUploadRequest : public ResumeUploadRequestBase {
 public:
  
  
  ResumeUploadRequest(RequestSender* sender,
                      const UploadRangeCallback& callback,
                      const ProgressCallback& progress_callback,
                      const GURL& upload_location,
                      int64 start_position,
                      int64 end_position,
                      int64 content_length,
                      const std::string& content_type,
                      const base::FilePath& local_file_path);
  virtual ~ResumeUploadRequest();

 protected:
  
  virtual void OnRangeRequestComplete(
      const UploadRangeResponse& response,
      scoped_ptr<base::Value> value) OVERRIDE;
  
  virtual void OnURLFetchUploadProgress(const net::URLFetcher* source,
                                        int64 current, int64 total) OVERRIDE;

 private:
  const UploadRangeCallback callback_;
  const ProgressCallback progress_callback_;

  DISALLOW_COPY_AND_ASSIGN(ResumeUploadRequest);
};


class GetUploadStatusRequest : public GetUploadStatusRequestBase {
 public:
  
  
  GetUploadStatusRequest(RequestSender* sender,
                         const UploadRangeCallback& callback,
                         const GURL& upload_url,
                         int64 content_length);
  virtual ~GetUploadStatusRequest();

 protected:
  
  virtual void OnRangeRequestComplete(
      const UploadRangeResponse& response,
      scoped_ptr<base::Value> value) OVERRIDE;

 private:
  const UploadRangeCallback callback_;

  DISALLOW_COPY_AND_ASSIGN(GetUploadStatusRequest);
};



class DownloadFileRequest : public DownloadFileRequestBase {
 public:
  
  DownloadFileRequest(RequestSender* sender,
                      const GDataWapiUrlGenerator& url_generator,
                      const DownloadActionCallback& download_action_callback,
                      const GetContentCallback& get_content_callback,
                      const ProgressCallback& progress_callback,
                      const std::string& resource_id,
                      const base::FilePath& output_file_path);
  virtual ~DownloadFileRequest();

  DISALLOW_COPY_AND_ASSIGN(DownloadFileRequest);
};

}  

#endif  
