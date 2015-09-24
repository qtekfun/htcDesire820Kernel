// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_DRIVE_DRIVE_API_REQUESTS_H_
#define GOOGLE_APIS_DRIVE_DRIVE_API_REQUESTS_H_

#include <string>

#include "base/callback_forward.h"
#include "base/time/time.h"
#include "google_apis/drive/base_requests.h"
#include "google_apis/drive/drive_api_url_generator.h"
#include "google_apis/drive/drive_common_callbacks.h"

namespace google_apis {

class ChangeList;
class FileResource;
class FileList;

typedef base::Callback<void(GDataErrorCode error,
                            scoped_ptr<FileResource> entry)>
    FileResourceCallback;

typedef base::Callback<void(GDataErrorCode error,
                            scoped_ptr<FileList> entry)> FileListCallback;

typedef base::Callback<void(GDataErrorCode error,
                            scoped_ptr<ChangeList> entry)> ChangeListCallback;

namespace drive {


class DriveApiDataRequest : public GetDataRequest {
 public:
  DriveApiDataRequest(RequestSender* sender, const GetDataCallback& callback);
  virtual ~DriveApiDataRequest();

  
  const std::string& fields() const { return fields_; }
  void set_fields(const std::string& fields) { fields_ = fields; }

 protected:
  
  virtual GURL GetURL() const OVERRIDE;

  
  
  virtual GURL GetURLInternal() const = 0;

 private:
  std::string fields_;

  DISALLOW_COPY_AND_ASSIGN(DriveApiDataRequest);
};


class FilesGetRequest : public DriveApiDataRequest {
 public:
  FilesGetRequest(RequestSender* sender,
                  const DriveApiUrlGenerator& url_generator,
                  const FileResourceCallback& callback);
  virtual ~FilesGetRequest();

  
  const std::string& file_id() const { return file_id_; }
  void set_file_id(const std::string& file_id) { file_id_ = file_id; }

 protected:
  
  virtual GURL GetURLInternal() const OVERRIDE;

 private:
  const DriveApiUrlGenerator url_generator_;
  std::string file_id_;

  DISALLOW_COPY_AND_ASSIGN(FilesGetRequest);
};


class FilesInsertRequest : public DriveApiDataRequest {
 public:
  FilesInsertRequest(RequestSender* sender,
                     const DriveApiUrlGenerator& url_generator,
                     const FileResourceCallback& callback);
  virtual ~FilesInsertRequest();

  
  const std::string& mime_type() const { return mime_type_; }
  void set_mime_type(const std::string& mime_type) {
    mime_type_ = mime_type;
  }

  const std::vector<std::string>& parents() const { return parents_; }
  void add_parent(const std::string& parent) { parents_.push_back(parent); }

  const std::string& title() const { return title_; }
  void set_title(const std::string& title) { title_ = title; }

 protected:
  
  virtual net::URLFetcher::RequestType GetRequestType() const OVERRIDE;
  virtual bool GetContentData(std::string* upload_content_type,
                              std::string* upload_content) OVERRIDE;

  
  virtual GURL GetURLInternal() const OVERRIDE;

 private:
  const DriveApiUrlGenerator url_generator_;

  std::string mime_type_;
  std::vector<std::string> parents_;
  std::string title_;

  DISALLOW_COPY_AND_ASSIGN(FilesInsertRequest);
};


class FilesPatchRequest : public DriveApiDataRequest {
 public:
  FilesPatchRequest(RequestSender* sender,
                    const DriveApiUrlGenerator& url_generator,
                    const FileResourceCallback& callback);
  virtual ~FilesPatchRequest();

  
  const std::string& file_id() const { return file_id_; }
  void set_file_id(const std::string& file_id) { file_id_ = file_id; }

  
  bool set_modified_date() const { return set_modified_date_; }
  void set_set_modified_date(bool set_modified_date) {
    set_modified_date_ = set_modified_date;
  }

  bool update_viewed_date() const { return update_viewed_date_; }
  void set_update_viewed_date(bool update_viewed_date) {
    update_viewed_date_ = update_viewed_date;
  }

  
  
  
  
  const std::string& title() const { return title_; }
  void set_title(const std::string& title) { title_ = title; }

  const base::Time& modified_date() const { return modified_date_; }
  void set_modified_date(const base::Time& modified_date) {
    modified_date_ = modified_date;
  }

