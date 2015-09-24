/** ---------------------------------------------------------------------------
 Copyright (c) 2011, Code Aurora Forum. All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are
 met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above
       copyright notice, this list of conditions and the following
       disclaimer in the documentation and/or other materials provided
       with the distribution.
     * Neither the name of Code Aurora Forum, Inc. nor the names of its
       contributors may be used to endorse or promote products derived
       from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 -----------------------------------------------------------------------------**/
#ifndef __DNSRESOLVERHELPER_H__
#define __DNSRESOLVERHELPER_H__

#include <vector>
#include <string>

#include "config.h"
#include "net/base/host_resolver.h"
#include "net/base/address_list.h"
#include "net/base/net_log.h"
#include "net/base/net_errors.h"
#include "base/message_loop.h"
#include "net/base/completion_callback.h"
#include "hosts_provider.h"

class HostResolverHelper: public net::HostResolver::HostnameResolverExt {
public:
    HostResolverHelper(net::HostResolver* hostresolver);
    virtual ~HostResolverHelper();

    
    
    virtual void Resolve() {
        MessageLoop::current()->PostDelayedTask(FROM_HERE, NewRunnableFunction(&HostResolverHelper::DoResolve, this),
                500);
    }
    

    
    static void DoResolve(HostResolverHelper* obj);

    
    void Init(HostsProvider* provider);

private:

    
    bool StartHostsResolution();

private:
    int num_of_hosts_to_resolve;
    net::HostResolver* hostresolver_;
    HostsProvider* hostname_provider_;
    

    class HostInfo: public base::RefCounted<HostInfo> {
    public:
        net::AddressList addrlist;
        net::HostResolver::RequestInfo reqinfo;
        net::HostResolver::RequestHandle reqhandle;
        bool pending;
        net::CompletionCallbackImpl<HostInfo> completion_callback_;

        HostInfo(const std::string& hostname);
        ~HostInfo() {
        }

        void OnLookupFinished(int result);

    };

    std::vector<scoped_refptr<HostInfo> > hostinfo_list_;

    
    void CancelAllRequests();
    void PrepareRequestsData(const std::vector<std::string>& hostnames);

};

net::HostResolver::HostnameResolverExt* CreateResolverIPObserver(net::HostResolver* hostResolver);

#endif
