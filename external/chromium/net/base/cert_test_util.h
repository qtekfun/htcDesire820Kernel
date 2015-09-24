// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_CERT_TEST_UTIL_H_
#define NET_BASE_CERT_TEST_UTIL_H_
#pragma once

#include <string>

#include "base/memory/ref_counted.h"

class FilePath;

namespace net {

class X509Certificate;

FilePath GetTestCertsDirectory();

scoped_refptr<X509Certificate> ImportCertFromFile(const FilePath& certs_dir,
                                                  const std::string& cert_file);

}  

#endif  
