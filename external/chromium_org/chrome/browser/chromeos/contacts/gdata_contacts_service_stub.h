// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CONTACTS_GDATA_CONTACTS_SERVICE_STUB_H_
#define CHROME_BROWSER_CHROMEOS_CONTACTS_GDATA_CONTACTS_SERVICE_STUB_H_

#include "chrome/browser/chromeos/contacts/gdata_contacts_service.h"

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/time/time.h"

namespace contacts {
typedef std::vector<const contacts::Contact*> ContactPointers;

class GDataContactsServiceStub : public GDataContactsServiceInterface {
 public:
  GDataContactsServiceStub();
  virtual ~GDataContactsServiceStub();

  int num_download_requests() const { return num_download_requests_; }
  int num_download_requests_with_wrong_timestamps() const {
    return num_download_requests_with_wrong_timestamps_;
  }
  void reset_stats() {
    num_download_requests_ = 0;
    num_download_requests_with_wrong_timestamps_ = 0;
  }
  void set_download_should_succeed(bool succeed) {
    download_should_succeed_ = succeed;
  }

  
  
  void SetContacts(const contacts::ContactPointers& contacts,
                   const base::Time& expected_min_update_time);

  
  virtual void DownloadContacts(SuccessCallback success_callback,
                                FailureCallback failure_callback,
                                const base::Time& min_update_time) OVERRIDE;

 private:
  
  int num_download_requests_;

  
  
  int num_download_requests_with_wrong_timestamps_;

  
  bool download_should_succeed_;

  
  ScopedVector<contacts::Contact> contacts_;

  
  
  base::Time expected_min_update_time_;

  DISALLOW_COPY_AND_ASSIGN(GDataContactsServiceStub);
};

}  

#endif  
