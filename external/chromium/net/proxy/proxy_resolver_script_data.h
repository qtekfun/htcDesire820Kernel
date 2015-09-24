// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_RESOLVER_SCRIPT_DATA_H_
#define NET_PROXY_PROXY_RESOLVER_SCRIPT_DATA_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/string16.h"
#include "googleurl/src/gurl.h"

namespace net {

class ProxyResolverScriptData
    : public base::RefCountedThreadSafe<ProxyResolverScriptData> {
 public:
  enum Type {
    TYPE_SCRIPT_CONTENTS,
    TYPE_SCRIPT_URL,
    TYPE_AUTO_DETECT,
  };

  
  static scoped_refptr<ProxyResolverScriptData> FromUTF8(
      const std::string& utf8);

  
  static scoped_refptr<ProxyResolverScriptData> FromUTF16(
      const string16& utf16);

  
  static scoped_refptr<ProxyResolverScriptData> FromURL(const GURL& url);

  
  static scoped_refptr<ProxyResolverScriptData> ForAutoDetect();

  Type type() const {
    return type_;
  }

  
  
  const string16& utf16() const;

  
  
  const GURL& url() const;

 private:
  friend class base::RefCountedThreadSafe<ProxyResolverScriptData>;
  ProxyResolverScriptData(Type type,
                          const GURL& url,
                          const string16& utf16);
  virtual ~ProxyResolverScriptData();


  const Type type_;
  const GURL url_;
  const string16 utf16_;
};

}  

#endif  
