// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_NET_URL_REQUEST_CONTEXT_GETTER_H_
#define CHROME_COMMON_NET_URL_REQUEST_CONTEXT_GETTER_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/task.h"
#include "net/base/net_export.h"

namespace base {
class MessageLoopProxy;
}

namespace net {
class CookieStore;
class URLRequestContext;

struct URLRequestContextGetterTraits;

class NET_EXPORT URLRequestContextGetter
    : public base::RefCountedThreadSafe<URLRequestContextGetter,
                                        URLRequestContextGetterTraits> {
 public:
  virtual URLRequestContext* GetURLRequestContext() = 0;

  
  
  virtual CookieStore* DONTUSEME_GetCookieStore();

  
  
  
  virtual scoped_refptr<base::MessageLoopProxy>
      GetIOMessageLoopProxy() const = 0;

  
  
  
  
  
  
  void set_is_main(bool is_main) { is_main_ = is_main; }

  static bool is_freed;

 protected:
  friend class DeleteTask<const URLRequestContextGetter>;
  friend struct URLRequestContextGetterTraits;

  URLRequestContextGetter();
  virtual ~URLRequestContextGetter();

  bool is_main() const { return is_main_; }

 private:
  
  
  void OnDestruct() const;

  
  
  bool is_main_;
};

struct URLRequestContextGetterTraits {
  static void Destruct(const URLRequestContextGetter* context_getter) {
    context_getter->OnDestruct();
  }
};

}  

#endif  
