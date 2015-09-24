// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SSL_SSL_REQUEST_INFO_H_
#define CHROME_BROWSER_SSL_SSL_REQUEST_INFO_H_
#pragma once

#include <string>

#include "base/memory/ref_counted.h"
#include "googleurl/src/gurl.h"
#include "webkit/glue/resource_type.h"

class SSLRequestInfo : public base::RefCounted<SSLRequestInfo> {
 public:
  SSLRequestInfo(const GURL& url,
                 ResourceType::Type resource_type,
                 int child_id,
                 int ssl_cert_id,
                 int ssl_cert_status);

  const GURL& url() const { return url_; }
  ResourceType::Type resource_type() const { return resource_type_; }
  int child_id() const { return child_id_; }
  int ssl_cert_id() const { return ssl_cert_id_; }
  int ssl_cert_status() const { return ssl_cert_status_; }

 private:
  friend class base::RefCounted<SSLRequestInfo>;

  virtual ~SSLRequestInfo();

  GURL url_;
  ResourceType::Type resource_type_;
  int child_id_;
  int ssl_cert_id_;
  int ssl_cert_status_;

  DISALLOW_COPY_AND_ASSIGN(SSLRequestInfo);
};

#endif  