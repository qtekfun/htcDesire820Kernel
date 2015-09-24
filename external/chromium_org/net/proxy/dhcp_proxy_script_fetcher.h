// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_DHCP_SCRIPT_FETCHER_H_
#define NET_PROXY_DHCP_SCRIPT_FETCHER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/proxy/proxy_script_fetcher.h"
#include "url/gurl.h"

namespace net {

class NET_EXPORT_PRIVATE DhcpProxyScriptFetcher {
 public:
  
  virtual ~DhcpProxyScriptFetcher();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Fetch(base::string16* utf16_text,
                    const CompletionCallback& callback) = 0;

  
  virtual void Cancel() = 0;

  
  
  virtual const GURL& GetPacURL() const = 0;

  
  
  virtual std::string GetFetcherName() const;

 protected:
  DhcpProxyScriptFetcher();

 private:
  DISALLOW_COPY_AND_ASSIGN(DhcpProxyScriptFetcher);
};

class NET_EXPORT_PRIVATE DoNothingDhcpProxyScriptFetcher
    : public DhcpProxyScriptFetcher {
 public:
  DoNothingDhcpProxyScriptFetcher();
  virtual ~DoNothingDhcpProxyScriptFetcher();

  virtual int Fetch(base::string16* utf16_text,
                    const CompletionCallback& callback) OVERRIDE;
  virtual void Cancel() OVERRIDE;
  virtual const GURL& GetPacURL() const OVERRIDE;
 private:
  GURL gurl_;
  DISALLOW_COPY_AND_ASSIGN(DoNothingDhcpProxyScriptFetcher);
};

}  

#endif  