  const base::Time& last_viewed_by_me_date() const {
    return last_viewed_by_me_date_;
  }
  void set_last_viewed_by_me_date(const base::Time& last_viewed_by_me_date) {
    last_viewed_by_me_date_ = last_viewed_by_me_date;
  }

  const std::vector<std::string>& parents() const { return parents_; }
  void add_parent(const std::string& parent) { parents_.push_back(parent); }

 protected:
  
  virtual net::URLFetcher::RequestType GetRequestType() const OVERRIDE;
  virtual std::vector<std::string> GetExtraRequestHeaders() const OVERRIDE;
  virtual bool GetContentData(std::string* upload_content_type,
                              std::string* upload_content) OVERRIDE;

  
  virtual GURL GetURLInternal() const OVERRIDE;

 private:
  const DriveApiUrlGenerator url_generator_;

  std::string file_id_;
  bool set_modified_date_;
  bool update_viewed_date_;

  std::string title_;
  base::Time modified_date_;
  base::Time last_viewed_by_me_date_;
  std::vector<std::string> parents_;

  DISALLOW_COPY_AND_ASSIGN(FilesPatchRequest);
};


class FilesCopyRequest : public DriveApiDataRequest {
 public:
  
  FilesCopyRequest(RequestSender* sender,
                   const DriveApiUrlGenerator& url_generator,
                   const FileResourceCallback& callback);
  virtual ~FilesCopyRequest();

  
  const std::string& file_id() const { return file_id_; }
  void set_file_id(const std::string& file_id) { file_id_ = file_id; }

  
  const std::vector<std::string>& parents() const { return parents_; }
  void add_parent(const std::string& parent) { parents_.push_back(parent); }

  const base::Time& modified_date() const { return modified_date_; }
  void set_modified_date(const base::Time& modified_date) {
    modified_date_ = modified_date;
  }

  const std::string& title() const { return title_; }
  void set_title(const std::string& title) { title_ = title; }

 protected:
  
  virtual net::URLFetcher::RequestType GetRequestType() const OVERRIDE;
  virtual bool GetContentData(std::string* upload_content_type,
                              std::string* upload_content) OVERRIDE;

  
  virtual GURL GetURLInternal() const OVERRIDE;

 private:
  const DriveApiUrlGenerator url_generator_;

  std::string file_id_;
  base::Time modified_date_;
  std::vector<std::string> parents_;
  std::string title_;

  DISALLOW_COPY_AND_ASSIGN(FilesCopyRequest);
};


class FilesListRequest : public DriveApiDataRequest {
 public:
  FilesListRequest(RequestSender* sender,
                   const DriveApiUrlGenerator& url_generator,
                   const FileListCallback& callback);
  virtual ~FilesListRequest();

  
  int max_results() const { return max_results_; }
  void set_max_results(int max_results) { max_results_ = max_results; }

  const std::string& page_token() const { return page_token_; }
  void set_page_token(const std::string& page_token) {
    page_token_ = page_token;
  }

  const std::string& q() const { return q_; }
  void set_q(const std::string& q) { q_ = q; }

 protected:
  
  virtual GURL GetURLInternal() const OVERRIDE;

 private:
  const DriveApiUrlGenerator url_generator_;
  int max_results_;
  std::string page_token_;
  std::string q_;

  DISALLOW_COPY_AND_ASSIGN(FilesListRequest);
};


class FilesListNextPageRequest : public DriveApiDataRequest {
 public:
  FilesListNextPageRequest(RequestSender* sender,
                           const FileListCallback& callback);
  virtual ~FilesListNextPageRequest();

  const GURL& next_link() const { return next_link_; }
  void set_next_link(const GURL& next_link) { next_link_ = next_link; }

 protected:
  
  virtual GURL GetURLInternal() const OVERRIDE;

 private:
  GURL next_link_;

  DISALLOW_COPY_AND_ASSIGN(FilesListNextPageRequest);
};


class FilesDeleteRequest : public EntryActionRequest {
 public:
  FilesDeleteRequest(RequestSender* sender,
                     const DriveApiUrlGenerator& url_generator,
                     const EntryActionCallback& callback);
  virtual ~FilesDeleteRequest();

  
  const std::string& file_id() const { return file_id_; }
  void set_file_id(const std::string& file_id) { file_id_ = file_id; }
  void set_etag(const std::string& etag) { etag_ = etag; }

 protected:
  
