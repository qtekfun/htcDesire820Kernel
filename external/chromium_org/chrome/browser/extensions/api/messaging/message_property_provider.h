// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_MESSAGING_MESSAGE_PROPERTY_PROVIDER_H_
#define CHROME_BROWSER_EXTENSIONS_API_MESSAGING_MESSAGE_PROPERTY_PROVIDER_H_

#include <string>

#include "base/callback.h"

class GURL;
class Profile;

namespace base {
class TaskRunner;
}

namespace net {
class URLRequestContextGetter;
}

namespace extensions {

class MessagePropertyProvider {
 public:
  MessagePropertyProvider();

  typedef base::Callback<void(const std::string&)> DomainBoundCertCallback;

  
  
  
  void GetDomainBoundCert(Profile* profile,
                          const GURL& source_url,
                          const DomainBoundCertCallback& reply);

 private:
  struct GetDomainBoundCertOutput;

  static void GetDomainBoundCertOnIOThread(
      scoped_refptr<base::TaskRunner> original_task_runner,
      scoped_refptr<net::URLRequestContextGetter> request_context_getter,
      const std::string& host,
      const DomainBoundCertCallback& reply);

  static void GotDomainBoundCert(
      scoped_refptr<base::TaskRunner> original_task_runner,
      struct GetDomainBoundCertOutput* output,
      const DomainBoundCertCallback& reply,
      int status);

  DISALLOW_COPY_AND_ASSIGN(MessagePropertyProvider);
};

}  

#endif  
