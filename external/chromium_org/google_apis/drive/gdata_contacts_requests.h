// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_DRIVE_GDATA_CONTACTS_REQUESTS_H_
#define GOOGLE_APIS_DRIVE_GDATA_CONTACTS_REQUESTS_H_

#include <string>

#include "base/time/time.h"
#include "google_apis/drive/base_requests.h"

namespace google_apis {


class GetContactGroupsRequest : public GetDataRequest {
 public:
  GetContactGroupsRequest(RequestSender* runner,
                          const GetDataCallback& callback);
  virtual ~GetContactGroupsRequest();

  void set_feed_url_for_testing(const GURL& url) {
    feed_url_for_testing_ = url;
  }

 protected:
  
  virtual GURL GetURL() const OVERRIDE;

 private:
  
  GURL feed_url_for_testing_;

  DISALLOW_COPY_AND_ASSIGN(GetContactGroupsRequest);
};


class GetContactsRequest : public GetDataRequest {
 public:
  GetContactsRequest(RequestSender* runner,
                     const std::string& group_id,
                     const base::Time& min_update_time,
                     const GetDataCallback& callback);
  virtual ~GetContactsRequest();

  void set_feed_url_for_testing(const GURL& url) {
    feed_url_for_testing_ = url;
  }

 protected:
  
  virtual GURL GetURL() const OVERRIDE;

 private:
  
  GURL feed_url_for_testing_;

  
  
  
  std::string group_id_;

  
  
  base::Time min_update_time_;

  DISALLOW_COPY_AND_ASSIGN(GetContactsRequest);
};


class GetContactPhotoRequest : public UrlFetchRequestBase {
 public:
  GetContactPhotoRequest(RequestSender* runner,
                         const GURL& photo_url,
                         const GetContentCallback& callback);
  virtual ~GetContactPhotoRequest();

 protected:
  
  virtual GURL GetURL() const OVERRIDE;
  virtual void ProcessURLFetchResults(const net::URLFetcher* source) OVERRIDE;
  virtual void RunCallbackOnPrematureFailure(GDataErrorCode code) OVERRIDE;

 private:
  
  GURL photo_url_;

  
  GetContentCallback callback_;

  DISALLOW_COPY_AND_ASSIGN(GetContactPhotoRequest);
};

}  

#endif  