  virtual net::URLFetcher::RequestType GetRequestType() const OVERRIDE;
  virtual GURL GetURL() const OVERRIDE;
  virtual std::vector<std::string> GetExtraRequestHeaders() const OVERRIDE;

 private:
  const DriveApiUrlGenerator url_generator_;
  std::string file_id_;
  std::string etag_;

  DISALLOW_COPY_AND_ASSIGN(FilesDeleteRequest);
};


class FilesTrashRequest : public DriveApiDataRequest {
 public:
  FilesTrashRequest(RequestSender* sender,
                    const DriveApiUrlGenerator& url_generator,
                    const FileResourceCallback& callback);
  virtual ~FilesTrashRequest();

  
  const std::string& file_id() const { return file_id_; }
  void set_file_id(const std::string& file_id) { file_id_ = file_id; }

 protected:
  
  virtual net::URLFetcher::RequestType GetRequestType() const OVERRIDE;

  
  virtual GURL GetURLInternal() const OVERRIDE;

 private:
  const DriveApiUrlGenerator url_generator_;
  std::string file_id_;

  DISALLOW_COPY_AND_ASSIGN(FilesTrashRequest);
};


class AboutGetRequest : public DriveApiDataRequest {
 public:
  AboutGetRequest(RequestSender* sender,
                  const DriveApiUrlGenerator& url_generator,
                  const AboutResourceCallback& callback);
  virtual ~AboutGetRequest();

 protected:
  
  virtual GURL GetURLInternal() const OVERRIDE;

 private:
  const DriveApiUrlGenerator url_generator_;

  DISALLOW_COPY_AND_ASSIGN(AboutGetRequest);
};


class ChangesListRequest : public DriveApiDataRequest {
 public:
  ChangesListRequest(RequestSender* sender,
                     const DriveApiUrlGenerator& url_generator,
                     const ChangeListCallback& callback);
  virtual ~ChangesListRequest();

  
  bool include_deleted() const { return include_deleted_; }
  void set_include_deleted(bool include_deleted) {
    include_deleted_ = include_deleted;
  }

  int max_results() const { return max_results_; }
  void set_max_results(int max_results) { max_results_ = max_results; }

  const std::string& page_token() const { return page_token_; }
  void set_page_token(const std::string& page_token) {
    page_token_ = page_token;
  }

  int64 start_change_id() const { return start_change_id_; }
  void set_start_change_id(int64 start_change_id) {
    start_change_id_ = start_change_id;
  }

 protected:
  
  virtual GURL GetURLInternal() const OVERRIDE;

 private:
  const DriveApiUrlGenerator url_generator_;
  bool include_deleted_;
  int max_results_;
  std::string page_token_;
  int64 start_change_id_;

  DISALLOW_COPY_AND_ASSIGN(ChangesListRequest);
};


class ChangesListNextPageRequest : public DriveApiDataRequest {
 public:
  ChangesListNextPageRequest(RequestSender* sender,
                             const ChangeListCallback& callback);
  virtual ~ChangesListNextPageRequest();

  const GURL& next_link() const { return next_link_; }
  void set_next_link(const GURL& next_link) { next_link_ = next_link; }

 protected:
  
  virtual GURL GetURLInternal() const OVERRIDE;

 private:
  GURL next_link_;

  DISALLOW_COPY_AND_ASSIGN(ChangesListNextPageRequest);
};


class AppsListRequest : public DriveApiDataRequest {
 public:
  AppsListRequest(RequestSender* sender,
                  const DriveApiUrlGenerator& url_generator,
                  const AppListCallback& callback);
  virtual ~AppsListRequest();

 protected:
  
  virtual GURL GetURLInternal() const OVERRIDE;

 private:
  const DriveApiUrlGenerator url_generator_;

  DISALLOW_COPY_AND_ASSIGN(AppsListRequest);
};


class ChildrenInsertRequest : public EntryActionRequest {
 public:
  ChildrenInsertRequest(RequestSender* sender,
                        const DriveApiUrlGenerator& url_generator,
                        const EntryActionCallback& callback);
  virtual ~ChildrenInsertRequest();

  
  const std::string& folder_id() const { return folder_id_; }
  void set_folder_id(const std::string& folder_id) {
    folder_id_ = folder_id;
  }

  
  const std::string& id() const { return id_; }
  void set_id(const std::string& id) { id_ = id; }

 protected:
  
