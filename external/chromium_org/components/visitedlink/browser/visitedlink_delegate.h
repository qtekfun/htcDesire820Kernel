// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_VISITEDLINK_BROWSER_VISITEDLINK_DELEGATE_H_
#define COMPONENTS_VISITEDLINK_BROWSER_VISITEDLINK_DELEGATE_H_

#include "base/memory/ref_counted.h"

class GURL;

namespace content {
class BrowserContext;
}

namespace visitedlink {

class VisitedLinkDelegate {
 public:
  
  class URLEnumerator : public base::RefCountedThreadSafe<URLEnumerator> {
   public:
    
    virtual void OnURL(const GURL& url) = 0;

    
    
    
    virtual void OnComplete(bool success) = 0;

   protected:
    virtual ~URLEnumerator() {}

   private:
    friend class base::RefCountedThreadSafe<URLEnumerator>;
  };

  
  
  
  
  virtual void RebuildTable(const scoped_refptr<URLEnumerator>& enumerator) = 0;

 protected:
  virtual ~VisitedLinkDelegate() {}
};

}  

#endif  
