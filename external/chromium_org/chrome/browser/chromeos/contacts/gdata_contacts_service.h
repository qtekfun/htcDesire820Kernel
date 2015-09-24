// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CONTACTS_GDATA_CONTACTS_SERVICE_H_
#define CHROME_BROWSER_CHROMEOS_CONTACTS_GDATA_CONTACTS_SERVICE_H_

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/time/time.h"
#include "url/gurl.h"

class Profile;

namespace base {
class Value;
}  

namespace google_apis {
class AuthServiceInterface;
class RequestSender;
}  

namespace net {
class URLRequestContextGetter;
}  

namespace contacts {

class Contact;

class GDataContactsServiceInterface {
 public:
  typedef base::Callback<void(scoped_ptr<ScopedVector<contacts::Contact> >)>
      SuccessCallback;
  typedef base::Closure FailureCallback;

  virtual ~GDataContactsServiceInterface() {}

  
  
  
  virtual void DownloadContacts(SuccessCallback success_callback,
                                FailureCallback failure_callback,
                                const base::Time& min_update_time) = 0;

 protected:
  GDataContactsServiceInterface() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(GDataContactsServiceInterface);
};

class GDataContactsService : public GDataContactsServiceInterface {
 public:
  typedef base::Callback<std::string(const std::string&)>
      RewritePhotoUrlCallback;

  GDataContactsService(
      net::URLRequestContextGetter* url_request_context_getter,
      google_apis::AuthServiceInterface* auth_service);
  virtual ~GDataContactsService();

  const std::string& cached_my_contacts_group_id_for_testing() const {
    return cached_my_contacts_group_id_;
  }
  void clear_cached_my_contacts_group_id_for_testing() {
    cached_my_contacts_group_id_.clear();
  }

  void set_max_photo_downloads_per_second_for_testing(int max_downloads) {
    max_photo_downloads_per_second_ = max_downloads;
  }
  void set_photo_download_timer_interval_for_testing(base::TimeDelta interval) {
    photo_download_timer_interval_ = interval;
  }
  void set_groups_feed_url_for_testing(const GURL& url) {
    groups_feed_url_for_testing_ = url;
  }
  void set_contacts_feed_url_for_testing(const GURL& url) {
    contacts_feed_url_for_testing_ = url;
  }
  void set_rewrite_photo_url_callback_for_testing(RewritePhotoUrlCallback cb) {
    rewrite_photo_url_callback_for_testing_ = cb;
  }

  
  virtual void DownloadContacts(SuccessCallback success_callback,
                                FailureCallback failure_callback,
                                const base::Time& min_update_time) OVERRIDE;

 private:
  class DownloadContactsRequest;

  
  
  void OnRequestComplete(DownloadContactsRequest* request);

  scoped_ptr<google_apis::RequestSender> sender_;

  
  
  std::string cached_my_contacts_group_id_;

  
  std::set<DownloadContactsRequest*> requests_;

  
  
  int max_photo_downloads_per_second_;

  
  
  
  base::TimeDelta photo_download_timer_interval_;

  
  GURL groups_feed_url_for_testing_;
  GURL contacts_feed_url_for_testing_;

  
  
  
  RewritePhotoUrlCallback rewrite_photo_url_callback_for_testing_;

  DISALLOW_COPY_AND_ASSIGN(GDataContactsService);
};

}  

#endif  