  virtual net::URLFetcher::RequestType GetRequestType() const OVERRIDE;
  virtual GURL GetURL() const OVERRIDE;
  virtual bool GetContentData(std::string* upload_content_type,
                              std::string* upload_content) OVERRIDE;

 private:
  const DriveApiUrlGenerator url_generator_;
  std::string folder_id_;
  std::string id_;

  DISALLOW_COPY_AND_ASSIGN(ChildrenInsertRequest);
};


class ChildrenDeleteRequest : public EntryActionRequest {
 public:
  
  ChildrenDeleteRequest(RequestSender* sender,
                        const DriveApiUrlGenerator& url_generator,
                        const EntryActionCallback& callback);
  virtual ~ChildrenDeleteRequest();

  
  const std::string& child_id() const { return child_id_; }
  void set_child_id(const std::string& child_id) {
    child_id_ = child_id;
  }

  const std::string& folder_id() const { return folder_id_; }
  void set_folder_id(const std::string& folder_id) {
    folder_id_ = folder_id;
  }

 protected:
  
  virtual net::URLFetcher::RequestType GetRequestType() const OVERRIDE;
  virtual GURL GetURL() const OVERRIDE;

 private:
  const DriveApiUrlGenerator url_generator_;
  std::string child_id_;
  std::string folder_id_;

  DISALLOW_COPY_AND_ASSIGN(ChildrenDeleteRequest);
};


class InitiateUploadNewFileRequest : public InitiateUploadRequestBase {
 public:
  
  
  
  
  InitiateUploadNewFileRequest(RequestSender* sender,
                               const DriveApiUrlGenerator& url_generator,
                               const std::string& content_type,
                               int64 content_length,
                               const std::string& parent_resource_id,
                               const std::string& title,
                               const InitiateUploadCallback& callback);
  virtual ~InitiateUploadNewFileRequest();

 protected:
  
  virtual GURL GetURL() const OVERRIDE;
  virtual net::URLFetcher::RequestType GetRequestType() const OVERRIDE;
  virtual bool GetContentData(std::string* upload_content_type,
                              std::string* upload_content) OVERRIDE;

 private:
  const DriveApiUrlGenerator url_generator_;
  const std::string parent_resource_id_;
  const std::string title_;

  DISALLOW_COPY_AND_ASSIGN(InitiateUploadNewFileRequest);
};


class InitiateUploadExistingFileRequest : public InitiateUploadRequestBase {
 public:
  
  
  
  
  
  InitiateUploadExistingFileRequest(RequestSender* sender,
                                    const DriveApiUrlGenerator& url_generator,
                                    const std::string& content_type,
                                    int64 content_length,
                                    const std::string& resource_id,
                                    const std::string& etag,
                                    const InitiateUploadCallback& callback);
  virtual ~InitiateUploadExistingFileRequest();

 protected:
  
  virtual GURL GetURL() const OVERRIDE;
  virtual net::URLFetcher::RequestType GetRequestType() const OVERRIDE;
  virtual std::vector<std::string> GetExtraRequestHeaders() const OVERRIDE;

 private:
  const DriveApiUrlGenerator url_generator_;
  const std::string resource_id_;
  const std::string etag_;

  DISALLOW_COPY_AND_ASSIGN(InitiateUploadExistingFileRequest);
};

typedef base::Callback<void(
    const UploadRangeResponse& response,
    scoped_ptr<FileResource> new_resource)> UploadRangeCallback;


class ResumeUploadRequest : public ResumeUploadRequestBase {
 public:
  
  
  ResumeUploadRequest(RequestSender* sender,
                      const GURL& upload_location,
                      int64 start_position,
                      int64 end_position,
                      int64 content_length,
                      const std::string& content_type,
                      const base::FilePath& local_file_path,
                      const UploadRangeCallback& callback,
                      const ProgressCallback& progress_callback);
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
                         const GURL& upload_url,
                         int64 content_length,
                         const UploadRangeCallback& callback);
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
                      const DriveApiUrlGenerator& url_generator,
                      const std::string& resource_id,
                      const base::FilePath& output_file_path,
                      const DownloadActionCallback& download_action_callback,
                      const GetContentCallback& get_content_callback,
                      const ProgressCallback& progress_callback);
  virtual ~DownloadFileRequest();

  DISALLOW_COPY_AND_ASSIGN(DownloadFileRequest);
};

}  
}  

#endif  
